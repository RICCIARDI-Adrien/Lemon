/** @file System_Get_Timer_Value.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 11/01/2015
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int SystemGetTimerValue(void)
{
	return (unsigned int) SystemCall(SYSTEM_CALL_TIMER_READ_VALUE, 0, 0, NULL, NULL);
}