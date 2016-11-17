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

/** During a vertical move, try to keep the cursor column as possible. */
static unsigned int Cursor_Vertical_Move_Display_Column = 0;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int CursorMoveToUp(void)
{
	unsigned int Line_Length;
	int Return_Value = 0;
	
	if (Cursor_Display_Row > 0) // The cursor will remain on display
	{
		Cursor_Display_Row--;
		Cursor_Buffer_Row--;
		
		// Go to the same column location on display if possible
		BufferGetLineLength(Cursor_Buffer_Row, &Line_Length);
		if (Line_Length < Cursor_Vertical_Move_Display_Column) Cursor_Display_Column = Line_Length; // The cursor column can't reach the CONFIGURATION_DISPLAY_COLUMNS_COUNT value
		else Cursor_Display_Column = Cursor_Vertical_Move_Display_Column;
	}
	else // The cursor will go out of display upper bound, so the text must be scrolled
	{
		if (Cursor_Buffer_Row > 0)
		{
			// Go to the same column location on display if possible
			BufferGetLineLength(Cursor_Buffer_Row - 1, &Line_Length);
			if (Line_Length < Cursor_Vertical_Move_Display_Column) Cursor_Display_Column = Line_Length; // The cursor column can't reach the CONFIGURATION_DISPLAY_COLUMNS_COUNT value
			else Cursor_Display_Column = Cursor_Vertical_Move_Display_Column;
			Cursor_Buffer_Row--;
			
			Return_Value = 1;
		}
	}
	
	return Return_Value;
}

int CursorMoveToDown(void)
{
	unsigned int Line_Length;
	int Return_Value = 0;
	
	// Check if the next line exists and cache its length
	if (BufferGetLineLength(Cursor_Buffer_Row + 1, &Line_Length) != 0) return Return_Value; // Nothing to do if the downer line does not exist
	
	if (Cursor_Display_Row < CONFIGURATION_DISPLAY_ROWS_COUNT - 1) Cursor_Display_Row++; // The cursor will remain on display
	else Return_Value = 1; // The cursor will go out of display lower bound, so the text must be scrolled
	
	Cursor_Buffer_Row++;
	
	// Update cursor location
	if (Line_Length >= Cursor_Vertical_Move_Display_Column) Cursor_Display_Column = Cursor_Vertical_Move_Display_Column; // Go to the preferred location if possible
	else if (Line_Length < Cursor_Display_Column) Cursor_Display_Column = Line_Length;
	
	return Return_Value;
}

int CursorMoveToLeft(void)
{
	unsigned int Line_Length;
	int Return_Value = 0;
	
	if (Cursor_Display_Column > 0) Cursor_Display_Column--; // The cursor will remain on display
	else if (Cursor_Buffer_Row > 0) // The cursor will go out of display leftmost bound, so the text must be scrolled one line upper
	{
			// Put the cursor at the end of the line
		BufferGetLineLength(Cursor_Buffer_Row - 1, &Line_Length); // No need to check the return value, the previous line is always available
		if (Cursor_Display_Row > 0) Cursor_Display_Row--;
		Cursor_Display_Column = Line_Length;
		Cursor_Buffer_Row--;
		
		// Force the text page redraw only if the new row is not displayed
		if (Cursor_Display_Row == 0) Return_Value = 1;
	}

	// Keep the current column location for an eventual vertical move
	Cursor_Vertical_Move_Display_Column = Cursor_Display_Column;

	return Return_Value;
}

int CursorMoveToRight(void)
{
	unsigned int Line_Length;
	int Return_Value = 0, Is_Last_Line;
	
	// Cache the current line length and presence
	if (BufferGetLineLength(Cursor_Buffer_Row, &Line_Length) != 0) return Return_Value; // Nothing to do if the current line does not exist (i.e. the buffer is empty)
	
	if ((Cursor_Display_Column < Line_Length) && (Cursor_Display_Column < CONFIGURATION_DISPLAY_COLUMNS_COUNT - 1)) Cursor_Display_Column++; // The cursor will remain on this line and on the display
	else // The cursor will go out of display rightmost bound or the current line bound, so the text must be scrolled one line downer
	{
		Is_Last_Line = BufferGetLineLength(Cursor_Buffer_Row + 1, &Line_Length);
		if (!Is_Last_Line|| (Is_Last_Line && (CursorGetBufferCharacterIndex() == Buffer_Characters_Count - 1))) // Little hack to allow a single '\n' to be displayed even if it is the last character in the buffer
		{
			// The downer line exists, so put the cursor at the beginning of the line
			if (Cursor_Display_Row < CONFIGURATION_DISPLAY_ROWS_COUNT - 1) Cursor_Display_Row++; // Do not cross the display lower bound
			Cursor_Display_Column = 0;
			Cursor_Buffer_Row++;
			
			// Force the text page redraw only if the new row is not displayed
			if (Cursor_Display_Row == CONFIGURATION_DISPLAY_ROWS_COUNT - 1) Return_Value = 1;
		}
	}
	
	// Keep the current column location for an eventual vertical move
	Cursor_Vertical_Move_Display_Column = Cursor_Display_Column;
	
	return Return_Value;
}

void CursorGoToLineBeginning(void)
{
	Cursor_Display_Column = 0;
	Cursor_Vertical_Move_Display_Column = 0;
}

void CursorGoToLineEnd(void)
{
	unsigned int Line_Length;
	
	// Get the current line length
	if (BufferGetLineLength(Cursor_Buffer_Row, &Line_Length) != 0) return; // The current line does not exist (the buffer is probably empty), do nothing
	
	Cursor_Display_Column = Line_Length;
	Cursor_Vertical_Move_Display_Column = Line_Length;
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
