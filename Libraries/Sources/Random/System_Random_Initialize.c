/** @file System_Random_Initialize.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
extern unsigned int System_Random_Libraries_Seed;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemRandomInitialize(void)
{
	System_Random_Libraries_Seed = SystemCall(SYSTEM_CALL_TIMER_READ_VALUE, 0, 0, NULL, NULL);
}
