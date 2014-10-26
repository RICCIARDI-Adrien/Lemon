/** @file Rain.c
 * @see Rain.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Rain.h"

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Wait for Period_Counts * 50 ms.
 * @param Period_Counts How many periods to wait.
 * @return 0 when no key was pressed,
 * @return the corresponding ASCII code when the user pressed a key.
 */
static char Wait(int Period_Counts)
{
	// Check only the keyboard if there is no need to wait
	if (Period_Counts == 0)
	{
		if (KeyboardIsKeyAvailable()) return KeyboardReadCharacter();
	}
	// Or wait the required time
	else
	{
		while (Period_Counts > 0)
		{
			if (KeyboardIsKeyAvailable()) return KeyboardReadCharacter();
		
			SystemWait(1);
			Period_Counts--;
		}
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int Row, Column, i, Raining_Speed, Loops_Count = 1, Raindrops_Count;
	
	// Check parameters count
	if (argc != 2)
	{
		ScreenWriteString(STRING_USAGE_1);
		ScreenWriteString(argv[0]);
		ScreenWriteString(STRING_USAGE_2);
		return -1;
	}
	
	// Check parameters value
	if (StringCompare(argv[1], PARAMETER_RAINING_SPEED_SLOW)) Raining_Speed = RAINING_SPEED_SLOW;
	else if (StringCompare(argv[1], PARAMETER_RAINING_SPEED_NORMAL)) Raining_Speed = RAINING_SPEED_NORMAL;
	else if (StringCompare(argv[1], PARAMETER_RAINING_SPEED_FAST)) Raining_Speed = RAINING_SPEED_FAST;
	else if (StringCompare(argv[1], PARAMETER_RAINING_SPEED_ULTRAFAST)) Raining_Speed = RAINING_SPEED_ULTRAFAST;
	else
	{
		ScreenWriteString(STRING_USAGE_1);
		ScreenWriteString(argv[0]);
		ScreenWriteString(STRING_USAGE_2);
		return -1;
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
			ScreenWriteString(STRING_STATISTICS_1);
			ScreenWriteInteger(Raindrops_Count);
			ScreenWriteString(STRING_STATISTICS_2);
			ScreenWriteInteger(Loops_Count);
			
			// Wait required time
			if (Wait(Raining_Speed) == KEYBOARD_KEY_CODE_ESCAPE) // Exit program if the player hit Escape key
			{
				ScreenClear();
				return 0;
			}
		}
		Loops_Count++;
	}
}