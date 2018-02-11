/** @file System_Timer_Get_Value.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int SystemTimerGetValue(void)
{
	return (unsigned int) LibrariesSystemCall(SYSTEM_CALL_TIMER_READ_VALUE, 0, 0, NULL, NULL);
}
