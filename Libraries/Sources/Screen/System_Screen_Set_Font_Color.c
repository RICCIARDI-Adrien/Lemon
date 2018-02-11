/** @file System_Screen_Set_Font_Color.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemScreenSetFontColor(TSystemScreenColor Color_Code)
{
	unsigned char Color;
	
	// Get current color
	Color = LibrariesSystemCall(SYSTEM_CALL_SCREEN_GET_COLOR, 0, 0, NULL, NULL);
	// Preserve background color and set new font color
	Color = (Color & 0xF0) | Color_Code;
	// Set new color
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_SET_COLOR, Color, 0, NULL, NULL);
}
