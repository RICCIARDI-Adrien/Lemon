/** @file Test_Display_ASCII_Characters.
 * Display all the ASCII characters the video card can generate.
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------------------------------------
/** Displayed line length. */
#define LINE_COLUMNS_COUNT 16

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
int main(void)
{
	unsigned char Video_Buffer[SYSTEM_SCREEN_ROWS_COUNT * SCREEN_COLUMNS_COUNT * 2];
	unsigned int Row, Column, Index = 0;
	unsigned char Character_Code = 0;
	
	// Clear screen
	for (Row = 0; Row < sizeof(Video_Buffer); Row += 2)
	{
		Video_Buffer[Row] = ' ';
		Video_Buffer[Row + 1] = SCREEN_MAKE_COLOR(SCREEN_COLOR_BLACK, SCREEN_COLOR_BLACK);
	}
	
	// Display characters
	for (Row = 0; Row < 256 / LINE_COLUMNS_COUNT; Row++)
	{
		for (Column = 0; Column < LINE_COLUMNS_COUNT; Column++)
		{
			Video_Buffer[Index] = Character_Code;
			Video_Buffer[Index + 1] = SCREEN_MAKE_COLOR(SCREEN_COLOR_WHITE, SCREEN_COLOR_BLACK);
			Index += 2;
			Character_Code++;
		}
		
		// Go to the next line beginning
		Index += (SCREEN_COLUMNS_COUNT - LINE_COLUMNS_COUNT) * 2;
	}
	
	ScreenDisplayBuffer(Video_Buffer);
	SystemKeyboardReadCharacter();
	
	ScreenClear();
	return 0;
}