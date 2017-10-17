/** @file No_Match_Dealer.c
 * @see Games.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
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
	ScreenWriteString(STRING_NO_MATCH_DEALER_INSTRUCTIONS);
	
	while (1)
	{
		// Generate numbers
		SystemRandomInitialize();
		for (i = 0; i < NUMBERS_COUNT; i++) Numbers[i] = SystemRandomGenerateNumber() % 100;

		// Show numbers on 2 rows
		ScreenSetFontColor(SCREEN_COLOR_LIGHT_BLUE);
		for (i = 0; i < NUMBERS_COUNT; i++)
		{
			// Pad number with a space character to keep alignment when drawing to screen
			if (Numbers[i] < 10) ScreenWriteCharacter(' ');
			ScreenWriteInteger(Numbers[i]);
			ScreenWriteCharacter(' ');
			
			// Go to next line if needed
			if (i % (NUMBERS_COUNT / 2) == 7) ScreenWriteCharacter('\n');
		}
		ScreenWriteCharacter('\n');
		ScreenSetFontColor(SCREEN_COLOR_BLUE);

		// Check for matches
		for (i = 0; i < NUMBERS_COUNT; i++)
		{
			for (j = 0; j < NUMBERS_COUNT; j++)
			{
				if ((i != j) && (Numbers[i] == Numbers[j]))
				{
					ScreenSetFontColor(SCREEN_COLOR_RED);
					ScreenWriteString(STRING_NO_MATCH_DEALER_GAME_LOST);
					goto End;
				}
			}
		}

		ScreenSetFontColor(SCREEN_COLOR_GREEN);
		ScreenWriteString(STRING_NO_MATCH_DEALER_GAME_WON);

	End:
		// Ask the player to retry or not
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		ScreenWriteString(STRING_NO_MATCH_DEALER_ASK_FOR_RETRY);
		
		// Wait for Enter or Escape
		do
		{
			Character = SystemKeyboardReadCharacter();
		} while ((Character != '\n') && (Character != SYSTEM_KEYBOARD_KEY_CODE_ESCAPE));
		if (Character == SYSTEM_KEYBOARD_KEY_CODE_ESCAPE) return;
	}
}
