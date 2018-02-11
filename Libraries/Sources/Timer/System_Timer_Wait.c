/** @file System_Timer_Wait.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemTimerWait(unsigned int Milliseconds_Count)
{
	LibrariesSystemCall(SYSTEM_CALL_TIMER_WAIT, Milliseconds_Count, 0, NULL, NULL);
}
