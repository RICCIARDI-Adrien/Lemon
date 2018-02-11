/** @file Display_Message.c
 * @see Display_Message.h for description.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
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
	LibrariesScreenWriteString("--- Testing ");
	LibrariesScreenWriteString(String_Message);
	LibrariesScreenWriteString(" ---\n");
}

void DisplayMessageTestSuccessful(void)
{
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
	LibrariesScreenWriteString("Test successful\n\n");
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
}

void DisplayMessageTestFailed(void)
{
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
	LibrariesScreenWriteString("Test failed\n");
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
	LibrariesSystemExitProgram();
}

void DisplayMessageErrorAndCode(char *String_Message, int Error_Code)
{
	char *String_Unknown_Error_Code = "Unknown error code", *String_Error_Code;
	
	// Display the user message
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
	LibrariesScreenWriteString("Error "); // User must provide the message
	LibrariesScreenWriteString(String_Message);
	LibrariesScreenWriteString(" : ");
	
	// Select the corresponding error string (if the error code is known)
	if ((unsigned int) Error_Code >= LIBRARIES_GET_ARRAY_ELEMENTS_COUNT(String_Error_Codes)) String_Error_Code = String_Unknown_Error_Code; // LIBRARIES_GET_ARRAY_ELEMENTS_COUNT() returns an unsigned value
	else String_Error_Code = String_Error_Codes[Error_Code];
	
	// Display the error code signification
	LibrariesScreenWriteString(String_Error_Code);
	LibrariesScreenWriteCharacter('\n');
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
}

void DisplayMessageError(char *String_Message)
{
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
	LibrariesScreenWriteString("Error : ");
	LibrariesScreenWriteString(String_Message);
	LibrariesScreenWriteCharacter('\n');
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
}
