/** @file Screen_Get_Cursor_Position.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 13/12/2013
 */
#include <Screen.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenGetCursorPosition(unsigned int *Pointer_Row, unsigned int *Pointer_Column)
{
	SystemCall(SYSTEM_CALL_SCREEN_GET_CURSOR_POSITION, 0, 0, Pointer_Row, Pointer_Column);
}