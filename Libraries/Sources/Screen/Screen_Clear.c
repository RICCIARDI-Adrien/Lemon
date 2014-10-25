/** @file Screen_Clear.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 13/12/2013
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenClear(void)
{
	SystemCall(SYSTEM_CALL_SCREEN_CLEAR, 0, 0, NULL, NULL);
}