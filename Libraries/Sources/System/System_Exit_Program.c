/** @file System_Exit_Program.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemExitProgram(void)
{
	SystemCall(SYSTEM_CALL_SYSTEM_EXIT_PROGRAM, 0, 0, NULL, NULL);
}