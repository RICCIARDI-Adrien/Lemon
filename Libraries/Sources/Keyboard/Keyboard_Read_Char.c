/** @file Keyboard_Read_Char.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 19/07/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int KeyboardReadChar(void)
{
	return SystemCall(SYSTEM_CALL_KEYBOARD_READ_CHARACTER, 0, 0, NULL, NULL);
}