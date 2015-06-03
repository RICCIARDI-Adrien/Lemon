/** @file String_Is_Character_A_Letter.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 28/05/2015
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