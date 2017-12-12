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
	SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_GREEN);
	ScreenWriteString("Test successful\n\n");
	SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_BLUE);
}

void DisplayMessageTestFailed(void)
{
	SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_RED);
	ScreenWriteString("Test failed\n");
	SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_BLUE);
	SystemExitProgram();
}

void DisplayMessageErrorAndCode(char *String_Message, int Error_Code)
{
	char *String_Unknown_Error_Code = "Unknown error code", *String_Error_Code;
	
	// Display the user message
	SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_RED);
	ScreenWriteString("Error "); // User must provide the message
	ScreenWriteString(String_Message);
	ScreenWriteString(" : ");
	
	// Select the corresponding error string (if the error code is known)
	if ((unsigned int) Error_Code >= SYSTEM_GET_ARRAY_ELEMENTS_COUNT(String_Error_Codes)) String_Error_Code = String_Unknown_Error_Code; // SYSTEM_GET_ARRAY_ELEMENTS_COUNT() returns an unsigned value
	else String_Error_Code = String_Error_Codes[Error_Code];
	
	// Display the error code signification
	ScreenWriteString(String_Error_Code);
	ScreenWriteCharacter('\n');
	SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_BLUE);
}

void DisplayMessageError(char *String_Message)
{
	SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_RED);
	ScreenWriteString("Error : ");
	ScreenWriteString(String_Message);
	ScreenWriteCharacter('\n');
	SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_BLUE);
}