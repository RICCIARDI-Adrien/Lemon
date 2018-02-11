/** @file Libraries_String_Convert_Integer_To_Hexadecimal.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesStringConvertIntegerToHexadecimal(int Integer, char *String_Hexadecimal_Output)
{
	unsigned int i;
	char Nibble;
	
	// Extract all 4-bit nibbles
	for (i = 0; i < sizeof(int) * 2; i++)
	{
		// Get the most significant nibble (to write to the string starting by 0)
		Nibble = (Integer & 0xF0000000) >> 28;
		
		// Convert it to ASCII
		if (Nibble < 10) Nibble += '0';
		else Nibble = Nibble - 10 + 'A';
		String_Hexadecimal_Output[i] = Nibble;
		
		// Go to next nibble
		Integer <<= 4;
	}
	
	// Append terminating zero
	String_Hexadecimal_Output[i] = 0;
}
