/** @file System_Wait.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/10/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemWait(unsigned int Periods_Count)
{
	SystemCall(SYSTEM_CALL_TIMER_WAIT, Periods_Count, 0, NULL, NULL);
}