/** @file System_Screen_Set_Cursor_Position.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemScreenSetCursorPosition(unsigned int Row, unsigned int Column)
{
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_SET_CURSOR_POSITION, Row, Column, NULL, NULL);
}
