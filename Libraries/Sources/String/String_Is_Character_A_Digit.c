/** @file String_Is_Character_A_Digit.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 28/05/2015
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int StringIsCharacterADigit(char Character)
{
	if ((Character >= '0') && (Character <= '9')) return 1;
	return 0;
}