/** @file Libraries_Screen_Set_Background_Color.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesScreenSetBackgroundColor(TLibrariesScreenColor Color_Code)
{
	unsigned char Color;
	
	// Get current color
	Color = LibrariesSystemCall(SYSTEM_CALL_SCREEN_GET_COLOR, 0, 0, NULL, NULL);
	// Preserve font color and set new background color
	Color = (Color & 0x0F) | (Color_Code << 4);
	// Set new color
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_SET_COLOR, Color, 0, NULL, NULL);
}
