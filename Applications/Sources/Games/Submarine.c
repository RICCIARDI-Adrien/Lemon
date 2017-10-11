/** @file Submarine.c
 * @see Games.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Games.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** The player in-game color. */
#define SUBMARINE_ALIVE_PLAYER_COLOR SCREEN_MAKE_COLOR(SCREEN_COLOR_LIGHT_GRAY, SCREEN_COLOR_LIGHT_GRAY)
/** The player dead color. */
#define SUBMARINE_DEAD_PLAYER_COLOR SCREEN_MAKE_COLOR(SCREEN_COLOR_RED, SCREEN_COLOR_RED)
/** The sea color. */
#define SUBMARINE_SEA_COLOR SCREEN_MAKE_COLOR(SCREEN_COLOR_BLUE, SCREEN_COLOR_BLUE)
/** The obstacles color. */
#define SUBMARINE_OBSTACLE_COLOR SCREEN_MAKE_COLOR(SCREEN_COLOR_BROWN, SCREEN_COLOR_BROWN)

/** The minimum amount of columns between two obstacles. */
#define SUBMARINE_MINIMUM_COLUMNS_COUNT_BETWEEN_OBSTACLES 8
/** The maximum amount of columns between two obstacles. */
#define SUBMARINE_MAXIMUM_COLUMNS_COUNT_BETWEEN_OBSTACLES 14

/** The frequency at which the scene is scrolled. */
#define SUBMARINE_SCENE_SCROLLING_FREQUENCY_DIVIDER 4

/** The player column location. */
#define SUBMARINE_PLAYER_COLUMN 3

/** How many time to wait between two frames to achieve a 60Hz frame rate. */
#define SUBMARINE_FRAME_PERIOD_MILLISECONDS 16

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The screen buffer. */
static TScreenBufferCharacter Submarine_Screen_Buffer[SCREEN_ROWS_COUNT][SCREEN_COLUMNS_COUNT];

/** The obstacles array, covering a whole column. */
static int Submarine_Obstacles_Bitmask[SCREEN_COLUMNS_COUNT];

/** Generate a column of obstacles when this variables reaches SUBMARINE_OBSTACLES_GENERATION_FREQUENCY_DIVIDER. */ 
static int Submarine_Obstacles_Generation_Counter; // This variable is not local to a function in order to be reset by the game entry point

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Generate the game rightmost column. */
static void SubmarineGenerateNextColumn(void)
{
	unsigned int i, Obstacles_Bitmask;
	unsigned char Color;
	
	// Move the scene to left
	ScreenScrollBufferToLeft(Submarine_Screen_Buffer);
	// Scroll the obstacles array too
	for (i = 0; i < SCREEN_COLUMNS_COUNT - 1; i++) Submarine_Obstacles_Bitmask[i] = Submarine_Obstacles_Bitmask[i + 1];
	
	// Should a column of obstacles be generated ?
	if (Submarine_Obstacles_Generation_Counter == 0)
	{
		// Generate the obstacles
		do
		{
			Obstacles_Bitmask = RandomGenerateNumber();
		} while ((Obstacles_Bitmask & 0x01FFFFFF) == 0x01FFFFFF); // Avoid generating a fully-filled column (WARNING, change this value if the number of rows changes too)
		
		// Fill the column with the obstacles
		for (i = 0; i < SCREEN_ROWS_COUNT; i++)
		{
			if (Obstacles_Bitmask & (1 << i)) Color = SUBMARINE_OBSTACLE_COLOR; // There is an obstacle on this row
			else Color = SUBMARINE_SEA_COLOR;
			
			Submarine_Screen_Buffer[i][SCREEN_COLUMNS_COUNT - 1].Color = Color;
		}
		
		Submarine_Obstacles_Bitmask[SCREEN_COLUMNS_COUNT - 1] = Obstacles_Bitmask;
		
		// Choose the next column that will contain obstacles
		Submarine_Obstacles_Generation_Counter = (RandomGenerateNumber() % (SUBMARINE_MAXIMUM_COLUMNS_COUNT_BETWEEN_OBSTACLES - SUBMARINE_MINIMUM_COLUMNS_COUNT_BETWEEN_OBSTACLES)) + SUBMARINE_MINIMUM_COLUMNS_COUNT_BETWEEN_OBSTACLES;
	}
	else
	{
		// Fill the whole column with "water"
		for (i = 0; i < SCREEN_ROWS_COUNT; i++) Submarine_Screen_Buffer[i][SCREEN_COLUMNS_COUNT - 1].Color = SUBMARINE_SEA_COLOR;
		
		Submarine_Obstacles_Bitmask[SCREEN_COLUMNS_COUNT - 1] = 0;
		Submarine_Obstacles_Generation_Counter--;
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void Submarine(void)
{
	int Scene_Scrolling_Frequency_Divider = 0, Is_Player_Dead = 0, Player_Row = SCREEN_ROWS_COUNT / 2; // Center the player
	unsigned char Player_Color = SUBMARINE_ALIVE_PLAYER_COLOR;
	unsigned int Start_Time, End_Time, Time_To_Wait, Score = 0;
	char String_Score[64], String_Converted_Score_Value[16];
	
	RandomInitialize();
	
	// Reset the scene and the obstacles
	ScreenClearBuffer(Submarine_Screen_Buffer, SUBMARINE_SEA_COLOR);
	MemorySetAreaValue(Submarine_Obstacles_Bitmask, sizeof(Submarine_Obstacles_Bitmask), 0);
	Submarine_Obstacles_Generation_Counter = 0; // Generate immediately a column of obstacles
	
	while (1)
	{
		Start_Time = SystemGetTimerValue();
		
		// Clear player trace before the screen is scrolled or the player moved
		Submarine_Screen_Buffer[Player_Row][SUBMARINE_PLAYER_COLUMN].Color = SUBMARINE_SEA_COLOR;
		
		// Handle player keys
		if (SystemKeyboardIsKeyAvailable())
		{
			switch (SystemKeyboardReadCharacter())
			{
				case KEYBOARD_KEY_CODE_ESCAPE:
					return;
					
				case KEYBOARD_KEY_CODE_ARROW_UP:
					if (Player_Row > 0) Player_Row--;
					break;
					
				case KEYBOARD_KEY_CODE_ARROW_DOWN:
					if (Player_Row < SCREEN_ROWS_COUNT - 1) Player_Row++;
					break;
			}
		}
		
		if (Scene_Scrolling_Frequency_Divider == SUBMARINE_SCENE_SCROLLING_FREQUENCY_DIVIDER)
		{
			SubmarineGenerateNextColumn();
			Scene_Scrolling_Frequency_Divider = 0;
		}
		else Scene_Scrolling_Frequency_Divider++;
		
		// Check collisions
		if (Submarine_Obstacles_Bitmask[SUBMARINE_PLAYER_COLUMN] & (1 << Player_Row))
		{
			Is_Player_Dead = 1;
			Player_Color = SUBMARINE_DEAD_PLAYER_COLOR;
		}
		// Increment the score only if an obstacles column was successfully crossed
		else if ((Scene_Scrolling_Frequency_Divider == 0) && (Submarine_Obstacles_Bitmask[SUBMARINE_PLAYER_COLUMN] != 0)) Score++; // An obstacles column is moved on the screen only when Scene_Scrolling_Frequency_Divider is zero, and an obstacles column is present only when its value is different from zero
		
		// Display the player
		Submarine_Screen_Buffer[Player_Row][SUBMARINE_PLAYER_COLUMN].Color = Player_Color;
		
		// Refresh the screen every time to force the game to run at 60Hz
		ScreenDisplayBuffer(Submarine_Screen_Buffer);
		
		if (Is_Player_Dead)
		{
			// Display the string on the screen's middle
			ScreenSetFontColor(SCREEN_COLOR_WHITE);
			ScreenSetBackgroundColor(SCREEN_COLOR_RED);
			ScreenSetCursorPosition(SCREEN_ROWS_COUNT / 2, 0); // The column coordinate will be computed by the ScreenWriteCenteredString() function
			ScreenWriteCenteredString(STRING_SUBMARINE_PLAYER_LOST);
			
			// Display the score string below
			StringConcatenate(String_Score, STRING_SUBMARINE_PLAYER_SCORE);
			StringConvertUnsignedIntegerToString(Score, String_Converted_Score_Value);
			StringConcatenate(String_Score, String_Converted_Score_Value);
			ScreenSetCursorPosition((SCREEN_ROWS_COUNT / 2) + 1, 0); // The column coordinate will be computed by the ScreenWriteCenteredString() function
			ScreenWriteCenteredString(String_Score);
			
			ScreenSetBackgroundColor(SCREEN_COLOR_WHITE); // Restore the default background color
			
			// Wait for the player to hit "escape" key
			while (SystemKeyboardReadCharacter() != KEYBOARD_KEY_CODE_ESCAPE);
			return;
		}
		
		// Wait enough time to make the game work at the desired refresh rate
		End_Time = SystemGetTimerValue();
		Time_To_Wait = SUBMARINE_FRAME_PERIOD_MILLISECONDS - (End_Time - Start_Time); // TODO Handle roll-over, even if the system needs to be booted for 49 days for it to happen
		if (Time_To_Wait <= SUBMARINE_FRAME_PERIOD_MILLISECONDS) SystemWait(Time_To_Wait); // Do not wait if the frame time has elapsed yet
	}
}