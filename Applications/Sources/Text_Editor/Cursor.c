/** @file Cursor.c
 * Handle the cursor movements and location.
 * @author Adrien RICCIARDI
 */
#include "Buffer.h"
#include "Configuration.h"
#include "Cursor.h"

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The cursor row location on display. */
static unsigned int Cursor_Display_Row = 0;
/** The cursor column location on display. */
static unsigned int Cursor_Display_Column = 0;

/** The cursor row location in the text buffer. */
static unsigned int Cursor_Buffer_Row = 0;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int CursorMoveToUp(void)
{
	unsigned int Line_Length;
	
	if (Cursor_Display_Row > 0) // The cursor will remain on display
	{
		Cursor_Display_Row--;
		Cursor_Buffer_Row--;
		return 0;
	}
	else // The cursor will go out of display lower bound, so the text must be scrolled
	{
		if ((Cursor_Buffer_Row > 0) && (BufferGetLineLength(Cursor_Buffer_Row - 1, &Line_Length) == 0))
		{
			// The line exists, go to the same column location on display if possible
			if (Line_Length < Cursor_Display_Column) Cursor_Display_Column = Line_Length; // The cursor column can't reach the CONFIGURATION_DISPLAY_COLUMNS_COUNT value
			Cursor_Buffer_Row--;
			return 1;
		}
	}
	return 0;
}

int CursorMoveToDown(void)
{
	unsigned int Line_Length;
	
	if (Cursor_Display_Row < CONFIGURATION_DISPLAY_ROWS_COUNT - 1) // The cursor will remain on display
	{
		Cursor_Display_Row++;
		Cursor_Buffer_Row++;
		return 0;
	}
	else // The cursor will go out of display lower bound, so the text must be scrolled
	{
		// Get the length of the bottom line
		if (BufferGetLineLength(Cursor_Buffer_Row + 1, &Line_Length) == 0)
		{
			// The line exists, go to the same column location on display if possible
			if (Line_Length < Cursor_Display_Column) Cursor_Display_Column = Line_Length; // The cursor column can't reach the CONFIGURATION_DISPLAY_COLUMNS_COUNT value
			Cursor_Buffer_Row++;
			return 1;
		}
		return 0;
	}
}

unsigned int CursorGetBufferCharacterIndex(void)
{
	return BufferFindLineBeginning(Cursor_Buffer_Row) + Cursor_Display_Column;
}

#if 0
unsigned int CursorGetLineLocation(void)
{
	return Cursor_Row;
}
#endif

unsigned int CursorGetDisplayRow(void)
{
	return Cursor_Display_Row;
}

unsigned int CursorGetDisplayColumn(void)
{
	return Cursor_Display_Column;
}

unsigned int CursorGetBufferRow(void)
{
	return Cursor_Buffer_Row;
}
