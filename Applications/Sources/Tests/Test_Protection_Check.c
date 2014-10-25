/** @file Test_Protection_Check.c
 * @brief Several tests to ensure that Intel memory protection is correctly enabled.
 * @author Adrien RICCIARDI
 * @version 1.0 : 08/12/2012
 */
#include <stdio.h>
#include <System.h>

void StackOverflow(void)
{
	static unsigned int i = 0, j = 0;
	
	i++;
	j++;
	if (j >= 100000)
	{
		printf("%u\n", i);
		j = 0;
	}
	
	StackOverflow();
}

int main(void)
{
	unsigned int *Pointer;
	volatile unsigned int Divided = 1234, Divisor = 0;
	
	puts("--- Protection check ---");
	puts("All tests must generate an error handled by the system to success.\n");
	
	puts("    1. Test privileged instructions");
	puts("    2. Test I/O instructions");
	puts("    3. Test stack overflow");
	puts("    4. Test out of bounds memory access");
	puts("    5. Test division by zero");
	puts("    Other. Quit\n");

	switch (getchar())
	{
		case '1':
			puts("-> Privileged instructions test.");
			asm("cli");
			puts("-> Test failed !");
			break;
			
		case '2':
			puts("-> I/O instructions test.");
			asm("mov dx, 0x3F2");
			asm("in ax, dx");
			puts("-> Test failed !");
			break;
			
		case '3':
			puts("-> Stack overflow test.");
			StackOverflow();
			puts("-> Test failed !");
			break;
		
		case '4':
			puts("-> Out of bounds memory access (the system must have less than 4GB of RAM).");
			Pointer = (unsigned int *) 0xF0000000; // Go to end of memory
			*Pointer = 0x12345678;
			printf("Read value : %u (= 305 419 896)\n", *Pointer);
			puts("-> Test failed !");
			break;
			
		case '5':
			puts("-> Division per zero test.");
			Divided = Divided / Divisor;
			puts("-> Test failed !");
			break;
	}
	
	return 0;
}
	
