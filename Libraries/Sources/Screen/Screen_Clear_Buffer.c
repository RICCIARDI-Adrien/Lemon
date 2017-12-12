/** @file Screen_Clear_Buffer.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenClearBuffer(void *Pointer_Buffer, unsigned char Color)
{
	unsigned int Row, Column;
	TScreenBufferCharacter *Pointer_Buffer_Character = (TScreenBufferCharacter *) Pointer_Buffer;
	
	for (Row = 0; Row < SYSTEM_SCREEN_ROWS_COUNT; Row++)
	{
		for (Column = 0; Column < SYSTEM_SCREEN_COLUMNS_COUNT; Column++)
		{
			Pointer_Buffer_Character->ASCII_Code = ' ';
			Pointer_Buffer_Character->Color = Color;
			Pointer_Buffer_Character++;
		}
	}
}