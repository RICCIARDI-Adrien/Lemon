/** @file Interface.c
 * @see Interface.h for description.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
#include "Interface.h"

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void InterfaceDisplayMessage(char *String_Message, TLibrariesScreenColor Message_Color)
{
	// Force the screen to scroll
	LibrariesScreenSetCursorPosition(LIBRARIES_SCREEN_ROWS_COUNT - 1, LIBRARIES_SCREEN_COLUMNS_COUNT - 1);
	LibrariesScreenWriteCharacter('\n');
	
	// Display the message at the screen end
	LibrariesScreenSetCursorPosition(LIBRARIES_SCREEN_ROWS_COUNT - 1, 0);
	LibrariesScreenSetFontColor(Message_Color);
	LibrariesScreenWriteString(String_Message);
}

void InterfaceDisplayUserMessage(char *String_Message)
{
	unsigned int i, Cursor_Row, Cursor_Column;
	
	// Display the prompt
	LibrariesScreenSetCursorPosition(0, 0);
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
	LibrariesScreenWriteString("> ");
	
	// Display the current written message
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
	for (i = 0; (String_Message[i] != 0) && (i < INTERFACE_USER_MESSAGE_MAXIMUM_SIZE); i++) LibrariesScreenWriteCharacter(String_Message[i]);
	
	// Clear the last user message line up to its end
	LibrariesScreenGetCursorPosition(&Cursor_Row, &Cursor_Column);
	for ( ; Cursor_Column < LIBRARIES_SCREEN_COLUMNS_COUNT; Cursor_Column++) LibrariesScreenWriteCharacter(' ');
}