/** @file Tests.c
 * Contain all automated tests that can be done on the system.
 * @author Adrien RICCIARDI
 * @version 1.0 : 20/05/2014
 * @version 1.1 : 24/05/2014, added FileRead() / FileWrite() test.
 * @version 1.2 : 11/06/2014, added ctype tests.
 * @version 1.3 : 12/06/2014, added assert() tests.
 * @version 1.4 : 07/12/2014, removed libc stuff to keep only Lemon-specific tests.
 */
#include <System.h>
#include "Tests.h"

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

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Display the next test to do.
 * @param String_Message The message to display.
 */
static void MessageStartingTest(char *String_Message)
{
	ScreenWriteString("--- Testing ");
	ScreenWriteString(String_Message);
	ScreenWriteString(" ---\n");
}

/** Display a success message. */
static void MessageTestSuccessful(void)
{
	ScreenSetFontColor(SCREEN_COLOR_GREEN);
	ScreenWriteString("Test successful\n\n");
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
}

/** Halt the program after displaying an error message. */
static void MessageTestFailed(void)
{
	ScreenSetFontColor(SCREEN_COLOR_RED);
	ScreenWriteString("Test failed\n");
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
	SystemExitProgram();
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
char *TestsGetErrorString(int Error_Code)
{
	return String_Error_Codes[Error_Code];
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	RandomInitialize();
	
	ScreenWriteString("### Automated tests ###\n\n");
	
	MessageStartingTest("Libraries File functions");
	if (TestsFile()) MessageTestSuccessful();
	else MessageTestFailed();

	MessageStartingTest("Libraries Memory functions");
	if (TestsMemory() == 0) MessageTestSuccessful();
	else MessageTestFailed();

	return 0;
}
