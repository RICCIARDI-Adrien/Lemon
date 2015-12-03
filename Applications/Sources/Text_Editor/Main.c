/** @file Main.c
 * Text editor main loop.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Buffer.h"
#include "Configuration.h"
#include "Cursor.h"
#include "Display.h"
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

/** Erase the cursor trace.
 * @warning This function must be called BEFORE updating the cursor.
 */
static void MainCursorEraseTrace(void)
{
	unsigned int Cursor_Display_Row, Cursor_Display_Column;
	char Character;
	
	// Cache values
	Cursor_Display_Row = CursorGetDisplayRow();
	Cursor_Display_Column = CursorGetDisplayColumn();
	
	// Avoid displaying non-printable characters
	Character = Buffer[CursorGetBufferCharacterIndex()];
	if ((Character >= 0) && (Character < ' ')) Character = ' '; // Must compare with zero because special characters like stressed letters have an ASCII code greater than 127, which become negative due to the signed char value
	
	// Display the character at the cursor position in the normal background color
	DisplaySetCursorPosition(Cursor_Display_Row, Cursor_Display_Column);
	DisplayWriteCharacter(Character);
	DisplaySetCursorPosition(Cursor_Display_Row, Cursor_Display_Column); // Reset the cursor location after it was updated by DisplayWriteCharacter()
}

/** Add the cursor trace on the display. */
static void MainCursorDisplay(void)
{
	unsigned int Cursor_Display_Row, Cursor_Display_Column;
	char Character;
	
	// Cache values
	Cursor_Display_Row = CursorGetDisplayRow();
	Cursor_Display_Column = CursorGetDisplayColumn();
	
	// Avoid displaying non-printable characters
	Character = Buffer[CursorGetBufferCharacterIndex()];
	if ((Character >= 0) && (Character < ' ')) Character = ' '; // Must compare with zero because special characters like stressed letters have an ASCII code greater than 127, which become negative due to the signed char value
	
	// Display the character at the cursor position with a green background
	DisplaySetBackgroundColor(CONFIGURATION_CURSOR_BACKGROUND_COLOR);
	DisplaySetCursorPosition(Cursor_Display_Row, Cursor_Display_Column);
	DisplayWriteCharacter(Character);
	
	// Reset the background color and cursor location
	DisplaySetBackgroundColor(CONFIGURATION_TEXT_BACKGROUND_COLOR);
	DisplaySetCursorPosition(Cursor_Display_Row, Cursor_Display_Column);
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	char *String_File_Name;
	unsigned char Character; // Must be unsigned as virtual key codes use values greater than 127
	unsigned int i, Temp;
	
	// Check parameters
	if (argc != 2)
	{
		ScreenWriteString(STRING_USAGE);
		return -1;
	}
	String_File_Name = argv[1];
	
	ScreenClear();
	DisplayClear(); // Prepare the display with a blank screen in case it's a new file
	
	// The specified file does exist
	if (FileExists(String_File_Name))
	{
		// Load it in the buffer
		MainLoadFile(String_File_Name);
		
		// Fill the screen with the first lines
		BufferDisplayPage(0);
	}
	
	// Display the cursor on the upper left corner
	MainCursorDisplay();
	DisplayRenderToScreen();
	
	// Process the user keys
	// TODO state machine to use commands like ctrl+s to save
	while (1)
	{
		Character = KeyboardReadCharacter();
		
		// Always erase the cursor trace because the display may not be cleared
		MainCursorEraseTrace();
		
		switch (Character)
		{
			// Exit program
			case KEYBOARD_KEY_CODE_ESCAPE:
				// TODO ask save message if the buffer has been modified (MainSaveFile())
				// TODO display a new line if the cursor is not at the beginning of a line
				ScreenClear();
				return 0;
				
			case KEYBOARD_KEY_CODE_ARROW_UP:
				if (CursorMoveToUp()) BufferDisplayPage(CursorGetBufferRow());
				break;
				
			case KEYBOARD_KEY_CODE_ARROW_DOWN:
				if (CursorMoveToDown())
				{
					Temp = CursorGetBufferRow();
					if (Temp < CONFIGURATION_DISPLAY_ROWS_COUNT) Temp = 0;
					else Temp = Temp - (CONFIGURATION_DISPLAY_ROWS_COUNT - 1); // Start displaying one line after the current first one
					BufferDisplayPage(Temp);
				}
				break;
				
			case KEYBOARD_KEY_CODE_ARROW_LEFT:
				if (CursorMoveToLeft()) BufferDisplayPage(CursorGetBufferRow());
				break;
				
			case KEYBOARD_KEY_CODE_ARROW_RIGHT:
				if (CursorMoveToRight())
				{
					Temp = CursorGetBufferRow();
					if (Temp < CONFIGURATION_DISPLAY_ROWS_COUNT) Temp = 0;
					else Temp = Temp - (CONFIGURATION_DISPLAY_ROWS_COUNT - 1); // Start displaying one line after the current first one
					BufferDisplayPage(Temp);
				}
				break;
				
			case KEYBOARD_KEY_CODE_ORIGIN:
				CursorGoToLineBeginning();
				break;
				
			case KEYBOARD_KEY_CODE_END:
				CursorGoToLineEnd();
				break;
				
			// TODO : ctrl state machine
			// TODO : ctrl + s
			// TODO : ctrl + c
			// TODO : ctrl + v
			// TODO : ctrl + x
			// TODO : page up
			// TODO : page down
			// TODO : insert
				
			case KEYBOARD_KEY_CODE_BACKSPACE:
			case KEYBOARD_KEY_CODE_DELETE:
				// Choose the character to remove according to the pressed key (delete removes the current character whereas backspace removes the previous character)
				Temp = CursorGetBufferCharacterIndex();
				if (Character == KEYBOARD_KEY_CODE_BACKSPACE)
				{
					Temp--; // Remove the previous character
					CursorMoveToLeft(); // Update the cursor location before changing the buffer content to avoid the cursor going to the end of the newly created upper line (which can be longer than the previous line was)
				}
				
				// Remove the character
				BufferRemoveCharacter(Temp);

				// The only characters that can make the scrolling go upper are backspace and delete, in all other cases the buffer must be redrawn from where it was displayed
				BufferDisplayPage(CursorGetBufferRow() - CursorGetDisplayRow());
				
				break;
				
			// Add the character to the buffer
			default:
				// Append the character
				if (BufferAppendCharacter(CursorGetBufferCharacterIndex(), (char) Character) != 0) break; // Nothing to do if the characters could not be added (TODO : error message if the buffer is full)
				
				// Update the cursor location
				CursorMoveToRight();
				
				// The only characters that can make the scrolling go upper are backspace and delete, in all other cases the buffer must be redrawn from where it was displayed
				BufferDisplayPage(CursorGetBufferRow() - CursorGetDisplayRow());
				break;
		}
		
		// Add the cursor trace and show on screen
		MainCursorDisplay();
		DisplayRenderToScreen();
	}
}