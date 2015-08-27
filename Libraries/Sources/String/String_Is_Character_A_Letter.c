/** @file String_Is_Character_A_Letter.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int StringIsCharacterALetter(char Character)
{
	if (((Character >= 'a') && (Character <= 'z')) || ((Character >= 'A') && (Character <= 'Z'))) return 1;
	return 0;
}