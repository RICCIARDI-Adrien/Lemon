/** @file Numbers.c
 * @see Games.h for description.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
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
		Character = LibrariesKeyboardReadCharacter();
		
		// Backspace, delete last digit if possible
		if (Character == '\b')
		{
			if (Digits_Count <= 0) continue;
			Digits_Count--;
			String[Digits_Count] = 0;
			LibrariesScreenWriteCharacter('\b');
		}
		
		// Enter, convert and return number only if user entered almost one digit
		else if (Character == '\n')
		{
			// No empty number allowed
			if (Digits_Count == 0) continue;
			
			String[Digits_Count] = 0;
			LibrariesScreenWriteCharacter('\n');
			return (int) LibrariesStringConvertStringToUnsignedInteger(String);
		}
		
		// Escape key, return exit code
		else if (Character == LIBRARIES_KEYBOARD_KEY_CODE_ESCAPE) return NUMBERS_EXIT_CODE;
		
		// Other characters, accept only digits if there is enough space in string
		else if ((Character >= '0') && (Character <= '9') && (Digits_Count < sizeof(String) - 1))
		{
			String[Digits_Count] = Character;
			Digits_Count++;
			LibrariesScreenWriteCharacter(Character);
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
	LibrariesRandomInitialize();
	Computer_Number = LibrariesRandomGenerateNumber() % MAXIMUM_NUMBER_VALUE + 1;
	
	// Show instructions
	LibrariesScreenWriteString(STRING_NUMBERS_INSTRUCTIONS_1);
	LibrariesScreenWriteInteger(MAXIMUM_NUMBER_VALUE);
	LibrariesScreenWriteString(STRING_NUMBERS_INSTRUCTIONS_2);
	LibrariesScreenWriteInteger(ATTEMPTS_COUNT);
	LibrariesScreenWriteString(STRING_NUMBERS_INSTRUCTIONS_3);
			
	while (1)
	{
		// Get player's number
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_LIGHT_BLUE);
		LibrariesScreenWriteString(STRING_NUMBERS_INSERT_NUMBER);
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
		Player_Number = ReadUserNumber();
				
		// Quit game ?
		if (Player_Number == NUMBERS_EXIT_CODE) return;
		
		Attempts++;
		if (Attempts >= ATTEMPTS_COUNT)
		{
			LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
			LibrariesScreenWriteString(STRING_NUMBERS_PLAYER_LOST_1);
			LibrariesScreenWriteInteger(Computer_Number);
			LibrariesScreenWriteString(STRING_NUMBERS_PLAYER_LOST_2);
			goto End;
		}
		
		// Compare to computer number
		if (Player_Number == Computer_Number) // Player won
		{
			LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
			LibrariesScreenWriteString(STRING_NUMBERS_PLAYER_WON_1);
			LibrariesScreenWriteInteger(Attempts);
			LibrariesScreenWriteString(STRING_NUMBERS_PLAYER_WON_2);
			goto End;
		}
		else if (Player_Number < Computer_Number) // Too small
		{
			LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
			LibrariesScreenWriteString(STRING_NUMBERS_NUMBER_TOO_SMALL);
		}
		else // Too big
		{
			LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
			LibrariesScreenWriteString(STRING_NUMBERS_NUMBER_TOO_BIG);
		}

		// Show remaining attempts
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
		LibrariesScreenWriteString(STRING_NUMBERS_REMAINING_ATTEMPTS_1);
		LibrariesScreenWriteInteger(ATTEMPTS_COUNT - Attempts);
		LibrariesScreenWriteString(STRING_NUMBERS_REMAINING_ATTEMPTS_2);
	}
	
End:
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
	LibrariesScreenWriteString(STRING_NUMBERS_END);
	LibrariesKeyboardReadCharacter();
}