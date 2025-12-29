/** @file Screen_Set_Cursor_Visible.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesScreenSetCursorVisible(int Is_Visible)
{
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_SET_CURSOR_VISIBLE, Is_Visible, 0, NULL, NULL);
}
