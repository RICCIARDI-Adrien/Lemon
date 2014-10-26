/** @file Screen_Write_Unsigned_Integer.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 26/10/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenWriteUnsignedInteger(unsigned int Unsigned_Integer)
{
	char String_Number[11]; // 10 digits (the maximum digits count for a 32-bit number) + terminating zero
	int Temp, Display_Index = 0;
	
	// Avoid conversion if the number is zero
	if (Unsigned_Integer == 0)
	{
		String_Number[0] = '0';
		String_Number[1] = 0;
	}
	else
	{
		// Convert number
		Display_Index = sizeof(String_Number) - 1; // Start from the end of the string as the following loop will reverse the digits
		while (Unsigned_Integer > 0)
		{
			Display_Index--; // Decrement first to make the loop stops on an existing character
			Temp = Unsigned_Integer / 10; // Integer division
			String_Number[Display_Index] = (Unsigned_Integer - (Temp * 10)) + 48; // Convert last character to ASCII
			Unsigned_Integer = Temp;
		}
		// Set terminating zero
		String_Number[sizeof(String_Number) - 1] = 0;
	}
	
	// Display the string
	SystemCall(SYSTEM_CALL_SCREEN_WRITE_STRING, 0, 0, &String_Number[Display_Index], NULL);
}