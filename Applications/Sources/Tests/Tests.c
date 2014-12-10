/** @file Tests.c
 * Contain all automated tests that can be done on the system.
 * @author Adrien RICCIARDI
 * @version 1.0 : 20/05/2014
 * @version 1.1 : 24/05/2014, added FileRead() / FileWrite() test.
 * @version 1.2 : 11/06/2014, added ctype tests.
 * @version 1.3 : 12/06/2014, added assert() tests.
 * @version 1.4 : 07/12/2014, removed libc stuff to keep only Lemon-specific tests.
 * @version 1.5 : 09/12/2014, multiple optimizations.
 */
#include <System.h>
#include "Display_Message.h"
#include "Tests.h"

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A test function. */
typedef int (*TTestFunction)(void);

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All the tests. */
static TTestFunction Test_Functions[] =
{
	TestsFile,
	TestsMemory,
	NULL
};

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	int i = 0;
	
	RandomInitialize();
	
	ScreenWriteString("### Automated tests ###\n\n");
	
	while (Test_Functions[i] != NULL)
	{
		if (Test_Functions[i]() != 0)
		{
			DisplayMessageTestFailed();
			break;
		}
		i++;
	}

	return 0;
}
