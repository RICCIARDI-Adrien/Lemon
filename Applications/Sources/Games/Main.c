/** @file Main.c
 * Display a menu showing all games.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Games.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A game entry point. */
typedef void (*PointerGameEntryPoint)(void);

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The main menu. */
static TSystemMenu Menu_Main =
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
		STRING_MAIN_MENU_GAME_SUBMARINE,
		STRING_MAIN_MENU_EXIT,
		NULL
	}
};

/** All available games. */
static PointerGameEntryPoint Game_Entry_Points[] =
{
	Numbers,
	BrainCalculation,
	NoMatchDealer,
	Rain,
	GameOfLife,
	Hangman,
	Submarine
};

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	unsigned int Selected_Menu_Entry;
	
	while (1)
	{
		Selected_Menu_Entry = MenuDisplay(&Menu_Main) - 1; // Returned menu entry index starts from 1
		
		// Exit program ?
		if (Selected_Menu_Entry >= sizeof(Game_Entry_Points) / sizeof(PointerGameEntryPoint)) return 0;
		
		// Start the game
		Game_Entry_Points[Selected_Menu_Entry]();
	}
}