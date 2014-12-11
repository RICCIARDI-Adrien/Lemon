/** @file Display_Message.c
 * @see Display_Message.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Display_Message.h"

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
// Public functions
//-------------------------------------------------------------------------------------------------
void DisplayMessageTestStarting(char *String_Message)
{
	ScreenWriteString("--- Testing ");
	ScreenWriteString(String_Message);
	ScreenWriteString(" ---\n");
}

void DisplayMessageTestSuccessful(void)
{
	ScreenSetFontColor(SCREEN_COLOR_GREEN);
	ScreenWriteString("Test successful\n\n");
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
}

void DisplayMessageTestFailed(void)
{
	ScreenSetFontColor(SCREEN_COLOR_RED);
	ScreenWriteString("Test failed\n");
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
	SystemExitProgram();
}

void DisplayMessageError(char *String_Message, int Error_Code)
{
	char *String_Unknown_Error_Code = "Unknown error code", *String_Error_Code;
	
	// Display the user message
	ScreenSetFontColor(SCREEN_COLOR_RED);
	ScreenWriteString("Error "); // User must provide the message
	ScreenWriteString(String_Message);
	ScreenWriteString(" : ");
	
	// Select the corresponding error string (if the error code is known)
	if ((unsigned int) Error_Code >= sizeof(String_Error_Codes) / sizeof(char *)) String_Error_Code = String_Unknown_Error_Code; // The sizeof() computation works as it is done on an array of pointers, no need to test if Error_Code < 0 because it is an unsigned comparison
	else String_Error_Code = String_Error_Codes[Error_Code];
	
	// Display the error code signification
	ScreenWriteString(String_Error_Code);
	ScreenWriteCharacter('\n');
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
}