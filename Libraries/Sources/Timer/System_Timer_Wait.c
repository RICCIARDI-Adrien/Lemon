/** @file Libraries_Timer_Wait.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesTimerWait(unsigned int Milliseconds_Count)
{
	LibrariesSystemCall(SYSTEM_CALL_TIMER_WAIT, Milliseconds_Count, 0, NULL, NULL);
}
