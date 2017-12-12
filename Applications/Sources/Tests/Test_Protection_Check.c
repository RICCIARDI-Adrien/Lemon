/** @file Test_Protection_Check.c
 * @brief Several tests to ensure that Intel memory protection is correctly enabled.
 * @author Adrien RICCIARDI
 */
#include <System.h>

void StackOverflow(void)
{
	static unsigned int i = 0, j = 0;
	
	i++;
	j++;
	if (j >= 100000)
	{
		SystemScreenWriteUnsignedInteger(i);
		SystemScreenWriteCharacter('\n');
		j = 0;
	}
	
	StackOverflow();
}

int main(void)
{
	volatile unsigned int *Pointer;
	volatile unsigned int Divided = 1234, Divisor = 0;
	
	SystemScreenWriteString("--- Protection check ---\n");
	SystemScreenWriteString("All tests must generate an error handled by the system to success.\n\n");
	
	SystemScreenWriteString("    1. Test privileged instructions\n");
	SystemScreenWriteString("    2. Test I/O instructions\n");
	SystemScreenWriteString("    3. Test stack overflow\n");
	SystemScreenWriteString("    4. Test out of bounds memory access\n");
	SystemScreenWriteString("    5. Test division by zero\n");
	SystemScreenWriteString("    6. Test malicious user space pointer\n");
	SystemScreenWriteString("    7. Test invalid system call\n");
	SystemScreenWriteString("    Other. Quit\n\n");

	switch (SystemKeyboardReadCharacter())
	{
		case '1':
			SystemScreenWriteString("-> Privileged instructions test.\n");
			asm("cli");
			SystemScreenWriteString("-> Test failed !\n");
			break;
			
		case '2':
			SystemScreenWriteString("-> I/O instructions test.\n");
			asm
			(
				"mov dx, 0x3F2\n"
				"in ax, dx"
				: // No output parameter
				: // No input parameter
				: "eax", "edx"
			);
			SystemScreenWriteString("-> Test failed !\n");
			break;
			
		case '3':
			SystemScreenWriteString("-> Stack overflow test.\n");
			StackOverflow();
			SystemScreenWriteString("-> Test failed !\n");
			break;
		
		case '4':
			SystemScreenWriteString("-> Out of bounds memory access (the system must have less than 4GB of RAM).\n");
			Pointer = (unsigned int *) 0xF0000000; // Go to end of memory
			*Pointer = 0x12345678;
			SystemScreenWriteString("Read value : ");
			SystemScreenWriteUnsignedInteger(*Pointer);
			SystemScreenWriteString("(= 305 419 896)\n");
			SystemScreenWriteString("-> Test failed !\n");
			break;
			
		case '5':
			SystemScreenWriteString("-> Division per zero test.\n");
			Divided = Divided / Divisor;
			SystemScreenWriteString("-> Test failed !\n");
			break;
			
		case '6':
			SystemScreenWriteString("-> Doing a system call with a malicious pointer as parameter.\n");
			Pointer = (unsigned int *) (0xFFFFFFFF - 0x000F0000); // The kernel will add 0x00100000 to this value, resulting in a pointer in the kernel space
			SystemScreenWriteString((char *) Pointer);
			SystemScreenWriteString("-> Test failed !\n");
			break;
			
		case '7':
			SystemScreenWriteString("-> Requesting an invalid system call.\n");
			asm
			(
				"mov eax, 0xFFFFFFFF\n"
				"int 0x60"
				: // No output parameter
				: // No input parameter
				: "eax"
			);
			break;
			
		default:
			break;
	}
	
	return 0;
}
