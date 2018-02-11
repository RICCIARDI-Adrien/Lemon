/** @file Numbers.c
 * @see Games.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Games.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** This value can't be issued by the user input function. */
#define NUMBERS_EXIT_CODE -1

/** The number to find is in range [1 ; MAXIMUM_NUMBER_VALUE]. */ 
#define MAXIMUM_NUMBER_VALUE 100000

/** How many attempts are allowed. */
#define ATTEMPTS_COUNT 20

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Read user number. Allow only numbers to be typed.
 * @return The number or NUMBERS_EXIT_CODE if the user hit escape key.
 */
static int ReadUserNumber(void)
{
	char String[11], Character;
	unsigned int Digits_Count = 0; // Must be unsigned to avoid comparison with sizeof() sign warning
	
	while (1)
	{
		Character = SystemKeyboardReadCharacter();
		
		// Backspace, delete last digit if possible
		if (Character == '\b')
		{
			if (Digits_Count <= 0) continue;
			Digits_Count--;
			String[Digits_Count] = 0;
			SystemScreenWriteCharacter('\b');
		}
		
		// Enter, convert and return number only if user entered almost one digit
		else if (Character == '\n')
		{
			// No empty number allowed
			if (Digits_Count == 0) continue;
			
			String[Digits_Count] = 0;
			SystemScreenWriteCharacter('\n');
			return (int) SystemStringConvertStringToUnsignedInteger(String);
		}
		
		// Escape key, return exit code
		else if (Character == LIBRARIES_KEYBOARD_KEY_CODE_ESCAPE) return NUMBERS_EXIT_CODE;
		
		// Other characters, accept only digits if there is enough space in string
		else if ((Character >= '0') && (Character <= '9') && (Digits_Count < sizeof(String) - 1))
		{
			String[Digits_Count] = Character;
			Digits_Count++;
			SystemScreenWriteCharacter(Character);
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void Numbers(void)
{
	int Computer_Number, Player_Number, Attempts = 0;
	
	// Choose number
	SystemRandomInitialize();
	Computer_Number = SystemRandomGenerateNumber() % MAXIMUM_NUMBER_VALUE + 1;
	
	// Show instructions
	SystemScreenWriteString(STRING_NUMBERS_INSTRUCTIONS_1);
	SystemScreenWriteInteger(MAXIMUM_NUMBER_VALUE);
	SystemScreenWriteString(STRING_NUMBERS_INSTRUCTIONS_2);
	SystemScreenWriteInteger(ATTEMPTS_COUNT);
	SystemScreenWriteString(STRING_NUMBERS_INSTRUCTIONS_3);
			
	while (1)
	{
		// Get player's number
		SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_LIGHT_BLUE);
		SystemScreenWriteString(STRING_NUMBERS_INSERT_NUMBER);
		SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
		Player_Number = ReadUserNumber();
				
		// Quit game ?
		if (Player_Number == NUMBERS_EXIT_CODE) return;
		
		Attempts++;
		if (Attempts >= ATTEMPTS_COUNT)
		{
			SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
			SystemScreenWriteString(STRING_NUMBERS_PLAYER_LOST_1);
			SystemScreenWriteInteger(Computer_Number);
			SystemScreenWriteString(STRING_NUMBERS_PLAYER_LOST_2);
			goto End;
		}
		
		// Compare to computer number
		if (Player_Number == Computer_Number) // Player won
		{
			SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
			SystemScreenWriteString(STRING_NUMBERS_PLAYER_WON_1);
			SystemScreenWriteInteger(Attempts);
			SystemScreenWriteString(STRING_NUMBERS_PLAYER_WON_2);
			goto End;
		}
		else if (Player_Number < Computer_Number) // Too small
		{
			SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
			SystemScreenWriteString(STRING_NUMBERS_NUMBER_TOO_SMALL);
		}
		else // Too big
		{
			SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
			SystemScreenWriteString(STRING_NUMBERS_NUMBER_TOO_BIG);
		}

		// Show remaining attempts
		SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
		SystemScreenWriteString(STRING_NUMBERS_REMAINING_ATTEMPTS_1);
		SystemScreenWriteInteger(ATTEMPTS_COUNT - Attempts);
		SystemScreenWriteString(STRING_NUMBERS_REMAINING_ATTEMPTS_2);
	}
	
End:
	SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
	SystemScreenWriteString(STRING_NUMBERS_END);
	SystemKeyboardReadCharacter();
}