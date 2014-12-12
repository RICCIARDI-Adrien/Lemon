/** @file Test_Protection_Check.c
 * @brief Several tests to ensure that Intel memory protection is correctly enabled.
 * @author Adrien RICCIARDI
 * @version 1.0 : 08/12/2012
 * @version 1.1 : 07/12/2014, ported to new Libraries version.
 */
#include <System.h>

void StackOverflow(void)
{
	static unsigned int i = 0, j = 0;
	
	i++;
	j++;
	if (j >= 100000)
	{
		ScreenWriteUnsignedInteger(i);
		ScreenWriteCharacter('\n');
		j = 0;
	}
	
	StackOverflow();
}

int main(void)
{
	volatile unsigned int *Pointer;
	volatile unsigned int Divided = 1234, Divisor = 0;
	
	ScreenWriteString("--- Protection check ---\n");
	ScreenWriteString("All tests must generate an error handled by the system to success.\n\n");
	
	ScreenWriteString("    1. Test privileged instructions\n");
	ScreenWriteString("    2. Test I/O instructions\n");
	ScreenWriteString("    3. Test stack overflow\n");
	ScreenWriteString("    4. Test out of bounds memory access\n");
	ScreenWriteString("    5. Test division by zero\n");
	ScreenWriteString("    6. Test malicious user space pointer\n");
	ScreenWriteString("    Other. Quit\n\n");

	switch (KeyboardReadCharacter())
	{
		case '1':
			ScreenWriteString("-> Privileged instructions test.\n");
			asm("cli");
			ScreenWriteString("-> Test failed !\n");
			break;
			
		case '2':
			ScreenWriteString("-> I/O instructions test.\n");
			asm("mov dx, 0x3F2");
			asm("in ax, dx");
			ScreenWriteString("-> Test failed !\n");
			break;
			
		case '3':
			ScreenWriteString("-> Stack overflow test.\n");
			StackOverflow();
			ScreenWriteString("-> Test failed !\n");
			break;
		
		case '4':
			ScreenWriteString("-> Out of bounds memory access (the system must have less than 4GB of RAM).\n");
			Pointer = (unsigned int *) 0xF0000000; // Go to end of memory
			*Pointer = 0x12345678;
			ScreenWriteString("Read value : ");
			ScreenWriteUnsignedInteger(*Pointer);
			ScreenWriteString("(= 305 419 896)\n");
			ScreenWriteString("-> Test failed !\n");
			break;
			
		case '5':
			ScreenWriteString("-> Division per zero test.\n");
			Divided = Divided / Divisor;
			ScreenWriteString("-> Test failed !\n");
			break;
			
		case '6':
			ScreenWriteString("-> Doing a system call with a malicious pointer as parameter.\n");
			Pointer = (unsigned int *) (0xFFFFFFFF - 0x000F0000); // The kernel will add 0x00100000 to this value, resulting in a pointer in the kernel space
			ScreenWriteString((char *) Pointer);
			ScreenWriteString("-> Test failed !\n");
			break;
			
		default:
			break;
	}
	
	return 0;
}
