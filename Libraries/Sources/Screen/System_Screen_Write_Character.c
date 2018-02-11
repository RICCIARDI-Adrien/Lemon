/** @file Libraries_Screen_Write_Character.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesScreenWriteCharacter(char Character)
{
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_WRITE_CHARACTER, Character, 0, NULL, NULL);
}
