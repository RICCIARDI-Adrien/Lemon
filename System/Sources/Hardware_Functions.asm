; Some useful hardware functions.
; Author : Adrien RICCIARDI
[BITS 32]

GLOBAL outb
GLOBAL inb

outb:
	push edx
	mov edx,[esp+8]
	mov eax,[esp+12]
	out dx,al
	pop edx
	ret
	
inb:
	push edx
	mov edx,[esp+8]
	in al,dx
	pop edx
	ret
