/** @file System_String_Is_Character_A_Digit.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int SystemStringIsCharacterADigit(char Character)
{
	if ((Character >= '0') && (Character <= '9')) return 1;
	return 0;
}
