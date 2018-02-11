/** @file Screen_Clear_Buffer.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesScreenClearBuffer(void *Pointer_Buffer, unsigned char Color)
{
	unsigned int Row, Column;
	TLibrariesScreenBufferCharacter *Pointer_Buffer_Character = (TLibrariesScreenBufferCharacter *) Pointer_Buffer;
	
	for (Row = 0; Row < LIBRARIES_SCREEN_ROWS_COUNT; Row++)
	{
		for (Column = 0; Column < LIBRARIES_SCREEN_COLUMNS_COUNT; Column++)
		{
			Pointer_Buffer_Character->ASCII_Code = ' ';
			Pointer_Buffer_Character->Color = Color;
			Pointer_Buffer_Character++;
		}
	}
}
