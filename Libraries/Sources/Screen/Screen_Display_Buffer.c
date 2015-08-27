/** @file Screen_Display_Buffer.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenDisplayBuffer(unsigned char *Pointer_Buffer)
{
	SystemCall(SYSTEM_CALL_SCREEN_DISPLAY_BUFFER, 0, 0, Pointer_Buffer, 0);
}