/** @file System_Wait.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemWait(unsigned int Milliseconds_Count)
{
	SystemCall(SYSTEM_CALL_TIMER_WAIT, Milliseconds_Count, 0, NULL, NULL);
}