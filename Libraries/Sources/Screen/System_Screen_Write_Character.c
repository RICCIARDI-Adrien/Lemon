/** @file System_Screen_Write_Character.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemScreenWriteCharacter(char Character)
{
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_WRITE_CHARACTER, Character, 0, NULL, NULL);
}
