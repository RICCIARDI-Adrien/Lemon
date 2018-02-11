; Ask the operating system for a service.
; Author : Adrien RICCIARDI
GLOBAL SystemCall

SystemCall:
	push ebx
	push ecx
	push edx
	push esi
	mov eax, [esp + 20]
	mov ebx, [esp + 24]
	mov ecx, [esp + 28]
	mov edx, [esp + 32]
	mov esi, [esp + 36]
	int 0x60
	pop esi
	pop edx
	pop ecx
	pop ebx
	ret
