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
#include "Tests.h"

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A test function. */
typedef int (*TTestFunction)(void);

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All the system errors description. */
static char *String_Error_Codes[] =
{
	"No error",
	"Files List is full",
	"Block Allocation Table is full",
	"Bad file name",
	"File not found",
	"File already existing",
	"Can't open more files",
	"File opened yet",
	"Unknown file opening mode",
	"The file is not opened",
	"Bad file opening mode",
	"Bad file descriptor (greater than the maximum number of simultaneous opened files)",
	"Bad UART parameters",
	"File to download is larger than RAM",
	"File not executable"
};

/** All the tests. */
static TTestFunction Test_Functions[] =
{
	TestsFile,
	TestsMemory,
	NULL
};

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
char *TestsGetErrorString(int Error_Code)
{
	return String_Error_Codes[Error_Code];
}

void TestsDisplayMessageTestStarting(char *String_Message)
{
	ScreenWriteString("--- Testing ");
	ScreenWriteString(String_Message);
	ScreenWriteString(" ---\n");
}

void TestsDisplayMessageTestSuccessful(void)
{
	ScreenSetFontColor(SCREEN_COLOR_GREEN);
	ScreenWriteString("Test successful\n\n");
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
}

void TestsDisplayMessageTestFailed(void)
{
	ScreenSetFontColor(SCREEN_COLOR_RED);
	ScreenWriteString("Test failed\n");
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
	SystemExitProgram();
}

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
			TestsDisplayMessageTestFailed();
			break;
		}
		i++;
	}

	return 0;
}
