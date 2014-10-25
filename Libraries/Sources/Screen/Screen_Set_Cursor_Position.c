/** @file Screen_Set_Cursor_Position.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 13/12/2013
 */
#include <Screen.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenSetCursorPosition(unsigned int Row, unsigned int Column)
{
	SystemCall(SYSTEM_CALL_SCREEN_SET_CURSOR_POSITION, Row, Column, NULL, NULL);
}