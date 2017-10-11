/** @file Rain.c
 * @see Games.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Games.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
// Raining speeds (act as a frequency divider)
/** Slow speed. */
#define RAINING_SPEED_SLOW 10
/** Normal speed. */
#define RAINING_SPEED_NORMAL 4
/** Fast speed. */
#define RAINING_SPEED_FAST 1
/** Fastest speed. */
#define RAINING_SPEED_ULTRAFAST 0

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Allow to select the rain speed. */
static TMenu Menu_Rain_Speed =
{
	STRING_RAIN_MENU_TITLE,
	STRING_RAIN_MENU_PROMPT,
	{
		STRING_RAIN_MENU_SLOW,
		STRING_RAIN_MENU_NORMAL,
		STRING_RAIN_MENU_FAST,
		STRING_RAIN_MENU_ULTRAFAST,
		STRING_RAIN_MENU_EXIT,
		NULL
	}
};

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Wait for Period_Counts * 50 ms and tells if the Escape key was pressed
 * @param Period_Counts How many periods to wait.
 * @return 1 if the Escape key was pressed,
 * @return 0 otherwise.
 */
static int Wait(unsigned int Period_Counts)
{
	// Check only the keyboard if there is no need to wait
	if (Period_Counts == 0)
	{
		if (SystemKeyboardIsKeyAvailable() && (KeyboardReadCharacter() == KEYBOARD_KEY_CODE_ESCAPE)) return 1;
	}
	// Or wait the required time
	else
	{
		while (Period_Counts > 0)
		{
			if (SystemKeyboardIsKeyAvailable() && (KeyboardReadCharacter() == KEYBOARD_KEY_CODE_ESCAPE)) return 1;
		
			SystemWait(50);
			Period_Counts--;
		}
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void Rain(void)
{
	int Row, Column, i, Raining_Speed, Loops_Count = 1, Raindrops_Count;
	
	// Ask the user for the rain speed
	switch (MenuDisplay(&Menu_Rain_Speed))
	{
		case 1:
			Raining_Speed = RAINING_SPEED_SLOW;
			break;
			
		case 2:
			Raining_Speed = RAINING_SPEED_NORMAL;
			break;
			
		case 3:
			Raining_Speed = RAINING_SPEED_FAST;
			break;
			
		case 4:
			Raining_Speed = RAINING_SPEED_ULTRAFAST;
			break;
			
		default:
			return;
	}
	
	RandomInitialize();
	
	while (1)
	{
		ScreenSetBackgroundColor(SCREEN_COLOR_WHITE);
		ScreenClear();
		Raindrops_Count = 0;
		
		for (i = 0; i < 200; i++)
		{
			// Choose drop location (avoiding (79, 24) because the console will scroll vertically going to next line)
			do
			{
				Column = RandomGenerateNumber() % SCREEN_COLUMNS_COUNT;
				Row = RandomGenerateNumber() % SCREEN_ROWS_COUNT;
			} while ((Column == SCREEN_COLUMNS_COUNT - 1) && (Row == SCREEN_ROWS_COUNT - 1));
			
			// Print raindrop
			ScreenSetCursorPosition(Row, Column);
			ScreenSetBackgroundColor(SCREEN_COLOR_BLUE);
			ScreenWriteCharacter(' ');
			Raindrops_Count++;
			
			// Print loop count
			ScreenSetCursorPosition(SCREEN_ROWS_COUNT - 1, 0);
			ScreenSetBackgroundColor(SCREEN_COLOR_WHITE);
			ScreenSetFontColor(SCREEN_COLOR_LIGHT_GREEN);
			ScreenWriteString(STRING_RAIN_STATISTICS_1);
			ScreenWriteInteger(Raindrops_Count);
			ScreenWriteString(STRING_RAIN_STATISTICS_2);
			ScreenWriteInteger(Loops_Count);
			
			// Wait required time
			if (Wait(Raining_Speed)) // Exit program if the player hit Escape key
			{
				ScreenClear();
				return;
			}
		}
		Loops_Count++;
	}
}