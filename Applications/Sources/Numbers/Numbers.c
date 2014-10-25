/** @file Numbers.c
 * @see Numbers.h for description.
 * @author Adrien RICCIARDI
 */
#include <stdio.h>
#include <stdlib.h>
#include <System.h>
#include <time.h>
#include "Numbers.h"

//-------------------------------------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------------------------------------
/** This value can't be issued by the user input function. */
#define NUMBERS_EXIT_CODE -1

//-------------------------------------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Read user number. Allow only numbers to be typed.
 * @return The number or NUMBERS_EXIT_CODE if the user hit escape key.
 */
static int ReadUserNumber(void)
{
	char String[11], Character;
	unsigned int Digits_Count = 0; // Must be unsigned to avoid comparison with sizeof() sign warning
	
	while (1)
	{
		Character = KeyboardReadChar();
		
		// Backspace, delete last digit if possible
		if (Character == '\b')
		{
			if (Digits_Count <= 0) continue;
			Digits_Count--;
			String[Digits_Count] = 0;
			putchar('\b');
		}
		
		// Enter, convert and return number only if user entered almost one digit
		else if (Character == '\n')
		{
			// No empty number allowed
			if (Digits_Count == 0) continue;
			
			String[Digits_Count] = 0;
			putchar('\n');
			return atoi(String);
		}
		
		// Escape key, return exit code
		else if (Character == KEYBOARD_KEY_CODE_ESCAPE) return NUMBERS_EXIT_CODE;
		
		// Other characters, accept only digits if there is enough space in string
		else if ((Character >= '0') && (Character <= '9') && (Digits_Count < sizeof(String) - 1))
		{
			String[Digits_Count] = Character;
			Digits_Count++;
			putchar(Character);
			fflush(stdout);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
int main(void)
{
	int Computer_Number, Player_Number, Attempts = 0;
		
	// Choose number
	srand(time(NULL));
	Computer_Number = rand() % MAXIMUM_NUMBER_VALUE + 1;
	
	// Show instructions
	printf(STRING_INSTRUCTIONS, MAXIMUM_NUMBER_VALUE, ATTEMPTS_COUNT);
			
	while (Attempts < ATTEMPTS_COUNT)
	{
		// Get player's number
		ScreenSetFontColor(SCREEN_COLOR_LIGHT_BLUE);
		printf(STRING_INSERT_NUMBER);
		fflush(stdout);
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		Player_Number = ReadUserNumber();
				
		// Quit game ?
		if (Player_Number == NUMBERS_EXIT_CODE)
		{
			putchar('\n');
			return EXIT_SUCCESS;
		}
		
		Attempts++;
		if (Attempts >= ATTEMPTS_COUNT)
		{
			ScreenSetFontColor(SCREEN_COLOR_RED);
			printf(STRING_PLAYER_LOST, Computer_Number);
			return EXIT_SUCCESS;
		}
		
		// Compare to computer number
		if (Player_Number == Computer_Number) // Player won
		{
			ScreenSetFontColor(SCREEN_COLOR_GREEN);
			printf(STRING_PLAYER_WON, Attempts);
			return EXIT_SUCCESS;
		}
		else if (Player_Number < Computer_Number) // Too small
		{
			ScreenSetFontColor(SCREEN_COLOR_RED);
			printf(STRING_NUMBER_TOO_SMALL);
		}
		else // Too big
		{
			ScreenSetFontColor(SCREEN_COLOR_RED);
			printf(STRING_NUMBER_TOO_BIG);
		}

		// Show remaining attempts
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		printf(STRING_REMAINING_ATTEMPTS, ATTEMPTS_COUNT - Attempts);
	}		
	return EXIT_SUCCESS;
}