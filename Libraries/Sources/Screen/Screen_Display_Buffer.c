/** @file Screen_Display_Buffer.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 13/12/2013
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenDisplayBuffer(unsigned char *Pointer_Buffer)
{
	SystemCall(SYSTEM_CALL_SCREEN_DISPLAY_BUFFER, 0, 0, Pointer_Buffer, 0);
}