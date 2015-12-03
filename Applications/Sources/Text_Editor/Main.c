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
/** Display a full screen message.
 * @param String_Message_Title The message title. It will be automatically centered.
 * @param String_Message_Content The message content.
 * @param Message_Content_Color The message content color.
 */
static void MainDisplayMessage(char *String_Message_Title, char *String_Message_Content, TScreenColor Message_Content_Color)
{
	ScreenClear();
	
	// Display the title
	ScreenWriteCenteredString(String_Message_Title);
	ScreenWriteString("\n\n");
	
	// Display the message content with the specified color
	ScreenSetFontColor(Message_Content_Color);
	ScreenWriteString(String_Message_Content);
	ScreenSetFontColor(CONFIGURATION_TEXT_FOREGROUND_COLOR); // Restore the default color
	
	// Display the prompt
	ScreenSetCursorPosition(SCREEN_ROWS_COUNT - 1, 0);
	ScreenWriteString(STRING_HIT_ENTER_TO_CONTINUE);
	
	// Wait for the Enter key to be pressed
	while (KeyboardReadCharacter() != KEYBOARD_KEY_CODE_ENTER);
	
	// Restore the previously displayed buffer
	DisplayRenderToScreen();
}

/** Load a file to the internal buffer.
 * @param String_File_Name The file to load.
 * @return 0 if the file was successfully loaded,
 * @return 1 if an error occurred.
 */
static int MainLoadFile(char *String_File_Name)
{
	unsigned int File_ID;
	
	// Try to open the file
	if (FileOpen(String_File_Name, FILE_OPENING_MODE_READ, &File_ID) != ERROR_CODE_NO_ERROR)
	{
		MainDisplayMessage(STRING_MESSAGE_TITLE_ERROR, STRING_ERROR_CANT_OPEN_FILE, SCREEN_COLOR_RED);
		return 1;
	}
	
	// Load file content
	if (FileRead(File_ID, Buffer, CONFIGURATION_BUFFER_MAXIMUM_SIZE, &Buffer_Characters_Count) != ERROR_CODE_NO_ERROR)
	{
		FileClose(File_ID);
		MainDisplayMessage(STRING_MESSAGE_TITLE_ERROR, STRING_ERROR_CANT_LOAD_FILE, SCREEN_COLOR_RED);
		return 1;
	}
	
	FileClose(File_ID);
	
	// Show a message if the file is too big for the buffer
	if (FileGetSize(String_File_Name) > CONFIGURATION_BUFFER_MAXIMUM_SIZE) MainDisplayMessage(STRING_MESSAGE_TITLE_WARNING, STRING_WARNING_FILE_IS_TOO_BIG, SCREEN_COLOR_BROWN);
	
	return 0;
}

/** Save the internal buffer to a file.
 * @param String_File_Name The file to save to.
 * @return 0 if the file was successfully saved,
 * @return 1 if an error occurred.
 */
static int MainSaveFile(char *String_File_Name)
{
	unsigned int File_ID;
	
	// Try to open the file
	if (FileOpen(String_File_Name, FILE_OPENING_MODE_WRITE, &File_ID) != ERROR_CODE_NO_ERROR)
	{
		MainDisplayMessage(STRING_MESSAGE_TITLE_ERROR, STRING_ERROR_CANT_OPEN_FILE, SCREEN_COLOR_RED);
		return 1;
	}
	
	// Write file content
	if (FileWrite(File_ID, Buffer, Buffer_Characters_Count) != ERROR_CODE_NO_ERROR)
	{
		FileClose(File_ID);
		MainDisplayMessage(STRING_MESSAGE_TITLE_ERROR, STRING_ERROR_CANT_SAVE_FILE, SCREEN_COLOR_RED);
		return 1;
	}
	
	FileClose(File_ID);
	
	return 0;
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

#if 0
// TODO should be cleaner to light the main loop with this function
/** Handle the Control key state machine.
 * @param Character The character received right after the Control one.
 */
static inline void MainHandleControlKeyStateMachine(unsigned char Character)
{
	
}
#endif

/** Gracefully exit to system. */
static void MainExitProgram(void)
{
	ScreenClear();
	SystemExitProgram();
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	char *String_File_Name;
	unsigned char Character; // Must be unsigned as virtual key codes use values greater than 127
	unsigned int Temp;
	int Is_Control_Key_Detected = 0;
	
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
		if (MainLoadFile(String_File_Name) != 0) MainExitProgram();
		
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
				MainExitProgram();
				break; // To make the compiler happy
				
			case KEYBOARD_KEY_CODE_ARROW_UP:
				if (CursorMoveToUp()) BufferDisplayPage(CursorGetBufferRow());
				Is_Control_Key_Detected = 0;
				break;
				
			case KEYBOARD_KEY_CODE_ARROW_DOWN:
				if (CursorMoveToDown())
				{
					Temp = CursorGetBufferRow();
					if (Temp < CONFIGURATION_DISPLAY_ROWS_COUNT) Temp = 0;
					else Temp = Temp - (CONFIGURATION_DISPLAY_ROWS_COUNT - 1); // Start displaying one line after the current first one
					BufferDisplayPage(Temp);
				}
				Is_Control_Key_Detected = 0;
				break;
				
			case KEYBOARD_KEY_CODE_ARROW_LEFT:
				if (CursorMoveToLeft()) BufferDisplayPage(CursorGetBufferRow());
				Is_Control_Key_Detected = 0;
				break;
				
			case KEYBOARD_KEY_CODE_ARROW_RIGHT:
				if (CursorMoveToRight())
				{
					Temp = CursorGetBufferRow();
					if (Temp < CONFIGURATION_DISPLAY_ROWS_COUNT) Temp = 0;
					else Temp = Temp - (CONFIGURATION_DISPLAY_ROWS_COUNT - 1); // Start displaying one line after the current first one
					BufferDisplayPage(Temp);
				}
				Is_Control_Key_Detected = 0;
				break;
				
			case KEYBOARD_KEY_CODE_ORIGIN:
				CursorGoToLineBeginning();
				Is_Control_Key_Detected = 0;
				break;
				
			case KEYBOARD_KEY_CODE_END:
				CursorGoToLineEnd();
				Is_Control_Key_Detected = 0;
				break;
				
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
				
				// Update the buffer and the display
				BufferRemoveCharacter(Temp); // Remove the character
				BufferDisplayPage(CursorGetBufferRow() - CursorGetDisplayRow());
				
				Is_Control_Key_Detected = 0;
				break;
				
			case KEYBOARD_KEY_CODE_CONTROL_LEFT:
			case KEYBOARD_KEY_CODE_CONTROL_RIGHT:
				Is_Control_Key_Detected = 1;
				break;
				
			// Add the character to the buffer
			default:
				// Handle the Control key state machine
				if (Is_Control_Key_Detected)
				{
					switch (Character)
					{
						// Save the file
						case 'S':
						case 's':
							if (MainSaveFile(String_File_Name) != 0) MainExitProgram();
							break;
					}
				}
				else
				{
					// Append the character
					if (BufferAppendCharacter(CursorGetBufferCharacterIndex(), (char) Character) != 0) break; // Nothing to do if the characters could not be added (TODO : error message if the buffer is full)
					
					// Update the cursor location and the display
					CursorMoveToRight();
					BufferDisplayPage(CursorGetBufferRow() - CursorGetDisplayRow());
				}
				
				Is_Control_Key_Detected = 0;
				break;
		}
		
		// Add the cursor trace and show on screen
		MainCursorDisplay();
		DisplayRenderToScreen();
	}
}