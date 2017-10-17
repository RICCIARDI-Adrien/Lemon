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
			ScreenWriteCharacter('\b');
		}
		
		// Enter, convert and return number only if user entered almost one digit
		else if (Character == '\n')
		{
			// No empty number allowed
			if (Digits_Count == 0) continue;
			
			String[Digits_Count] = 0;
			ScreenWriteCharacter('\n');
			return (int) StringConvertStringToUnsignedInteger(String);
		}
		
		// Escape key, return exit code
		else if (Character == SYSTEM_KEYBOARD_KEY_CODE_ESCAPE) return NUMBERS_EXIT_CODE;
		
		// Other characters, accept only digits if there is enough space in string
		else if ((Character >= '0') && (Character <= '9') && (Digits_Count < sizeof(String) - 1))
		{
			String[Digits_Count] = Character;
			Digits_Count++;
			ScreenWriteCharacter(Character);
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
	ScreenWriteString(STRING_NUMBERS_INSTRUCTIONS_1);
	ScreenWriteInteger(MAXIMUM_NUMBER_VALUE);
	ScreenWriteString(STRING_NUMBERS_INSTRUCTIONS_2);
	ScreenWriteInteger(ATTEMPTS_COUNT);
	ScreenWriteString(STRING_NUMBERS_INSTRUCTIONS_3);
			
	while (1)
	{
		// Get player's number
		ScreenSetFontColor(SCREEN_COLOR_LIGHT_BLUE);
		ScreenWriteString(STRING_NUMBERS_INSERT_NUMBER);
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		Player_Number = ReadUserNumber();
				
		// Quit game ?
		if (Player_Number == NUMBERS_EXIT_CODE) return;
		
		Attempts++;
		if (Attempts >= ATTEMPTS_COUNT)
		{
			ScreenSetFontColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_NUMBERS_PLAYER_LOST_1);
			ScreenWriteInteger(Computer_Number);
			ScreenWriteString(STRING_NUMBERS_PLAYER_LOST_2);
			goto End;
		}
		
		// Compare to computer number
		if (Player_Number == Computer_Number) // Player won
		{
			ScreenSetFontColor(SCREEN_COLOR_GREEN);
			ScreenWriteString(STRING_NUMBERS_PLAYER_WON_1);
			ScreenWriteInteger(Attempts);
			ScreenWriteString(STRING_NUMBERS_PLAYER_WON_2);
			goto End;
		}
		else if (Player_Number < Computer_Number) // Too small
		{
			ScreenSetFontColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_NUMBERS_NUMBER_TOO_SMALL);
		}
		else // Too big
		{
			ScreenSetFontColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_NUMBERS_NUMBER_TOO_BIG);
		}

		// Show remaining attempts
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		ScreenWriteString(STRING_NUMBERS_REMAINING_ATTEMPTS_1);
		ScreenWriteInteger(ATTEMPTS_COUNT - Attempts);
		ScreenWriteString(STRING_NUMBERS_REMAINING_ATTEMPTS_2);
	}
	
End:
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
	ScreenWriteString(STRING_NUMBERS_END);
	SystemKeyboardReadCharacter();
}