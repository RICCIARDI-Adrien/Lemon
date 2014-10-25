/** @file Random_Initialize.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/10/2014
 */
#include <Random.h>
#include <System.h>

extern unsigned int Random_Libraries_Seed;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void RandomInitialize(void)
{
	Random_Libraries_Seed = SystemCall(SYSTEM_CALL_TIMER_READ_VALUE, 0, 0, NULL, NULL);
}