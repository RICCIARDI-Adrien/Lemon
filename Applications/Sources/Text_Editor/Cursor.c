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
/** The cursor row location. */
static unsigned int Cursor_Row = 0;
/** The cursor column location. */
static unsigned int Cursor_Column = 0;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void CursorMoveToBottom(void)
{
	unsigned int Line_Length;
	
	// Get the length of the bottom line
	if (BufferGetLineLength(Cursor_Row + 1, &Line_Length) == 0)
	{
		// The line exists, go to the same column location if possible
		if (Line_Length < Cursor_Column) Cursor_Column = Line_Length; // The cursor column can't reach the CONFIGURATION_SCREEN_COLUMNS_COUNT value
		Cursor_Row++;
	}
}

unsigned int CursorGetCharacterIndex(void)
{
	//ScreenWriteUnsignedInteger((Cursor_Row * CONFIGURATION_SCREEN_COLUMNS_COUNT) + Cursor_Column);
	//ScreenWriteCharacter('\n');
	return BufferFindLineBeginning(Cursor_Row) + Cursor_Column; //(Cursor_Row * CONFIGURATION_SCREEN_COLUMNS_COUNT) + Cursor_Column;
}

unsigned int CursorGetLineLocation(void)
{
	return Cursor_Row;
}

unsigned int CursorGetScreenRow(void)
{
	return Cursor_Row % CONFIGURATION_SCREEN_ROWS_COUNT;
}

unsigned int CursorGetScreenColumn(void)
{
	return Cursor_Column;
}

#if 0
void CursorConvertCharacterIndexToScreenLocation(unsigned int Character_Index, unsigned int *Pointer_Screen_Row, unsigned int *Pointer_Screen_Column)
{
	*Pointer_Screen_Row = Character_Index / CONFIGURATION_SCREEN_COLUMNS_COUNT;
	*Pointer_Screen_Column = Character_Index - (*Pointer_Screen_Row * CONFIGURATION_SCREEN_COLUMNS_COUNT);
}
#endif