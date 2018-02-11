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
	SystemScreenWriteString("--- Testing ");
	SystemScreenWriteString(String_Message);
	SystemScreenWriteString(" ---\n");
}

void DisplayMessageTestSuccessful(void)
{
	SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
	SystemScreenWriteString("Test successful\n\n");
	SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
}

void DisplayMessageTestFailed(void)
{
	SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
	SystemScreenWriteString("Test failed\n");
	SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
	LibrariesSystemExitProgram();
}

void DisplayMessageErrorAndCode(char *String_Message, int Error_Code)
{
	char *String_Unknown_Error_Code = "Unknown error code", *String_Error_Code;
	
	// Display the user message
	SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
	SystemScreenWriteString("Error "); // User must provide the message
	SystemScreenWriteString(String_Message);
	SystemScreenWriteString(" : ");
	
	// Select the corresponding error string (if the error code is known)
	if ((unsigned int) Error_Code >= LIBRARIES_GET_ARRAY_ELEMENTS_COUNT(String_Error_Codes)) String_Error_Code = String_Unknown_Error_Code; // LIBRARIES_GET_ARRAY_ELEMENTS_COUNT() returns an unsigned value
	else String_Error_Code = String_Error_Codes[Error_Code];
	
	// Display the error code signification
	SystemScreenWriteString(String_Error_Code);
	SystemScreenWriteCharacter('\n');
	SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
}

void DisplayMessageError(char *String_Message)
{
	SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
	SystemScreenWriteString("Error : ");
	SystemScreenWriteString(String_Message);
	SystemScreenWriteCharacter('\n');
	SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
}