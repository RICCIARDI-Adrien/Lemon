/** @file System_Screen_Display_Buffer.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemScreenDisplayBuffer(void *Pointer_Buffer)
{
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_DISPLAY_BUFFER, 0, 0, Pointer_Buffer, 0);
}
