/** @file Rain.c
 * @see Games.h for description.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
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
static TLibrariesMenu Menu_Rain_Speed =
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
		if (LibrariesKeyboardIsKeyAvailable() && (LibrariesKeyboardReadCharacter() == LIBRARIES_KEYBOARD_KEY_CODE_ESCAPE)) return 1;
	}
	// Or wait the required time
	else
	{
		while (Period_Counts > 0)
		{
			if (LibrariesKeyboardIsKeyAvailable() && (LibrariesKeyboardReadCharacter() == LIBRARIES_KEYBOARD_KEY_CODE_ESCAPE)) return 1;
		
			LibrariesTimerWait(50);
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
	switch (LibrariesMenuDisplay(&Menu_Rain_Speed))
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
	
	LibrariesRandomInitialize();
	
	while (1)
	{
		LibrariesScreenSetBackgroundColor(LIBRARIES_SCREEN_COLOR_WHITE);
		LibrariesScreenClear();
		Raindrops_Count = 0;
		
		for (i = 0; i < 200; i++)
		{
			// Choose drop location (avoiding (79, 24) because the console will scroll vertically going to next line)
			do
			{
				Column = LibrariesRandomGenerateNumber() % LIBRARIES_SCREEN_COLUMNS_COUNT;
				Row = LibrariesRandomGenerateNumber() % LIBRARIES_SCREEN_ROWS_COUNT;
			} while ((Column == LIBRARIES_SCREEN_COLUMNS_COUNT - 1) && (Row == LIBRARIES_SCREEN_ROWS_COUNT - 1));
			
			// Print raindrop
			LibrariesScreenSetCursorPosition(Row, Column);
			LibrariesScreenSetBackgroundColor(LIBRARIES_SCREEN_COLOR_BLUE);
			LibrariesScreenWriteCharacter(' ');
			Raindrops_Count++;
			
			// Print loop count
			LibrariesScreenSetCursorPosition(LIBRARIES_SCREEN_ROWS_COUNT - 1, 0);
			LibrariesScreenSetBackgroundColor(LIBRARIES_SCREEN_COLOR_WHITE);
			LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_LIGHT_GREEN);
			LibrariesScreenWriteString(STRING_RAIN_STATISTICS_1);
			LibrariesScreenWriteInteger(Raindrops_Count);
			LibrariesScreenWriteString(STRING_RAIN_STATISTICS_2);
			LibrariesScreenWriteInteger(Loops_Count);
			
			// Wait required time
			if (Wait(Raining_Speed)) // Exit program if the player hit Escape key
			{
				LibrariesScreenClear();
				return;
			}
		}
		Loops_Count++;
	}
}