/** @file Rain.c
 * @see Rain.h for description.
 * @author Adrien RICCIARDI
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <System.h>
#include <time.h>
#include "Rain.h"

//-------------------------------------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Wait for Period_Counts * 50 ms.
 * @param Period_Counts How many periods to wait.
 * @return 0 when no key was pressed,
 * @return the corresponding ASCII code when the user pressed a key.
 */
static char Wait(int Period_Counts)
{
	unsigned int Wait_Period;
	
	Wait_Period = time(NULL) + Period_Counts;
	while ((unsigned int) time(NULL) < Wait_Period)
	{
		if (KeyboardIsKeyAvailable()) return KeyboardReadChar();
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int Row, Column, i, Raining_Speed, Loops_Count = 1, Raindrops_Count;
	
	// Check parameters count
	if (argc != 2)
	{
		printf(STRING_USAGE, argv[0]);
		return EXIT_FAILURE;
	}
	
	// Check parameters value
	if (strcmp(argv[1], PARAMETER_RAINING_SPEED_SLOW) == 0) Raining_Speed = RAINING_SPEED_SLOW;
	else if (strcmp(argv[1], PARAMETER_RAINING_SPEED_NORMAL) == 0) Raining_Speed = RAINING_SPEED_NORMAL;
	else if (strcmp(argv[1], PARAMETER_RAINING_SPEED_FAST) == 0) Raining_Speed = RAINING_SPEED_FAST;
	else if (strcmp(argv[1], PARAMETER_RAINING_SPEED_ULTRAFAST) == 0) Raining_Speed = RAINING_SPEED_ULTRAFAST;
	else
	{
		printf(STRING_USAGE, argv[0]);
		return EXIT_FAILURE;
	}
	
	srand(time(NULL));
	
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
				Column = rand() % SCREEN_COLUMNS_COUNT;
				Row = rand() % SCREEN_ROWS_COUNT;
			} while ((Column == SCREEN_COLUMNS_COUNT - 1) && (Row == SCREEN_ROWS_COUNT - 1));
			
			// Print raindrop
			ScreenSetCursorPosition(Row, Column);
			ScreenSetBackgroundColor(SCREEN_COLOR_BLUE);
			putchar(' ');
			fflush(stdout);
			Raindrops_Count++;
			
			// Print loop count
			ScreenSetCursorPosition(SCREEN_ROWS_COUNT - 1, 0);
			ScreenSetBackgroundColor(SCREEN_COLOR_WHITE);
			ScreenSetFontColor(SCREEN_COLOR_LIGHT_GREEN);
			printf(STRING_STATISTICS, Raindrops_Count, Loops_Count);
			fflush(stdout);
			
			// Wait required time
			if (Wait(Raining_Speed) == KEYBOARD_KEY_CODE_ESCAPE) // Exit program if the player hit Escape key
			{
				ScreenClear();
				return EXIT_SUCCESS;
			}
		}
		Loops_Count++;
	}
}