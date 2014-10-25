/** @file Keyboard_Read_Character.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 19/07/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int KeyboardReadCharacter(void)
{
	return SystemCall(SYSTEM_CALL_KEYBOARD_READ_CHARACTER, 0, 0, NULL, NULL);
}