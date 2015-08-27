/** @file Keyboard_Is_Key_Available.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int KeyboardIsKeyAvailable(void)
{
	return SystemCall(SYSTEM_CALL_KEYBOARD_IS_KEY_AVAILABLE, 0, 0, NULL, NULL);
}