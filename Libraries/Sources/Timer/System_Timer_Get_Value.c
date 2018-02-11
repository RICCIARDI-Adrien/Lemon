/** @file Libraries_Timer_Get_Value.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int LibrariesTimerGetValue(void)
{
	return (unsigned int) LibrariesSystemCall(SYSTEM_CALL_TIMER_READ_VALUE, 0, 0, NULL, NULL);
}
