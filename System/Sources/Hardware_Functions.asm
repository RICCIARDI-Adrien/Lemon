; Some useful hardware functions.
; Author : Adrien RICCIARDI
; V 1.0 : 24/07/2011 
; V 1.1 : 16/11/2011
; V 1.2 : 01/08/2012
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
