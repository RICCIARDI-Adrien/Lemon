/** @file Libraries_String_Is_Character_A_Digit.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesStringIsCharacterADigit(char Character)
{
	if ((Character >= '0') && (Character <= '9')) return 1;
	return 0;
}
