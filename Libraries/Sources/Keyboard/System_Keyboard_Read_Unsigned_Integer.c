/** @file System_Keyboard_Read_Unsigned_Integer.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many characters are needed to represent the maximum allowed number (i.e. 2^32). */
#define SYSTEM_KEYBOARD_READ_UNSIGNED_INTEGER_MAXIMUM_CHARACTERS_COUNT 10

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int SystemKeyboardReadUnsignedInteger(void)
{
	unsigned char Character;
	int i = 0;
	char String_Number[SYSTEM_KEYBOARD_READ_UNSIGNED_INTEGER_MAXIMUM_CHARACTERS_COUNT + 1]; // +1 for the terminating zero
	
	// Read the number string
	while (1)
	{
		// Get a character
		Character = SystemCall(SYSTEM_CALL_KEYBOARD_READ_CHARACTER, 0, 0, NULL, NULL);
		
		// Process the character
		if (Character == '\b') // Backspace
		{
			if (i > 0)
			{
				i--;
				String_Number[i] = 0;
				SystemCall(SYSTEM_CALL_SCREEN_WRITE_CHARACTER, '\b', 0, NULL, NULL);
			}
			continue;
		}
		else if ((Character == '\n') && (i > 0)) // Enter (does not allow an empty string)
		{
			String_Number[i] = 0;
			SystemCall(SYSTEM_CALL_SCREEN_WRITE_CHARACTER, '\n', 0, NULL, NULL);
			break;
		}
		else if (!SystemStringIsCharacterADigit(Character)) continue; // Allow only digits
		
		// Append the character to the string only if there is enough room to
		if (i < SYSTEM_KEYBOARD_READ_UNSIGNED_INTEGER_MAXIMUM_CHARACTERS_COUNT)
		{
			String_Number[i] = Character;
			i++;
		
			// Display the character
			SystemCall(SYSTEM_CALL_SCREEN_WRITE_CHARACTER, Character, 0, NULL, NULL);
		}
	}
	
	// Convert the number to binary
	return SystemStringConvertStringToUnsignedInteger(String_Number);
}
