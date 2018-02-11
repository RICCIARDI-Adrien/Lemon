/** @file Interface.c
 * @see Interface.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Interface.h"

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void InterfaceDisplayMessage(char *String_Message, TSystemScreenColor Message_Color)
{
	// Force the screen to scroll
	SystemScreenSetCursorPosition(LIBRARIES_SCREEN_ROWS_COUNT - 1, LIBRARIES_SCREEN_COLUMNS_COUNT - 1);
	SystemScreenWriteCharacter('\n');
	
	// Display the message at the screen end
	SystemScreenSetCursorPosition(LIBRARIES_SCREEN_ROWS_COUNT - 1, 0);
	SystemScreenSetFontColor(Message_Color);
	SystemScreenWriteString(String_Message);
}

void InterfaceDisplayUserMessage(char *String_Message)
{
	unsigned int i, Cursor_Row, Cursor_Column;
	
	// Display the prompt
	SystemScreenSetCursorPosition(0, 0);
	SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
	SystemScreenWriteString("> ");
	
	// Display the current written message
	SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
	for (i = 0; (String_Message[i] != 0) && (i < INTERFACE_USER_MESSAGE_MAXIMUM_SIZE); i++) SystemScreenWriteCharacter(String_Message[i]);
	
	// Clear the last user message line up to its end
	SystemScreenGetCursorPosition(&Cursor_Row, &Cursor_Column);
	for ( ; Cursor_Column < LIBRARIES_SCREEN_COLUMNS_COUNT; Cursor_Column++) SystemScreenWriteCharacter(' ');
}