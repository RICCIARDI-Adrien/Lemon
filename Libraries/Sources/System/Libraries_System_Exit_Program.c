/** @file Libraries_System_Exit_Program.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesSystemExitProgram(void)
{
	LibrariesSystemCall(SYSTEM_CALL_SYSTEM_EXIT_PROGRAM, 0, 0, NULL, NULL);
}
