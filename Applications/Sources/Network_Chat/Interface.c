/** @file Interface.c
 * @see Interface.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Interface.h"

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void InterfaceDisplayMessage(char *String_Message, TScreenColor Message_Color)
{
	// Force the screen to scroll
	ScreenSetCursorPosition(SCREEN_ROWS_COUNT - 1, SCREEN_COLUMNS_COUNT - 1);
	ScreenWriteCharacter('\n');
	
	// Display the message at the screen end
	ScreenSetCursorPosition(SCREEN_ROWS_COUNT - 1, 0);
	ScreenSetFontColor(Message_Color);
	ScreenWriteString(String_Message);
}

void InterfaceDisplayUserMessage(char *String_Message)
{
	unsigned int i, Cursor_Row, Cursor_Column;
	
	// Display the prompt
	ScreenSetCursorPosition(0, 0);
	ScreenSetFontColor(SCREEN_COLOR_GREEN);
	ScreenWriteString("> ");
	
	// Display the current written message
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
	for (i = 0; (String_Message[i] != 0) && (i < INTERFACE_USER_MESSAGE_MAXIMUM_SIZE); i++) ScreenWriteCharacter(String_Message[i]);
	
	// Clear the last user message line up to its end
	ScreenGetCursorPosition(&Cursor_Row, &Cursor_Column);
	for ( ; Cursor_Column < SCREEN_COLUMNS_COUNT; Cursor_Column++) ScreenWriteCharacter(' ');
}