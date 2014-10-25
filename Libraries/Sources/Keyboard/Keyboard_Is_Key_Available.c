/** @file Keyboard_Is_Key_Available.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 21/10/2012
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int KeyboardIsKeyAvailable(void)
{
	return SystemCall(SYSTEM_CALL_KEYBOARD_IS_KEY_AVAILABLE, 0, 0, NULL, NULL);
}