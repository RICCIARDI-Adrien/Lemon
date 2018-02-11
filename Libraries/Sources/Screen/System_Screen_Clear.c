/** @file System_Screen_Clear.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemScreenClear(void)
{
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_CLEAR, 0, 0, NULL, NULL);
}
