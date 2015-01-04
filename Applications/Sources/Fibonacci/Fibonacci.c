/** @file Fibonacci.c
 * Compute the Fibonacci suite up to the requested numbers of iterations.
 * @author Adrien RICCIARDI
 * @version 1.0 : 03/01/2015
 */
#include <System.h>

#define ITERATIONS_COUNT 1000000000

int main(void)
{
	TMathInteger Penultimate_Number, Last_Number, Result;
	unsigned long long Iterations;
	
	ScreenWriteString("Starting computation.\n");
	
	// Initialize the first numbers
	MathIntegerInitializeFromInt(0, &Penultimate_Number);
	MathIntegerInitializeFromInt(1, &Last_Number);
	
	for (Iterations = 0; Iterations < ITERATIONS_COUNT; Iterations++)
	{
		MathIntegerAdd(&Penultimate_Number, &Last_Number, &Result);
		MathIntegerCopy(&Last_Number, &Penultimate_Number);
		MathIntegerCopy(&Result, &Last_Number);
	}
	
	ScreenWriteString("Result :\n");
	MathIntegerDisplayHexadecimal(&Result);
	ScreenWriteCharacter('\n');
	
	return 0;
}