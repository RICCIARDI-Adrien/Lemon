; Some useful hardware functions.
; Author : Adrien RICCIARDI
[BITS 32]

GLOBAL outb
GLOBAL outw
GLOBAL outd
GLOBAL inb
GLOBAL inw
GLOBAL ind

outb:
	push edx
	mov edx, [esp + 8]
	mov eax, [esp + 12]
	out dx, al
	pop edx
	ret
	
outw:
	push edx
	mov edx, [esp + 8]
	mov eax, [esp + 12]
	out dx, ax
	pop edx
	ret
	
outd:
	push edx
	mov edx, [esp + 8]
	mov eax, [esp + 12]
	out dx, eax
	pop edx
	ret
	
inb:
	push edx
	mov edx, [esp + 8]
	in al, dx
	pop edx
	ret
	
inw:
	push edx
	mov edx, [esp + 8]
	in ax, dx
	pop edx
	ret
	
ind:
	push edx
	mov edx, [esp + 8]
	in eax, dx
	pop edx
	ret
