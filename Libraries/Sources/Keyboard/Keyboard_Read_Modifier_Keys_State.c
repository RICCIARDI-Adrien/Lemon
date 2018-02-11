/** @file Keyboard_Read_Modifier_Keys_State.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int LibrariesKeyboardReadModifierKeysState(void)
{
	return LibrariesSystemCall(SYSTEM_CALL_KEYBOARD_READ_MODIFIER_KEYS_STATE, 0, 0, NULL, NULL);
}
