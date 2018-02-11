/** @file Display.c
 * @see Display.h for description.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
#include "Configuration.h"
#include "Display.h"

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The buffer that will be sent verbatim to the VGA video memory. */
static TLibrariesScreenBufferCharacter Display_Video_Memory[CONFIGURATION_DISPLAY_ROWS_COUNT * CONFIGURATION_DISPLAY_COLUMNS_COUNT];

/** The cursor row location. */
static unsigned int Display_Cursor_Row = 0;
/** The cursor column location. */
static unsigned int Display_Cursor_Column = 0;

/** The current color (foreground and background). */
static unsigned char Display_Color = LIBRARIES_SCREEN_MAKE_COLOR(CONFIGURATION_TEXT_FOREGROUND_COLOR, CONFIGURATION_TEXT_BACKGROUND_COLOR);

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
void DisplayClear(void)
{
	// Clear the screen
	LibrariesScreenClearBuffer(Display_Video_Memory, LIBRARIES_SCREEN_MAKE_COLOR(CONFIGURATION_TEXT_FOREGROUND_COLOR, CONFIGURATION_TEXT_BACKGROUND_COLOR));
	
	// Put the cursor to the upper left corner
	Display_Cursor_Row = 0;
	Display_Cursor_Column = 0;
}

void DisplayWriteCharacter(char Character)
{
	TLibrariesScreenBufferCharacter Display_Character;
	
	switch (Character)
	{
		// Set cursor location to the beginning of the line
		case '\r':
			Display_Cursor_Column = 0;
			break;
		
		// Go to the beginning of the next line
		case '\n':
			// Go to next line if that does not cross the display lower bound
			if (Display_Cursor_Row < CONFIGURATION_DISPLAY_ROWS_COUNT - 1)
			{
				Display_Cursor_Column = 0;
				Display_Cursor_Row++;
			}
			break;
		
		// Display other characters
		default:
			Display_Character.ASCII_Code = Character;
			Display_Character.Color = Display_Color;
			
			// Add the character to the video memory buffer
			Display_Video_Memory[Display_Cursor_Row * CONFIGURATION_DISPLAY_COLUMNS_COUNT + Display_Cursor_Column] = Display_Character;
			
			// Update cursor location
			Display_Cursor_Column++;
			if (Display_Cursor_Column >= CONFIGURATION_DISPLAY_COLUMNS_COUNT) // Go to next line
			{
				Display_Cursor_Column = 0;
				Display_Cursor_Row++;
			}
			break;
	}
}

void DisplaySetBackgroundColor(TLibrariesScreenColor Color_Code)
{
	Display_Color &= 0x0F; // Erase the current background color
	Display_Color |= (Color_Code & 0x0F) << 4; // Set the new one
}

void DisplayGetCursorPosition(unsigned int *Pointer_Row, unsigned int *Pointer_Column)
{
	*Pointer_Row = Display_Cursor_Row;
	*Pointer_Column = Display_Cursor_Column;
}

void DisplaySetCursorPosition(unsigned int Row, unsigned int Column)
{
	if ((Row < CONFIGURATION_DISPLAY_ROWS_COUNT) && (Column < CONFIGURATION_DISPLAY_COLUMNS_COUNT))
	{
		Display_Cursor_Row = Row;
		Display_Cursor_Column = Column;
	}
}

void DisplayRenderToScreen(void)
{
	LibrariesScreenDisplayBuffer((unsigned char *) Display_Video_Memory);
}