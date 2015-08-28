/** @file Main.c
 * Display a menu showing all games.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Games.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The main menu. */
static TMenu Menu_Main =
{
	STRING_MAIN_MENU_TITLE,
	STRING_MAIN_MENU_PROMPT,
	{
		STRING_MAIN_MENU_GAME_NUMBERS,
		STRING_MAIN_MENU_GAME_BRAIN_CALCULATION,
		STRING_MAIN_MENU_GAME_NO_MATCH_DEALER,
		STRING_MAIN_MENU_GAME_RAIN,
		STRING_MAIN_MENU_GAME_GAME_OF_LIFE,
		STRING_MAIN_MENU_GAME_HANGMAN,
		STRING_MAIN_MENU_EXIT,
		NULL
	}
};

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	while (1)
	{
		switch (MenuDisplay(&Menu_Main))
		{
			case 1:
				Numbers();
				break;
				
			case 2:
				BrainCalculation();
				break;
				
			case 3:
				NoMatchDealer();
				break;
				
			case 4:
				Rain();
				break;
				
			case 5:
				GameOfLife();
				break;
				
			case 6:
				Hangman();
				break;
				
			default:
				return 0;
		}
	}
}