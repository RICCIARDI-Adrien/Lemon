/** @file System_Keyboard_Read_Character.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int SystemKeyboardReadCharacter(void)
{
	return LibrariesSystemCall(SYSTEM_CALL_KEYBOARD_READ_CHARACTER, 0, 0, NULL, NULL);
}
