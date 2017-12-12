/** @file System_Screen_Clear_Buffer.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemScreenClearBuffer(void *Pointer_Buffer, unsigned char Color)
{
	unsigned int Row, Column;
	TSystemScreenBufferCharacter *Pointer_Buffer_Character = (TSystemScreenBufferCharacter *) Pointer_Buffer;
	
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
