/** @file Submarine.c
 * @see Games.h for description.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
#include "Games.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** The player in-game color. */
#define SUBMARINE_ALIVE_PLAYER_COLOR LIBRARIES_SCREEN_MAKE_COLOR(LIBRARIES_SCREEN_COLOR_LIGHT_GRAY, LIBRARIES_SCREEN_COLOR_LIGHT_GRAY)
/** The player dead color. */
#define SUBMARINE_DEAD_PLAYER_COLOR LIBRARIES_SCREEN_MAKE_COLOR(LIBRARIES_SCREEN_COLOR_RED, LIBRARIES_SCREEN_COLOR_RED)
/** The sea color. */
#define SUBMARINE_SEA_COLOR LIBRARIES_SCREEN_MAKE_COLOR(LIBRARIES_SCREEN_COLOR_BLUE, LIBRARIES_SCREEN_COLOR_BLUE)
/** The obstacles color. */
#define SUBMARINE_OBSTACLE_COLOR LIBRARIES_SCREEN_MAKE_COLOR(LIBRARIES_SCREEN_COLOR_BROWN, LIBRARIES_SCREEN_COLOR_BROWN)

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
static TLibrariesScreenBufferCharacter Submarine_Screen_Buffer[LIBRARIES_SCREEN_ROWS_COUNT][LIBRARIES_SCREEN_COLUMNS_COUNT];

/** The obstacles array, covering a whole column. */
static int Submarine_Obstacles_Bitmask[LIBRARIES_SCREEN_COLUMNS_COUNT];

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
	LibrariesScreenScrollBufferToLeft(Submarine_Screen_Buffer);
	// Scroll the obstacles array too
	for (i = 0; i < LIBRARIES_SCREEN_COLUMNS_COUNT - 1; i++) Submarine_Obstacles_Bitmask[i] = Submarine_Obstacles_Bitmask[i + 1];
	
	// Should a column of obstacles be generated ?
	if (Submarine_Obstacles_Generation_Counter == 0)
	{
		// Generate the obstacles
		do
		{
			Obstacles_Bitmask = LibrariesRandomGenerateNumber();
		} while ((Obstacles_Bitmask & 0x01FFFFFF) == 0x01FFFFFF); // Avoid generating a fully-filled column (WARNING, change this value if the number of rows changes too)
		
		// Fill the column with the obstacles
		for (i = 0; i < LIBRARIES_SCREEN_ROWS_COUNT; i++)
		{
			if (Obstacles_Bitmask & (1 << i)) Color = SUBMARINE_OBSTACLE_COLOR; // There is an obstacle on this row
			else Color = SUBMARINE_SEA_COLOR;
			
			Submarine_Screen_Buffer[i][LIBRARIES_SCREEN_COLUMNS_COUNT - 1].Color = Color;
		}
		
		Submarine_Obstacles_Bitmask[LIBRARIES_SCREEN_COLUMNS_COUNT - 1] = Obstacles_Bitmask;
		
		// Choose the next column that will contain obstacles
		Submarine_Obstacles_Generation_Counter = (LibrariesRandomGenerateNumber() % (SUBMARINE_MAXIMUM_COLUMNS_COUNT_BETWEEN_OBSTACLES - SUBMARINE_MINIMUM_COLUMNS_COUNT_BETWEEN_OBSTACLES)) + SUBMARINE_MINIMUM_COLUMNS_COUNT_BETWEEN_OBSTACLES;
	}
	else
	{
		// Fill the whole column with "water"
		for (i = 0; i < LIBRARIES_SCREEN_ROWS_COUNT; i++) Submarine_Screen_Buffer[i][LIBRARIES_SCREEN_COLUMNS_COUNT - 1].Color = SUBMARINE_SEA_COLOR;
		
		Submarine_Obstacles_Bitmask[LIBRARIES_SCREEN_COLUMNS_COUNT - 1] = 0;
		Submarine_Obstacles_Generation_Counter--;
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void Submarine(void)
{
	int Scene_Scrolling_Frequency_Divider = 0, Is_Player_Dead = 0, Player_Row = LIBRARIES_SCREEN_ROWS_COUNT / 2; // Center the player
	unsigned char Player_Color = SUBMARINE_ALIVE_PLAYER_COLOR;
	unsigned int Start_Time, End_Time, Time_To_Wait, Score = 0;
	char String_Score[64], String_Converted_Score_Value[16];
	
	LibrariesRandomInitialize();
	
	// Reset the scene and the obstacles
	LibrariesScreenClearBuffer(Submarine_Screen_Buffer, SUBMARINE_SEA_COLOR);
	LibrariesMemorySetAreaValue(Submarine_Obstacles_Bitmask, sizeof(Submarine_Obstacles_Bitmask), 0);
	Submarine_Obstacles_Generation_Counter = 0; // Generate immediately a column of obstacles
	
	while (1)
	{
		Start_Time = LibrariesTimerGetValue();
		
		// Clear player trace before the screen is scrolled or the player moved
		Submarine_Screen_Buffer[Player_Row][SUBMARINE_PLAYER_COLUMN].Color = SUBMARINE_SEA_COLOR;
		
		// Handle player keys
		if (LibrariesKeyboardIsKeyAvailable())
		{
			switch (LibrariesKeyboardReadCharacter())
			{
				case LIBRARIES_KEYBOARD_KEY_CODE_ESCAPE:
					return;
					
				case LIBRARIES_KEYBOARD_KEY_CODE_ARROW_UP:
					if (Player_Row > 0) Player_Row--;
					break;
					
				case LIBRARIES_KEYBOARD_KEY_CODE_ARROW_DOWN:
					if (Player_Row < LIBRARIES_SCREEN_ROWS_COUNT - 1) Player_Row++;
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
		LibrariesScreenDisplayBuffer(Submarine_Screen_Buffer);
		
		if (Is_Player_Dead)
		{
			// Display the string on the screen's middle
			LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_WHITE);
			LibrariesScreenSetBackgroundColor(LIBRARIES_SCREEN_COLOR_RED);
			LibrariesScreenSetCursorPosition(LIBRARIES_SCREEN_ROWS_COUNT / 2, 0); // The column coordinate will be computed by the LibrariesScreenWriteCenteredString() function
			LibrariesScreenWriteCenteredString(STRING_SUBMARINE_PLAYER_LOST);
			
			// Display the score string below
			LibrariesStringConcatenate(String_Score, STRING_SUBMARINE_PLAYER_SCORE);
			LibrariesStringConvertUnsignedIntegerToString(Score, String_Converted_Score_Value);
			LibrariesStringConcatenate(String_Score, String_Converted_Score_Value);
			LibrariesScreenSetCursorPosition((LIBRARIES_SCREEN_ROWS_COUNT / 2) + 1, 0); // The column coordinate will be computed by the LibrariesScreenWriteCenteredString() function
			LibrariesScreenWriteCenteredString(String_Score);
			
			LibrariesScreenSetBackgroundColor(LIBRARIES_SCREEN_COLOR_WHITE); // Restore the default background color
			
			// Wait for the player to hit "escape" key
			while (LibrariesKeyboardReadCharacter() != LIBRARIES_KEYBOARD_KEY_CODE_ESCAPE);
			return;
		}
		
		// Wait enough time to make the game work at the desired refresh rate
		End_Time = LibrariesTimerGetValue();
		Time_To_Wait = SUBMARINE_FRAME_PERIOD_MILLISECONDS - (End_Time - Start_Time); // TODO Handle roll-over, even if the system needs to be booted for 49 days for it to happen
		if (Time_To_Wait <= SUBMARINE_FRAME_PERIOD_MILLISECONDS) LibrariesTimerWait(Time_To_Wait); // Do not wait if the frame time has elapsed yet
	}
}