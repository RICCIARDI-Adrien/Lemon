/** @file No_Match_Dealer.c
 * @see No_Match_Dealer.h for description.
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/12/2012
 */
#include <System.h>
#include "No_Match_Dealer.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
#define NUMBERS_COUNT 16

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int main(void)
{
	int i, Numbers[NUMBERS_COUNT], j;
	
	// Show instructions
	ScreenWriteString(STRING_INSTRUCTIONS);
	
	// Generate numbers
	RandomInitialize();
	for (i = 0; i < NUMBERS_COUNT; i++) Numbers[i] = RandomGenerateNumber() % 100;

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
				ScreenWriteString(STRING_GAME_LOST);
				return 0;
			}
		}
	}

	ScreenSetFontColor(SCREEN_COLOR_GREEN);
	ScreenWriteString(STRING_GAME_WON);
	return 0;
}
