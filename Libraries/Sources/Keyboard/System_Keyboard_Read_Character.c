/** @file Libraries_Keyboard_Read_Character.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesKeyboardReadCharacter(void)
{
	return LibrariesSystemCall(SYSTEM_CALL_KEYBOARD_READ_CHARACTER, 0, 0, NULL, NULL);
}
