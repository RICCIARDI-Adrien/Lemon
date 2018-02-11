/** @file System_Screen_Get_Cursor_Position.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemScreenGetCursorPosition(unsigned int *Pointer_Row, unsigned int *Pointer_Column)
{
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_GET_CURSOR_POSITION, 0, 0, Pointer_Row, Pointer_Column);
}
