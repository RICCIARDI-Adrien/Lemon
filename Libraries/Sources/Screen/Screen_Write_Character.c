/** @file Screen_Write_Character.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/10/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenWriteCharacter(char Character)
{
	SystemCall(SYSTEM_CALL_SCREEN_WRITE_CHARACTER, Character, 0, NULL, NULL);
}