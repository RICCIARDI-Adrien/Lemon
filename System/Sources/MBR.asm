; Kernel Master Boot Record loader.
; Author : Adrien RICCIARDI
; V 1.0 : 02/09/2010
; V 1.1 : 14/11/2010
; V 1.2 : 01/08/2012
; V 1.3 : 25/11/2012, retrieve boot disk parameters using BIOS.
; V 1.4 : 27/12/2012, moved kernel in memory at address 0x10000 (linear) to avoid crossing 64K limit boundary when loading sectors.
; V 1.5 : 27/04/2014, SECTORS_TO_LOAD_COUNT is now defined on the assembler command line.
; V 1.6 : 22/06/2014, directly program the IDE controller when loading from hard disk to make the system load on a lot of buggy BIOSes.
; V 1.7 : 29/05/2015, added a valid partition table to be able to boot from USB stick, System now loads only from hard disk with custom LBA support whereas Installer loads only with BIOS support.
[BITS 16]
[ORG 0]

;--------------------------------------------------------------------------------------------------
; Constants and macros
;--------------------------------------------------------------------------------------------------
;%define DEBUG ; Uncomment to enable debug output
KERNEL_LOAD_SEGMENT EQU 1000h
KERNEL_PROTECTED_MODE_ADDRESS EQU (KERNEL_LOAD_SEGMENT * 16)

; IDE hard disk ports definition
HARD_DISK_PORT_DATA EQU 01F0h
HARD_DISK_PORT_SECTOR_COUNT EQU 01F2h
HARD_DISK_PORT_LBA_ADDRESS_LOW EQU 01F3h
HARD_DISK_PORT_LBA_ADDRESS_MIDDLE EQU 01F4h
HARD_DISK_PORT_LBA_ADDRESS_HIGH EQU 01F5h
HARD_DISK_PORT_DEVICE_HEAD EQU 01F6h
HARD_DISK_PORT_COMMAND EQU 01F7h
HARD_DISK_PORT_STATUS EQU 03F6h

; IDE hard disk useful commands
HARD_DISK_COMMAND_READ_WITH_RETRIES EQU 20h

;--------------------------------------------------------------------------------------------------
; Entry point
;--------------------------------------------------------------------------------------------------
Entry_Point:
	; Initialize stack and data segment
	cli
	mov ax, 07C0h
	mov ds, ax
	xor ax, ax
	mov ss, ax
	mov bp, ax
	mov sp, 0FFFFh
	mov [Boot_Device], dl
	sti

	; Disable BIOS text cursor
	mov ah, 2
	xor bh, bh
	mov dh, 25
	mov dl, 80
	int 10h

	; Enable use of 16 text colors instead of cursor blinking
	mov ah, 10h
	mov al, 3
	xor bl, bl
	int 10h

	; Load the Installer kernel from the boot device (there only one partition starting from zero, there is no need to read the partitions table)
	%ifdef INSTALLER
		call LoadKernelWithBIOS
	; Load the System kernel from the hard disk
	%else
		; Get the starting LBA sector of the first partition
		mov eax, DWORD [First_Partition_Starting_Sector_LBA]
		inc eax ; Bypass the first sector which is the MBR
		mov DWORD [Logical_Sector_Number], eax
		call LoadKernelWithLBA
	%endif
	
.Enter_Protected_Mode:
	; Load GDT
	cli
	lgdt [GDT_Pointer]

	; Enable 32-bit protected mode
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	; Flush cs register hidden part
	jmp DWORD .Flush_Processor_Cache
.Flush_Processor_Cache:

	; Initialize segment selectors
	mov ax, 16 ; Offset of data segment descriptor in GDT
	mov ds, ax
	mov es, ax
	;mov fs, ax CRASH
	;mov gs, ax CRASH
	mov ss, ax
	
	; Far jump to the kernel entry point.
	jmp DWORD 8:KERNEL_PROTECTED_MODE_ADDRESS ; '8' is to select the kernel code segment

;--------------------------------------------------------------------------------------------------
; Functions
;--------------------------------------------------------------------------------------------------
; Use the BIOS to access to the storage disk
LoadKernelWithBIOS:
	; Get boot drive parameters
	mov ah, 8
	mov dl, [Boot_Device]
	int 13h

	; Get heads count
	inc dh
	movzx ax, dh
	mov [Heads_Count], ax

	%ifdef DEBUG
		xor di, di
		call WriteHex
		mov ax, 10h
		int 16h
	%endif

	; Get sectors per track count
	and cl, 3Fh
	movzx ax, cl
	mov [Sectors_Per_Track_Count], ax

	%ifdef DEBUG
		mov di, 160
		call WriteHex
		mov ax, 10h
		int 16h
	%endif

	; Load kernel
	mov ax, KERNEL_LOAD_SEGMENT
	mov es, ax
	xor bx, bx
	mov cx, SECTORS_TO_LOAD_COUNT

	; Loop for loading kernel sectors
.Loop_Load_Kernel_With_BIOS:
	push cx

	; Convert LBA to CHS
	xor dx, dx
	mov ax, [Logical_Sector_Number]
	div WORD [Sectors_Per_Track_Count]
	inc dl
	mov cl, dl ; Sector
	xor dx, dx
	div WORD [Heads_Count]
	mov ch, al ; Cylinder
	mov dh, dl ; Head

	; Load sector
	mov ah, 2
	mov al, 1 ; Load 1 sector by time
	mov dl, [Boot_Device]
	int 13h

 	inc WORD [Logical_Sector_Number]
	add bx, 512

	; Go to next memory segment if this one is full
	or bx, bx
	jnz .Loop_Load_Kernel_With_BIOS_Continue_Loading
	mov ax, es
	add ax, 1000h ; 64KB / 16
	mov es, ax

.Loop_Load_Kernel_With_BIOS_Continue_Loading:
	pop cx
	loop .Loop_Load_Kernel_With_BIOS

	ret

; Program the IDE controller to load the kernel from the hard disk
LoadKernelWithLBA:
	cli
	mov ax, KERNEL_LOAD_SEGMENT
	mov es, ax
	xor di, di
	mov cx, SECTORS_TO_LOAD_COUNT

.Loop_Load_Kernel_With_LBA:
	; Wait for the controller to be ready
	mov dx, HARD_DISK_PORT_STATUS
.Wait_Controller_Ready_1:
	in al, dx
	test al, 80h
	jnz .Wait_Controller_Ready_1

	; Select master device
	mov al, 40h
	mov dx, HARD_DISK_PORT_DEVICE_HEAD
	out dx, al
	
	; Send sectors count high byte (the sector count is always 1 to avoid issues with the 400ns delay between sectors)
	xor al, al
	mov dx, HARD_DISK_PORT_SECTOR_COUNT
	out dx, al
	; Send sectors count low byte
	mov al, 1
	mov dx, HARD_DISK_PORT_SECTOR_COUNT
	out dx, al
	
	; Send 48-bit LBA byte 3
	mov al, BYTE [Logical_Sector_Number + 3]
	mov dx, HARD_DISK_PORT_LBA_ADDRESS_LOW
	out dx, al
	; Send 48-bit LBA byte 4 (always zero as the LBA value is 4-bytes long)
	xor al, al
	mov dx, HARD_DISK_PORT_LBA_ADDRESS_MIDDLE
	out dx, al
	; Send 48-bit LBA byte 5 (always zero as the LBA value is 4-bytes long)
	xor al, al
	mov dx, HARD_DISK_PORT_LBA_ADDRESS_HIGH
	out dx, al
	; Send 48-bit LBA byte 0
	mov al, BYTE [Logical_Sector_Number]
	mov dx, HARD_DISK_PORT_LBA_ADDRESS_LOW
	out dx, al
	; Send 48-bit LBA byte 1
	mov al, BYTE [Logical_Sector_Number + 1]
	mov dx, HARD_DISK_PORT_LBA_ADDRESS_MIDDLE
	out dx, al
	; Send 48-bit LBA byte 2
	mov al, BYTE [Logical_Sector_Number + 2]
	mov dx, HARD_DISK_PORT_LBA_ADDRESS_HIGH
	out dx, al

	; Send read command with automatic retries
	mov al, HARD_DISK_COMMAND_READ_WITH_RETRIES
	mov dx, HARD_DISK_PORT_COMMAND
	out dx, al

	; Wait for read clearance
	mov dx, HARD_DISK_PORT_STATUS
.Wait_Controller_Ready_2:
	in al, dx
	test al, 80h
	jnz .Wait_Controller_Ready_2

	; Read data
	mov ax, cx ; Push cx
	cld ; Auto increment di register
	mov cx, 256 ; Sector size is divided by two because we read words from the bus
	mov dx, HARD_DISK_PORT_DATA
	rep insw
	mov cx, ax ; Pop cx

	inc DWORD [Logical_Sector_Number]
	loop .Loop_Load_Kernel_With_LBA

	sti
	ret

%ifdef DEBUG
	; Convert a number in its hexadecimal representation and display it
	; @param ax : The number to display
	; @param di : The screen offset
	WriteHex:
		push bx
		push cx
		push es

		; Change to video memory text segment
		mov bx, 0B800h
		mov es, bx
		; Hold the shifting amount
		mov cl, 12
		; The loop counter
		mov ch, 4

	.Loop:
		; Get the nibble to display (the leftmost one) in "bl"
		mov bx, ax
		shr bx, cl
		and bl, 0Fh
		sub cl, 4 ; Next nibble

		; Is the nibble a digit ?
		cmp bl, 10
		jae .Convert_Letter_To_ASCII

	.Convert_Digit_To_ASCII:
		add bl, 48
		jmp .Display_Digit

	.Convert_Letter_To_ASCII:
		add bl, 65 - 10

	.Display_Digit:
		mov [es:di], bl
		mov BYTE [es:di + 1], 4
		add di, 2

		; Stop when the shifting amount is negative
		dec ch
		ja .Loop

		pop es
		pop cx
		pop bx
		ret
%endif

;--------------------------------------------------------------------------------------------------
; Variables
;--------------------------------------------------------------------------------------------------
Global_Descriptor_Table DB 0, 0, 0, 0, 0, 0, 0, 0
                        DB 0FFh, 0FFh, 0, 0, 0, 10011010b, 11001111b, 0 ; Code exec-read
                        DB 0FFh, 0FFh, 0, 0, 0, 10010010b, 11001111b, 0 ; Data read-write

GDT_Pointer DW 8 * 3
             DD Global_Descriptor_Table + 7C00h

Boot_Device DB 0
Logical_Sector_Number DD 1
Sectors_Per_Track_Count DW 18 ; Like a 1.44MB floppy disk
Heads_Count DW 2 ; Like a 1.44MB floppy disk

; The partition table starts at offset 446
times 446 - ($ - $$) nop
; First partition entry
DB 80h, ; Mark the partition as active
DB 01h, 01h, 00h ; CHS address of first absolute sector in partition (genisoimage wants it to be 0/1/1)
DB 0EFh ; Partition type (allow FAT12, FAT16, FAT32 or other file system)
DB 01h, 12h, 4Fh ; CHS address of last absolute sector in partition (set the last one of a 1.44MB floppy disk)
First_Partition_Starting_Sector_LBA DB 00h, 00h, 00h, 00h ; LBA of first absolute sector in partition (make it start from 0)
DB 40h, 0Bh, 00h, 00h ; Number of sectors in partition (2880 as in a 1.44MB floppy disk; the number is in little endian)
; Second partition entry (empty)
DB 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
; Third partition entry (empty)
DB 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
; Fourth partition entry (empty)
DB 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

; BIOS magical number
DB 55h
DB 0AAh
