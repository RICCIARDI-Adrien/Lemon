/** @file Network_IP_Convert_From_String.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int NetworkIPConvertFromString(char *String_IP_Address, unsigned int *Pointer_IP_Address)
{
	char String_Byte[4], Character;
	int i, Byte_String_Offset, Bytes_Count;
	unsigned int Result = 0, Byte;
	
	for (Bytes_Count = 0; Bytes_Count < 4; Bytes_Count++)
	{
		// Extract a byte value
		Byte_String_Offset = 0;
		for (i = 0; i < 3; i++)
		{
			// Parse the next character
			Character = *String_IP_Address;
			
			// The string is terminated
			if (Character == 0) break;
			// The character is a digit
			else if (LibrariesStringIsCharacterADigit(Character))
			{
				// Append the digit
				String_Byte[Byte_String_Offset] = Character;
				Byte_String_Offset++;
			}
			 // Stop if a dot is detected
			else if (Character == '.') break;
			// An unauthorized character was detected
			else return 1;
			
			String_IP_Address++;
		}
		// Append a terminating zero
		String_Byte[Byte_String_Offset] = 0;
		
		// Bypass the dot character (there is no dot at the IP end)
		if (Bytes_Count < 3)
		{
			if (*String_IP_Address != '.') return 1;
			String_IP_Address++;
		}
		
		// Is a number found ?
		if (String_Byte[0] == 0) return 1;
		
		// Convert the number to binary
		Byte = LibrariesStringConvertStringToUnsignedInteger(String_Byte);
		if (Byte > 255) return 1; // Make sure the byte value is in range [0..255]
		
		// Append the converted byte to the result in a big endian way
		Result >>= 8; // Shift all appended bytes to the right side (reverse of what would be done in little endian)
		Result |= Byte << 24; // Append the byte to the left side of the double word
	}
	
	*Pointer_IP_Address = Result;
	return 0;
}