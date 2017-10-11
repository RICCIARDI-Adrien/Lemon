/** @file System_Keyboard_Read_Modifier_Keys_State.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int SystemKeyboardReadModifierKeysState(void)
{
	return SystemCall(SYSTEM_CALL_KEYBOARD_READ_MODIFIER_KEYS_STATE, 0, 0, NULL, NULL);
}
