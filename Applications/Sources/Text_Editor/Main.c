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
// Private variables
//-------------------------------------------------------------------------------------------------
/** The copied text buffer. */
static char Main_Copy_Buffer[CONFIGURATION_COPY_BUFFER_MAXIMUM_SIZE];
/** Tell if the copy buffer is empty because it was pasted or not. */
static int Main_Is_Copy_Buffer_Pasted = 1;
/** The copy buffer current length. */
static unsigned int Main_Copy_Buffer_Length = 0;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Display a full screen message.
 * @param String_Message_Title The message title. It will be automatically centered.
 * @param String_Message_Content The message content.
 * @param String_Message_Footer What to display on the screen bottom line.
 * @param Message_Title_Color The message content color.
 */
static void MainDisplayMessage(char *String_Message_Title, char *String_Message_Content, char *String_Message_Footer, TScreenColor Message_Title_Color)
{
	ScreenClear();
	
	// Display the title with the specified color
	ScreenSetFontColor(Message_Title_Color);
	ScreenWriteCenteredString(String_Message_Title);
	ScreenSetFontColor(CONFIGURATION_TEXT_FOREGROUND_COLOR); // Restore the default color
	ScreenWriteString("\n\n");
	
	// Display the message content 
	ScreenWriteString(String_Message_Content);

	// Display the footer message
	ScreenSetCursorPosition(SCREEN_ROWS_COUNT - 1, 0);
	ScreenWriteString(String_Message_Footer);
}

/** Wait for the user to press the enter key. */
static inline void __attribute__((always_inline)) MainWaitForEnterKey(void)
{
	// Wait for the Enter key to be pressed
	while (SystemKeyboardReadCharacter() != SYSTEM_KEYBOARD_KEY_CODE_ENTER);
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
	if (SystemFileOpen(String_File_Name, SYSTEM_FILE_OPENING_MODE_READ, &File_ID) != ERROR_CODE_NO_ERROR)
	{
		MainDisplayMessage(STRING_MESSAGE_TITLE_ERROR, STRING_ERROR_CANT_OPEN_FILE, STRING_HIT_ENTER_TO_CONTINUE, SCREEN_COLOR_RED);
		MainWaitForEnterKey();
		return 1;
	}
	
	// Load file content
	if (SystemFileRead(File_ID, Buffer, CONFIGURATION_BUFFER_MAXIMUM_SIZE, &Buffer_Characters_Count) != ERROR_CODE_NO_ERROR)
	{
		SystemFileClose(File_ID);
		MainDisplayMessage(STRING_MESSAGE_TITLE_ERROR, STRING_ERROR_CANT_LOAD_FILE, STRING_HIT_ENTER_TO_CONTINUE, SCREEN_COLOR_RED);
		MainWaitForEnterKey();
		return 1;
	}
	
	SystemFileClose(File_ID);
	
	// Show a message if the file is too big for the buffer
	if (SystemFileGetSize(String_File_Name) > CONFIGURATION_BUFFER_MAXIMUM_SIZE)
	{
		MainDisplayMessage(STRING_MESSAGE_TITLE_WARNING, STRING_WARNING_FILE_IS_TOO_BIG, STRING_HIT_ENTER_TO_CONTINUE, SCREEN_COLOR_BROWN);
		MainWaitForEnterKey();
	}
	
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
	if (SystemFileOpen(String_File_Name, SYSTEM_FILE_OPENING_MODE_WRITE, &File_ID) != ERROR_CODE_NO_ERROR)
	{
		MainDisplayMessage(STRING_MESSAGE_TITLE_ERROR, STRING_ERROR_CANT_OPEN_FILE, STRING_HIT_ENTER_TO_CONTINUE, SCREEN_COLOR_RED);
		MainWaitForEnterKey();
		DisplayRenderToScreen(); // Restore the previously displayed buffer
		return 1;
	}
	
	// Write file content
	if (SystemFileWrite(File_ID, Buffer, Buffer_Characters_Count) != ERROR_CODE_NO_ERROR)
	{
		SystemFileClose(File_ID);
		MainDisplayMessage(STRING_MESSAGE_TITLE_ERROR, STRING_ERROR_CANT_SAVE_FILE, STRING_HIT_ENTER_TO_CONTINUE, SCREEN_COLOR_RED);
		MainWaitForEnterKey();
		DisplayRenderToScreen(); // Restore the previously displayed buffer
		return 1;
	}
	
	SystemFileClose(File_ID);
	
	return 0;
}

/** Erase the cursor trace.
 * @warning This function must be called BEFORE updating the cursor position.
 */
static void MainCursorEraseTrace(void)
{
	unsigned int Cursor_Display_Row, Cursor_Display_Column, Character_Index;
	char Character;
	
	// Cache values
	Cursor_Display_Row = CursorGetDisplayRow();
	Cursor_Display_Column = CursorGetDisplayColumn();
	
	// Choose whether displaying the underneath character or an empty space
	Character_Index = CursorGetBufferCharacterIndex();
	if (Character_Index >= Buffer_Characters_Count) Character = ' '; // The cursor is located one character more than the buffer end (so there is no character underneath)
	else
	{
		// Avoid displaying non-printable characters
		Character = Buffer[Character_Index];
		if ((Character >= 0) && (Character < ' ')) Character = ' '; // Must compare with zero because special characters like stressed letters have an ASCII code greater than 127, which become negative due to the signed char value
	}
	
	// Display the character at the cursor position in the normal background color
	DisplaySetCursorPosition(Cursor_Display_Row, Cursor_Display_Column);
	DisplayWriteCharacter(Character);
	DisplaySetCursorPosition(Cursor_Display_Row, Cursor_Display_Column); // Reset the cursor location after it was updated by DisplayWriteCharacter()
}

/** Add the cursor trace on the display. */
static void MainCursorDisplay(void)
{
	unsigned int Cursor_Display_Row, Cursor_Display_Column, Character_Index;
	char Character;
	
	// Cache values
	Cursor_Display_Row = CursorGetDisplayRow();
	Cursor_Display_Column = CursorGetDisplayColumn();
	
	// Choose whether displaying the underneath character or an empty space
	Character_Index = CursorGetBufferCharacterIndex();
	if (Character_Index >= Buffer_Characters_Count) Character = ' '; // The cursor is located one character more than the buffer end (so there is no character underneath)
	else
	{
		// Avoid displaying non-printable characters
		Character = Buffer[Character_Index];
		if ((Character >= 0) && (Character < ' ')) Character = ' '; // Must compare with zero because special characters like stressed letters have an ASCII code greater than 127, which become negative due to the signed char value
	}
	
	// Display the character at the cursor position with a green background
	DisplaySetBackgroundColor(CONFIGURATION_CURSOR_BACKGROUND_COLOR);
	DisplaySetCursorPosition(Cursor_Display_Row, Cursor_Display_Column);
	DisplayWriteCharacter(Character);
	
	// Reset the background color and cursor location
	DisplaySetBackgroundColor(CONFIGURATION_TEXT_BACKGROUND_COLOR);
	DisplaySetCursorPosition(Cursor_Display_Row, Cursor_Display_Column);
}

/** Display text information (lines count, characters count and so on). */
static void MainDisplayTextInformation(void)
{
	char String_Content[256], String_Number[11];
	
	// Create the "lines count" string
	SystemStringCopy(STRING_MESSAGE_TEXT_INFORMATION_CONTENT_1, String_Content);
	SystemStringConvertUnsignedIntegerToString(BufferGetLinesCount(), String_Number);
	StringConcatenate(String_Content, String_Number);
	
	// Create the "characters count" string
	StringConcatenate(String_Content, STRING_MESSAGE_TEXT_INFORMATION_CONTENT_2);
	SystemStringConvertUnsignedIntegerToString(Buffer_Characters_Count, String_Number);
	StringConcatenate(String_Content, String_Number);
	
	MainDisplayMessage(STRING_MESSAGE_TEXT_INFORMATION_TITLE, String_Content, STRING_HIT_ENTER_TO_CONTINUE, SCREEN_COLOR_BLUE);
	MainWaitForEnterKey();
	DisplayRenderToScreen(); // Restore the previously displayed buffer
}

/** Cut the whole line at the current cursor location and concatenate it to the copy buffer content (start from the buffer beginning if a paste was done before). */
void MainCutCurrentLine(void)
{
	unsigned int Line_Beginning, Line_Length, i;
	
	// Find the current line properties
	i = CursorGetBufferRow(); // Recycle the 'i' variable
	// Length and line presence in the same time
	if (BufferGetLineLength(i, &Line_Length) != 0) return; // The line is not available
	// Starting index
	Line_Beginning = BufferFindLineBeginning(i);
	if (Line_Beginning == Buffer_Characters_Count) return; // This line is not available
	
	// Add the trailing '\n' to the buffer or an empty line will be displayed
	i = Line_Beginning + Line_Length; // Recycle 'i' variable
	if ((i < Buffer_Characters_Count) && (Line_Length < CONFIGURATION_DISPLAY_COLUMNS_COUNT) && (Buffer[i] == '\n')) Line_Length++;
	
	// Reset the buffer if needed (the cut text is concatenated to the copy buffer until the user pastes it)
	if (Main_Is_Copy_Buffer_Pasted)
	{
		Main_Copy_Buffer_Length = 0;
		Main_Is_Copy_Buffer_Pasted = 0;
	}
	
	// Do not cut if the line would not fit in the copy buffer
	if (Main_Copy_Buffer_Length + Line_Length >= CONFIGURATION_COPY_BUFFER_MAXIMUM_SIZE) return;
	
	// Concatenate the cut text to the buffer
	SystemMemoryCopyArea(&Buffer[Line_Beginning], &Main_Copy_Buffer[Main_Copy_Buffer_Length], Line_Length);
	Main_Copy_Buffer_Length += Line_Length;
	
	// Remove the string from the edition buffer (TODO optimize)
	for (i = 0; i < Line_Length; i++) BufferRemoveCharacter(Line_Beginning);
	
	// Refresh the display
	CursorGoToLineBeginning(); // Make sure the cursor is not at the middle of a now non-existent line TODO handle cut of last line (no more line in the buffer)
	BufferDisplayPage(CursorGetBufferRow() - CursorGetDisplayRow());
}

/** Paste the copy buffer content at the current cursor location. */
void MainPasteCopyBuffer(void)
{
	unsigned int i, Character_Insertion_Index;
	
	// Nothing to paste if the copy buffer is empty
	if (Main_Copy_Buffer_Length == 0) return;
	
	// Paste at the current cursor location (start from the buffer end to add all characters in the right order)
	Character_Insertion_Index = CursorGetBufferCharacterIndex();
	for (i = Main_Copy_Buffer_Length - 1; i != 0xFFFFFFFF; i--) BufferAppendCharacter(Character_Insertion_Index, Main_Copy_Buffer[i]); // Preserve the copy buffer size because it's content can be pasted several times
	
	// Put the cursor at the end of the pasted lines
	for (i = 0; i < Main_Copy_Buffer_Length; i++) CursorMoveToRight(); // Update the cursor when all characters have been inserted, so the cursor can safely move
	
	// Refresh the display
	BufferDisplayPage(CursorGetBufferRow() - CursorGetDisplayRow());
	
	// The copy buffer will be reset on the next cut action
	Main_Is_Copy_Buffer_Pasted = 1;
}

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
	unsigned int Temp, Modifier_Keys_State;
	int Is_Text_Modified = 0;
	
	// Check parameters
	if (argc != 2)
	{
		ScreenWriteString(STRING_USAGE);
		return -1;
	}
	String_File_Name = argv[1];
	
	ScreenClear();
	DisplayClear(); // Prepare the display with a blank screen in case it's a new file
	
	// Does the specified file exist ?
	if (SystemFileExists(String_File_Name))
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
	while (1)
	{
		Character = SystemKeyboardReadCharacter();
		Modifier_Keys_State = SystemKeyboardReadModifierKeysState();
		
		// Always erase the cursor trace because the display may not be cleared
		MainCursorEraseTrace();
		
		switch (Character)
		{
			// Exit program
			case SYSTEM_KEYBOARD_KEY_CODE_ESCAPE:
				// Ask the whether to save the document if the buffer has been modified without being saved
				if (Is_Text_Modified)
				{
					MainDisplayMessage(STRING_MESSAGE_TITLE_WARNING, STRING_MESSAGE_UNSAVED_TEXT_WARNING_CONTENT, STRING_MESSAGE_UNSAVED_TEXT_WARNING_FOOTER, SCREEN_COLOR_BROWN);
					
					// Wait for Enter or Escape key
					do
					{
						Character = SystemKeyboardReadCharacter();
					} while ((Character != SYSTEM_KEYBOARD_KEY_CODE_ENTER) && (Character != SYSTEM_KEYBOARD_KEY_CODE_ESCAPE));
					
					// Save the file if requested to
					if (Character == SYSTEM_KEYBOARD_KEY_CODE_ENTER) MainSaveFile(String_File_Name);
				}
				MainExitProgram();
				break; // To make the compiler happy
				
			case SYSTEM_KEYBOARD_KEY_CODE_ARROW_UP:
				if (CursorMoveToUp()) BufferDisplayPage(CursorGetBufferRow());
				break;
				
			case SYSTEM_KEYBOARD_KEY_CODE_ARROW_DOWN:
				if (CursorMoveToDown())
				{
					Temp = CursorGetBufferRow();
					if (Temp < CONFIGURATION_DISPLAY_ROWS_COUNT) Temp = 0;
					else Temp = Temp - (CONFIGURATION_DISPLAY_ROWS_COUNT - 1); // Start displaying one line after the current first one
					BufferDisplayPage(Temp);
				}
				break;
				
			case SYSTEM_KEYBOARD_KEY_CODE_ARROW_LEFT:
				if (CursorMoveToLeft()) BufferDisplayPage(CursorGetBufferRow());
				break;
				
			case SYSTEM_KEYBOARD_KEY_CODE_ARROW_RIGHT:
				if (CursorMoveToRight())
				{
					Temp = CursorGetBufferRow();
					if (Temp < CONFIGURATION_DISPLAY_ROWS_COUNT) Temp = 0;
					else Temp = Temp - (CONFIGURATION_DISPLAY_ROWS_COUNT - 1); // Start displaying one line after the current first one
					BufferDisplayPage(Temp);
				}
				break;
				
			case SYSTEM_KEYBOARD_KEY_CODE_ORIGIN:
				CursorGoToLineBeginning();
				break;
				
			case SYSTEM_KEYBOARD_KEY_CODE_END:
				CursorGoToLineEnd();
				break;
				
			// TODO : page up
			// TODO : page down
			// TODO : insert
				
			case SYSTEM_KEYBOARD_KEY_CODE_BACKSPACE:
			case SYSTEM_KEYBOARD_KEY_CODE_DELETE:
				if (Buffer_Characters_Count == 0) break; // Nothing to delete if the buffer is empty
				
				// Choose the character to remove according to the pressed key (delete removes the current character whereas backspace removes the previous character)
				Temp = CursorGetBufferCharacterIndex();
				if (Character == SYSTEM_KEYBOARD_KEY_CODE_BACKSPACE)
				{
					Temp--; // Remove the previous character
					CursorMoveToLeft(); // Update the cursor location before changing the buffer content to avoid the cursor going to the end of the newly created upper line (which can be longer than the previous line was)
				}
				else
				{
					// Nothing to do if there is no character to remove (i.e. the cursor is at the buffer's end)
					if (Temp >= Buffer_Characters_Count) break;
				}
				
				// Update the buffer and the display
				BufferRemoveCharacter(Temp); // Remove the character
				BufferDisplayPage(CursorGetBufferRow() - CursorGetDisplayRow());
				
				Is_Text_Modified = 1;
				break;
				
			case SYSTEM_KEYBOARD_KEY_CODE_F1:
				MainDisplayTextInformation();
				break;
				
			default:
				// Handle Control+<key> shortcuts
				if (Modifier_Keys_State & (SYSTEM_KEYBOARD_MODIFIER_KEY_BIT_MASK_LEFT_CONTROL | SYSTEM_KEYBOARD_MODIFIER_KEY_BIT_MASK_RIGHT_CONTROL))
				{
					switch (Character)
					{
						case 'S':
						case 's':
							if (MainSaveFile(String_File_Name) != 0) MainExitProgram();
							Is_Text_Modified = 0;
							break;
							
						case 'V':
						case 'v':
							MainPasteCopyBuffer();
							Is_Text_Modified = 1;
							break;
							
						case 'X':
						case 'x':
							MainCutCurrentLine();
							Is_Text_Modified = 1;
							break;
					}
				}
				// Add the character to the buffer
				else
				{
					// Discard function keys
					if ((Character >= SYSTEM_KEYBOARD_KEY_CODE_F1) && (Character <= SYSTEM_KEYBOARD_KEY_CODE_F11)) break;
					
					// Append the character
					if (BufferAppendCharacter(CursorGetBufferCharacterIndex(), (char) Character) != 0) break; // Nothing to do if the character could not be added (TODO : error message if the buffer is full)
					
					// Update the cursor location and the display
					CursorMoveToRight();
					BufferDisplayPage(CursorGetBufferRow() - CursorGetDisplayRow());
					
					Is_Text_Modified = 1;
				}
				break;
		}
		
		// Add the cursor trace and show on screen
		MainCursorDisplay();
		DisplayRenderToScreen();
	}
}
