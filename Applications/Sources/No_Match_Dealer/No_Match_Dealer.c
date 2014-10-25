/** @file No_Match_Dealer.c
 * @see No_Match_Dealer.h for description.
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/12/2012
 */
#include <stdio.h>
#include <stdlib.h>
#include <System.h>
#include <time.h>
#include "No_Match_Dealer.h"

//-------------------------------------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------------------------------------
#define NUMBERS_COUNT 16

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
int main(void)
{
	int i, Numbers[NUMBERS_COUNT], j;
	
	// Show instructions
	printf(STRING_INSTRUCTIONS);
	
	// Generate numbers
	srand(time(NULL));
	for (i = 0; i < NUMBERS_COUNT; i++) Numbers[i] = rand() % 100;

	// Show numbers on 2 rows
	ScreenSetFontColor(SCREEN_COLOR_LIGHT_BLUE);
	for (i = 0; i < NUMBERS_COUNT; i++)
	{
		// Pad number with a space character to keep alignement when drawing to screen
		if (Numbers[i] < 10) printf(" ");
		printf("%d ", Numbers[i]);
		
		// Go to next line if needed
		if (i % (NUMBERS_COUNT / 2) == 7) printf("\n");
	}
	printf("\n");
	ScreenSetFontColor(SCREEN_COLOR_BLUE);

	// Check for matches
	for (i = 0; i < NUMBERS_COUNT; i++)
	{
		for (j = 0; j < NUMBERS_COUNT; j++)
		{
			if ((i != j) && (Numbers[i] == Numbers[j]))
			{
				ScreenSetFontColor(SCREEN_COLOR_RED);
				printf(STRING_GAME_LOST);
				return EXIT_SUCCESS;
			}
		}
	}

	ScreenSetFontColor(SCREEN_COLOR_GREEN);
	printf(STRING_GAME_WON);
	return EXIT_SUCCESS;
}
