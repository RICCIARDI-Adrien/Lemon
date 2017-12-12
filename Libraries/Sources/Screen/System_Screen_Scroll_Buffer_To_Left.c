/** @file System_Screen_Scroll_Buffer_To_Left.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemScreenScrollBufferToLeft(void *Pointer_Buffer)
{
	int Row, Column;
	unsigned short *Pointer_Buffer_Word = (unsigned short *) Pointer_Buffer; // Each buffer character is 2-byte long
	
	for (Row = 0; Row < SYSTEM_SCREEN_ROWS_COUNT; Row++)
	{
		for (Column = 0; Column < SYSTEM_SCREEN_COLUMNS_COUNT - 1; Column++) // Stop one column before the last one
		{
			*Pointer_Buffer_Word = *(Pointer_Buffer_Word + 1);
			Pointer_Buffer_Word++;
		}
		Pointer_Buffer_Word++; // Skip the rightmost column
	}
}
