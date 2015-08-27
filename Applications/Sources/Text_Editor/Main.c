/** @file Main.c
 * Text editor main loop.
 * @author Adrien RICCIARDI
 * @version 1.0 : 19/02/2015
 */
#include <System.h>
#include "Buffer.h"
#include "Configuration.h"
#include "Cursor.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Load a file to the internal buffer.
 * @param String_File_Name The file to load.
 */
static void MainLoadFile(char *String_File_Name)
{
	unsigned int File_ID;
	
	// Try to open the file
	if (FileOpen(String_File_Name, FILE_OPENING_MODE_READ, &File_ID) != ERROR_CODE_NO_ERROR)
	{
		ScreenSetFontColor(SCREEN_COLOR_RED);
		ScreenWriteString(STRING_ERROR_CANT_OPEN_FILE);
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		SystemExitProgram();
	}
	
	// Load file content
	if (FileRead(File_ID, Buffer, CONFIGURATION_BUFFER_MAXIMUM_SIZE, &Buffer_Characters_Count) != ERROR_CODE_NO_ERROR)
	{
		ScreenSetFontColor(SCREEN_COLOR_RED);
		ScreenWriteString(STRING_ERROR_CANT_LOAD_FILE);
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		FileClose(File_ID);
		SystemExitProgram();
	}
	
	FileClose(File_ID);
	
	// Show a message if the file is too big for the buffer
	if (FileGetSize(String_File_Name) > CONFIGURATION_BUFFER_MAXIMUM_SIZE)
	{
		ScreenSetFontColor(SCREEN_COLOR_MAGENTA);
		ScreenWriteString(STRING_FILE_IS_TOO_BIG);
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		// Wait for the user to hit a key
		KeyboardReadCharacter();
	}
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	char *String_File_Name;
	unsigned char Character; // Must be unsigned as virtual key codes use values greater than 127
	unsigned int i, Previous_Cursor_Location = 0, Cursor_Row, Cursor_Column;
	
	// Check parameters
	if (argc != 2)
	{
		ScreenWriteString(STRING_USAGE);
		return -1;
	}
	String_File_Name = argv[1];
	
	ScreenClear();
	
	// The specified file does exist
	if (FileExists(String_File_Name))
	{
		// Load it in the buffer
		MainLoadFile(String_File_Name);
		
		// Fill the screen with the first lines
		for (i = 0; i < CONFIGURATION_SCREEN_ROWS_COUNT; i++) BufferDisplayLine(i);
	}
	
	// Put the cursor on the top left location of the screen
	//CursorSetToHome();
	
	// Process the user keys
	// TODO state machine to use commands like ctrl+s to save
	while (1)
	{
		Character = KeyboardReadCharacter();
		switch (Character)
		{
			// Exit program
			case KEYBOARD_KEY_CODE_ESCAPE:
				// TODO ask save message if the buffer has been modified (MainSaveFile())
				// TODO display a new line if the cursor is not at the beginning of a line
				return 0;
				
			case KEYBOARD_KEY_CODE_ARROW_UP:
				// TODO
				break;
				
			case KEYBOARD_KEY_CODE_ARROW_DOWN:
				CursorMoveToBottom();
				break;
				
			case KEYBOARD_KEY_CODE_ARROW_LEFT:
				//if (CursorGetScreenColumn() == 0)
				break;
				
			case KEYBOARD_KEY_CODE_ARROW_RIGHT:
				
				break;
				
			// Add the character to the buffer
			default:
				BufferAddCharacter(CursorGetCharacterIndex(), (char) Character);
				
				// ++ cursor
				
				// Display the modified line and all the lines below
				
				break;
		}
		
		// Erase the cursor previous trace
		/*CursorConvertCharacterIndexToScreenLocation(Previous_Cursor_Location, &Cursor_Row, &Cursor_Column);
		ScreenSetCursorLocation(Cursor_Row, Cursor_Column);
		ScreenWriteCharacter(Buffer[Previous_Cursor_Location]);*/
		
		// Display the cursor at the new location
		ScreenSetBackgroundColor(CONFIGURATION_CURSOR_COLOR);
		ScreenSetCursorPosition(CursorGetScreenRow(), CursorGetScreenColumn());
		Character = Buffer[CursorGetCharacterIndex()];
		if (Character == '\n') Character = ' '; // Convert new line character to space to allow it to be displayed
		ScreenWriteCharacter(Character);
	}
}