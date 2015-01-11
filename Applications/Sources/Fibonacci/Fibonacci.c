/** @file Fibonacci.c
 * Compute the Fibonacci suite up to the requested numbers of iterations.
 * @author Adrien RICCIARDI
 * @version 1.0 : 03/01/2015
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many iterations to compute. */
#define ITERATIONS_COUNT 10000000

/** Convert the macro identifier to a C string. */
#define CONVERT_MACRO_NAME_TO_STRING(X) #X
/** Convert the macro value to a C string. The preprocessor needs two passes to do the conversion, so the CONVERT_MACRO_NAME_TO_STRING() is needed. */
#define CONVERT_MACRO_VALUE_TO_STRING(X) CONVERT_MACRO_NAME_TO_STRING(X)

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	static TMathInteger Penultimate_Number, Last_Number, Result; // Do not store these variables on the stack as they are a bit heavy
	unsigned long long Iterations;
	unsigned int Start_Time, End_Time;
	
	ScreenWriteString("Starting computation.\n");
	ScreenWriteString("Iterations count : ");
	ScreenWriteString(CONVERT_MACRO_VALUE_TO_STRING(ITERATIONS_COUNT));
	ScreenWriteString("\nInteger precision : 32768 bits\n");
	Start_Time = SystemGetTimerValue();
	
	// Initialize the first numbers
	MathIntegerInitializeFromInt(0, &Penultimate_Number);
	MathIntegerInitializeFromInt(1, &Last_Number);
	
	for (Iterations = 0; Iterations < ITERATIONS_COUNT; Iterations++)
	{
		MathIntegerAdd(&Penultimate_Number, &Last_Number, &Result);
		MathIntegerCopy(&Last_Number, &Penultimate_Number);
		MathIntegerCopy(&Result, &Last_Number);
	}
	
	End_Time = SystemGetTimerValue();
	
	ScreenWriteString("Result :\n");
	MathIntegerDisplayHexadecimal(&Result);
	ScreenWriteCharacter('\n');
	
	ScreenWriteString("Computation duration (+/- 1 second) : ");
	ScreenSetFontColor(SCREEN_COLOR_GREEN);
	ScreenWriteUnsignedInteger((End_Time - Start_Time) / 20); // Convert to seconds
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
	ScreenWriteString(" seconds.\n");
	
	return 0;
}