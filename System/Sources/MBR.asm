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
; V 1.8 : 31/08/2015, added the ability to choose between LBA-28 and LBA-48.
; V 1.9 : 06/11/2015, added automatic LBA addressing mode selection (LBA28 or LBA48).
; V 1.10 : 02/04/2016, used BIOS INT13 extension to load the kernel from LBA hard disks in order to be compatible with SATA drivers. Really old BIOSes won't work with this extension.
[BITS 16]
[ORG 0]

;--------------------------------------------------------------------------------------------------
; Constants and macros
;--------------------------------------------------------------------------------------------------
;%define DEBUG ; Uncomment to enable debug output
MBR_LOAD_SEGMENT EQU 07C0h
KERNEL_LOAD_SEGMENT EQU 1000h
KERNEL_PROTECTED_MODE_ADDRESS EQU (KERNEL_LOAD_SEGMENT * 16)

;--------------------------------------------------------------------------------------------------
; Entry point
;--------------------------------------------------------------------------------------------------
Entry_Point:
	; Initialize stack and data segment
	cli
	mov ax, MBR_LOAD_SEGMENT
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
	%ifdef CONFIGURATION_BUILD_INSTALLER
		call LoadKernelFromInstallationMedia
	; Load the System kernel from the hard disk
	%else
		; Get the starting LBA sector of the first partition
		mov eax, DWORD [First_Partition_Starting_Sector_LBA]
		inc eax ; Bypass the first sector which is the MBR
		mov DWORD [Disk_Address_Packet_Starting_Sector_LBA_Low], eax
		
		; Display the first sector to load
		%ifdef DEBUG
			xor di, di
			mov ax, [Disk_Address_Packet_Starting_Sector_LBA_Low + 2]
			call WriteHex
			mov ax, [Disk_Address_Packet_Starting_Sector_LBA_Low]
			call WriteHex
		%endif
		
		call LoadKernelFromHardDisk
	%endif
	
	; Wait for a key press in order to display the debug messages
	%ifdef DEBUG
		mov ax, 10h
		int 16h
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

	; Initialize segment selectors to point to the kernel data segment
	mov ax, 16 ; Offset of data segment descriptor in GDT
	mov ds, ax
	mov es, ax ; Initialize extra segment register too in case an Intel string instruction would be used
	mov ss, ax
	
	; Far jump to the kernel entry point.
	jmp DWORD 8:KERNEL_PROTECTED_MODE_ADDRESS ; '8' is to select the kernel code segment

;--------------------------------------------------------------------------------------------------
; Functions
;--------------------------------------------------------------------------------------------------
%ifdef CONFIGURATION_BUILD_INSTALLER
	; Use the BIOS to access to the storage disk
	LoadKernelFromInstallationMedia:
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
		%endif

		; Get sectors per track count
		and cl, 3Fh
		movzx ax, cl
		mov [Sectors_Per_Track_Count], ax

		%ifdef DEBUG
			mov di, 160
			call WriteHex
		%endif

		; Load kernel
		mov ax, KERNEL_LOAD_SEGMENT
		mov es, ax
		xor bx, bx
		mov cx, SECTORS_TO_LOAD_COUNT

		; Loop for loading kernel sectors
	.Load_Kernel_From_Installation_Media_Loop:
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
		jnz .Load_Kernel_From_Installation_Media_Loop_Continue_Loading
		mov ax, es
		add ax, 1000h ; 64KB / 16
		mov es, ax

	.Load_Kernel_From_Installation_Media_Loop_Continue_Loading:
		pop cx
		loop .Load_Kernel_From_Installation_Media_Loop

		ret
%else
	LoadKernelFromHardDisk:
		mov cx, SECTORS_TO_LOAD_COUNT
		
		; Load the kernel to the specified address
		mov eax, KERNEL_LOAD_SEGMENT << 16 ; Intel is little-endian, so invert segment:offset order (offset value is zero)
		mov [Disk_Address_Packet_Destination_Buffer], eax
	
	.Load_Kernel_From_Hard_Disk_Loop:
		; Load a sector
		mov ah, 42h
		mov dl, [Boot_Device]
		mov si, Disk_Address_Packet
		int 13h
		
		; Prepare for the next sector read
		add DWORD [Disk_Address_Packet_Destination_Buffer], 512 ; Increment the buffer address
		inc DWORD [Disk_Address_Packet_Starting_Sector_LBA_Low] ; Increment only the lower part of the LBA address for now, TODO use whole 64 bits
		
		loop .Load_Kernel_From_Hard_Disk_Loop
	
		ret
%endif

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
             DD Global_Descriptor_Table + (MBR_LOAD_SEGMENT * 16) ; Convert the load segment to protected mode address

Boot_Device DB 0

%ifdef CONFIGURATION_BUILD_INSTALLER
	Logical_Sector_Number DD 1
	Sectors_Per_Track_Count DW 18 ; Like a 1.44MB floppy disk
	Heads_Count DW 2 ; Like a 1.44MB floppy disk
%else
	; Extended Read Sector command Disk Address Packet (only changeable parameters are named)
	Disk_Address_Packet:
	DB 16 ; Packet size, 16-byte long, do not use the 64-bit buffer address extension
	DB 0 ; Unused
	DW 1 ; Sectors to read count, always read 1 sector at a time to make it simpler
	Disk_Address_Packet_Destination_Buffer DD 0
	Disk_Address_Packet_Starting_Sector_LBA_Low DD 0 ; Intel is little-endian, so low first
	Disk_Address_Packet_Starting_Sector_LBA_High DD 0
%endif

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
