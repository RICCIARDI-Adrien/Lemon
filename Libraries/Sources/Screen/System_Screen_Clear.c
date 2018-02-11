/** @file Libraries_Screen_Clear.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesScreenClear(void)
{
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_CLEAR, 0, 0, NULL, NULL);
}
