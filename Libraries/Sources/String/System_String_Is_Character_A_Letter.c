/** @file Libraries_String_Is_Character_A_Letter.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesStringIsCharacterALetter(char Character)
{
	if (((Character >= 'a') && (Character <= 'z')) || ((Character >= 'A') && (Character <= 'Z'))) return 1;
	return 0;
}
