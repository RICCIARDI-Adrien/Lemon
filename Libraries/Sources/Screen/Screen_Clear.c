/** @file Screen_Clear.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenClear(void)
{
	SystemCall(SYSTEM_CALL_SCREEN_CLEAR, 0, 0, NULL, NULL);
}