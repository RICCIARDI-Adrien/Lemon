/** @file Random_Initialize.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
extern unsigned int Libraries_Random_Libraries_Seed;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesRandomInitialize(void)
{
	Libraries_Random_Libraries_Seed = LibrariesSystemCall(SYSTEM_CALL_TIMER_READ_VALUE, 0, 0, NULL, NULL);
}
