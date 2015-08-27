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
		STRING_MAIN_MENU_EXIT,
		NULL
	}
};

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	while (1)
	{
		switch (MenuDisplay(&Menu_Main))
		{
			// Numbers
			case 1:
				break;
				
			// Brain calculation
			case 2:
				BrainCalculation();
				break;
				
			default:
				return 0;
		}
	}
}