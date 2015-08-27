/** @file System_Wait.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemWait(unsigned int Periods_Count)
{
	SystemCall(SYSTEM_CALL_TIMER_WAIT, Periods_Count, 0, NULL, NULL);
}