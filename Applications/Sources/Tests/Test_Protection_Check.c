/** @file Test_Protection_Check.c
 * @brief Several tests to ensure that Intel memory protection is correctly enabled.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

void StackOverflow(void)
{
	static unsigned int i = 0, j = 0;
	
	i++;
	j++;
	if (j >= 100000)
	{
		LibrariesScreenWriteUnsignedInteger(i);
		LibrariesScreenWriteCharacter('\n');
		j = 0;
	}
	
	StackOverflow();
}

int main(void)
{
	volatile unsigned int *Pointer;
	volatile unsigned int Divided = 1234, Divisor = 0;
	
	LibrariesScreenWriteString("--- Protection check ---\n");
	LibrariesScreenWriteString("All tests must generate an error handled by the system to success.\n\n");
	
	LibrariesScreenWriteString("    1. Test privileged instructions\n");
	LibrariesScreenWriteString("    2. Test I/O instructions\n");
	LibrariesScreenWriteString("    3. Test stack overflow\n");
	LibrariesScreenWriteString("    4. Test out of bounds memory access\n");
	LibrariesScreenWriteString("    5. Test division by zero\n");
	LibrariesScreenWriteString("    6. Test malicious user space pointer\n");
	LibrariesScreenWriteString("    7. Test invalid system call\n");
	LibrariesScreenWriteString("    Other. Quit\n\n");

	switch (LibrariesKeyboardReadCharacter())
	{
		case '1':
			LibrariesScreenWriteString("-> Privileged instructions test.\n");
			asm("cli");
			LibrariesScreenWriteString("-> Test failed !\n");
			break;
			
		case '2':
			LibrariesScreenWriteString("-> I/O instructions test.\n");
			asm
			(
				"mov dx, 0x3F2\n"
				"in ax, dx"
				: // No output parameter
				: // No input parameter
				: "eax", "edx"
			);
			LibrariesScreenWriteString("-> Test failed !\n");
			break;
			
		case '3':
			LibrariesScreenWriteString("-> Stack overflow test.\n");
			StackOverflow();
			LibrariesScreenWriteString("-> Test failed !\n");
			break;
		
		case '4':
			LibrariesScreenWriteString("-> Out of bounds memory access (the system must have less than 4GB of RAM).\n");
			Pointer = (unsigned int *) 0xF0000000; // Go to end of memory
			*Pointer = 0x12345678;
			LibrariesScreenWriteString("Read value : ");
			LibrariesScreenWriteUnsignedInteger(*Pointer);
			LibrariesScreenWriteString("(= 305 419 896)\n");
			LibrariesScreenWriteString("-> Test failed !\n");
			break;
			
		case '5':
			LibrariesScreenWriteString("-> Division per zero test.\n");
			Divided = Divided / Divisor;
			LibrariesScreenWriteString("-> Test failed !\n");
			break;
			
		case '6':
			LibrariesScreenWriteString("-> Doing a system call with a malicious pointer as parameter.\n");
			Pointer = (unsigned int *) (0xFFFFFFFF - 0x000F0000); // The kernel will add 0x00100000 to this value, resulting in a pointer in the kernel space
			LibrariesScreenWriteString((char *) Pointer);
			LibrariesScreenWriteString("-> Test failed !\n");
			break;
			
		case '7':
			LibrariesScreenWriteString("-> Requesting an invalid system call.\n");
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
