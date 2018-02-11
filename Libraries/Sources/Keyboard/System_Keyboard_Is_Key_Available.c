/** @file System_Keyboard_Is_Key_Available.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int SystemKeyboardIsKeyAvailable(void)
{
	return LibrariesSystemCall(SYSTEM_CALL_KEYBOARD_IS_KEY_AVAILABLE, 0, 0, NULL, NULL);
}
