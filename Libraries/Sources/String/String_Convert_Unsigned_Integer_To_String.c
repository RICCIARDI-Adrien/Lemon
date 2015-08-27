/** @file String_Convert_Unsigned_Integer_To_String.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void StringConvertUnsignedIntegerToString(unsigned int Number, char *String_Destination)
{
	unsigned int Number_Divided_By_Ten;
	int i = 10, j = 0; // Go to the temporary string last number, +1 as the variable is decremented first (see below)
	char String_Temp[10]; // Enough to hold a positive 32-bit number, no need for a terminating zero here
	
	// Handle zero as a special case to ease the processing
	if (Number == 0)
	{
		String_Destination[0] = '0';
		String_Destination[1] = 0;
		return;
	}
	
	// Extract each digit from the number
	while (Number > 0)
	{
		i--; // Decrement first so 'i' will point on the last string's converted digit when exiting the loop
		Number_Divided_By_Ten = Number / 10;
		String_Temp[i] = Number - (Number_Divided_By_Ten * 10); // Extract the last digit
		String_Temp[i] += 48; // Convert the digit to an ASCII character
		Number = Number_Divided_By_Ten; // Go to next digit
	}
	
	// Copy converted string to the destination string
	while (i < 10) // Loop while i has not reached the end of the converted number
	{
		String_Destination[j] = String_Temp[i];
		i++;
		j++;
	}
	// Append the terminating zero
	String_Destination[j] = 0;
}