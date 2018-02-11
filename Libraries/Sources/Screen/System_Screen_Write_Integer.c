/** @file System_Screen_Write_Integer.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemScreenWriteInteger(int Integer)
{
	char String_Number[12]; // The minus sign + 10 digits (the maximum digits count for a 32-bit number) + terminating zero
	int Temp, Display_Index = 0, Is_Number_Negative = 0;
	
	// Avoid conversion if the number is zero
	if (Integer == 0)
	{
		String_Number[0] = '0';
		String_Number[1] = 0;
	}
	// Handle 32-bit minimum negative value as it can't be represented on a 32-bit unsigned int
	else if (Integer == -2147483647 - 1) SystemStringCopy("-2147483648", String_Number); // Can't directly use the -2147483648 constant or the compiler will moan
	else
	{
		// Store number sign for later use
		if (Integer < 0)
		{
			Is_Number_Negative = 1;
			Integer = -Integer; // Cast the number to unsigned
		}
		
		// Convert number
		Display_Index = sizeof(String_Number) - 1; // Start from the end of the string as the following loop will reverse the digits
		while (Integer > 0)
		{
			Display_Index--; // Decrement first to make the loop stops on an existing character
			Temp = Integer / 10; // Integer division
			String_Number[Display_Index] = (Integer - (Temp * 10)) + 48; // Convert last character to ASCII
			Integer = Temp;
		}
		
		// Add the negative sign if needed
		if (Is_Number_Negative)
		{
			Display_Index--;
			String_Number[Display_Index] = '-';
		}
		
		// Set terminating zero
		String_Number[sizeof(String_Number) - 1] = 0;
	}
	
	// Display the string
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_WRITE_STRING, 0, 0, &String_Number[Display_Index], NULL);
}
