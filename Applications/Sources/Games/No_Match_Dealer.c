/** @file No_Match_Dealer.c
 * @see Games.h for description.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
#include "Games.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many numbers to pick. */
#define NUMBERS_COUNT 16

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void NoMatchDealer(void)
{
	int i, Numbers[NUMBERS_COUNT], j;
	char Character;
	
	// Show instructions
	LibrariesScreenWriteString(STRING_NO_MATCH_DEALER_INSTRUCTIONS);
	
	while (1)
	{
		// Generate numbers
		LibrariesRandomInitialize();
		for (i = 0; i < NUMBERS_COUNT; i++) Numbers[i] = LibrariesRandomGenerateNumber() % 100;

		// Show numbers on 2 rows
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_LIGHT_BLUE);
		for (i = 0; i < NUMBERS_COUNT; i++)
		{
			// Pad number with a space character to keep alignment when drawing to screen
			if (Numbers[i] < 10) LibrariesScreenWriteCharacter(' ');
			LibrariesScreenWriteInteger(Numbers[i]);
			LibrariesScreenWriteCharacter(' ');
			
			// Go to next line if needed
			if (i % (NUMBERS_COUNT / 2) == 7) LibrariesScreenWriteCharacter('\n');
		}
		LibrariesScreenWriteCharacter('\n');
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);

		// Check for matches
		for (i = 0; i < NUMBERS_COUNT; i++)
		{
			for (j = 0; j < NUMBERS_COUNT; j++)
			{
				if ((i != j) && (Numbers[i] == Numbers[j]))
				{
					LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
					LibrariesScreenWriteString(STRING_NO_MATCH_DEALER_GAME_LOST);
					goto End;
				}
			}
		}

		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
		LibrariesScreenWriteString(STRING_NO_MATCH_DEALER_GAME_WON);

	End:
		// Ask the player to retry or not
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
		LibrariesScreenWriteString(STRING_NO_MATCH_DEALER_ASK_FOR_RETRY);
		
		// Wait for Enter or Escape
		do
		{
			Character = LibrariesKeyboardReadCharacter();
		} while ((Character != '\n') && (Character != LIBRARIES_KEYBOARD_KEY_CODE_ESCAPE));
		if (Character == LIBRARIES_KEYBOARD_KEY_CODE_ESCAPE) return;
	}
}
