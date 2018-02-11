/** @file Libraries_Keyboard_Is_Key_Available.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesKeyboardIsKeyAvailable(void)
{
	return LibrariesSystemCall(SYSTEM_CALL_KEYBOARD_IS_KEY_AVAILABLE, 0, 0, NULL, NULL);
}
