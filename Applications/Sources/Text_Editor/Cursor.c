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
		
		// Go to the same column location on display if possible
		BufferGetLineLength(Cursor_Buffer_Row, &Line_Length);
		if (Line_Length < Cursor_Display_Column) Cursor_Display_Column = Line_Length; // The cursor column can't reach the CONFIGURATION_DISPLAY_COLUMNS_COUNT value
		return 0;
	}
	else // The cursor will go out of display upper bound, so the text must be scrolled
	{
		if (Cursor_Buffer_Row > 0)
		{
			// Go to the same column location on display if possible
			BufferGetLineLength(Cursor_Buffer_Row - 1, &Line_Length);
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
		
		// Go to the same column location on display if possible
		BufferGetLineLength(Cursor_Buffer_Row, &Line_Length);
		if (Line_Length < Cursor_Display_Column) Cursor_Display_Column = Line_Length; // The cursor column can't reach the CONFIGURATION_DISPLAY_COLUMNS_COUNT value
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

int CursorMoveToLeft(void)
{
	unsigned int Line_Length;
	
	if (Cursor_Display_Column > 0) // The cursor will remain on display
	{
		Cursor_Display_Column--;
		return 0;
	}
	else // The cursor will go out of display leftmost bound, so the text must be scrolled one line upper
	{
		if (Cursor_Buffer_Row > 0)
		{
			// Put the cursor at the end of the line
			BufferGetLineLength(Cursor_Buffer_Row - 1, &Line_Length);
			if (Line_Length >= CONFIGURATION_DISPLAY_COLUMNS_COUNT) Line_Length = CONFIGURATION_DISPLAY_COLUMNS_COUNT - 1; // Make sure the cursor location will fit on the display
			Cursor_Display_Row--;
			Cursor_Display_Column = Line_Length;
			Cursor_Buffer_Row--;
			
			// Force the text page redraw only if the new row is not displayed
			if (Cursor_Display_Row == 0) return 1;
		}
	}
	return 0;
}

int CursorMoveToRight(void)
{
	unsigned int Line_Length;
	
	// Cache the next line length and presence
	BufferGetLineLength(Cursor_Buffer_Row, &Line_Length);
	
	if ((Cursor_Display_Column < Line_Length) && (Cursor_Display_Column < CONFIGURATION_DISPLAY_COLUMNS_COUNT - 1)) // The cursor will remain on this line and on the display
	{
		Cursor_Display_Column++;
		return 0;
	}
	else // The cursor will go out of display rightmost bound or the current line bound, so the text must be scrolled one line downer
	{
		if (BufferGetLineLength(Cursor_Buffer_Row + 1, &Line_Length) == 0)
		{
			// The downer line exists, so put the cursor at the beginning of the line
			Cursor_Display_Row++;
			Cursor_Display_Column = 0;
			Cursor_Buffer_Row++;
			
			// Force the text page redraw only if the new row is not displayed
			if (Cursor_Display_Row == CONFIGURATION_DISPLAY_COLUMNS_COUNT - 1) return 1;
		}
	}
	return 0;
}

unsigned int CursorGetBufferCharacterIndex(void)
{
	return BufferFindLineBeginning(Cursor_Buffer_Row) + Cursor_Display_Column;
}

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
