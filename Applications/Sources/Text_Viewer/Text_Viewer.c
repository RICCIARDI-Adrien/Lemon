/** @file Text_Viewer.c
 * @see Text_Viewer.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Text_Viewer.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** The End Of Line arbitrary character. */
#define TEXT_VIEWER_END_OF_LINE_CHARACTER 1 // Smiley character, so it's easy to find it if it is displayed

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
static char Buffer[TEXT_VIEWER_BUFFER_SIZE_BYTES];
static unsigned int Current_Offset, File_Size_Bytes, Line_Number = 0;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Try to load the whole file in memory and add new line markers where needed.
 * @param String_File_Name Name of the file to load.
 * @return 0 if the file was successfully loaded,
 * @return 1 if the file was not found,
 * @return 2 if the file is too big to fit in memory,
 * @return 3 if an error occurred when reading the file.
 */
static int LoadFile(char *String_File_Name)
{
	unsigned int File_ID, i, Characters_Count = 0, Read_Bytes_Count;
	char Character;
	
	// Open the file
	if (FileOpen(String_File_Name, FILE_OPENING_MODE_READ, &File_ID) != ERROR_CODE_NO_ERROR) return 1;
	
	// Get file size
	File_Size_Bytes = FileGetSize(String_File_Name);
	
	// Load file line after line
	Current_Offset = 0;
	for (i = 0; i < File_Size_Bytes; i++)
	{
		// Is there enough room in buffer to store the character ?
		if (Current_Offset >= TEXT_VIEWER_BUFFER_SIZE_BYTES)
		{
			FileClose(File_ID);
			return 2;
		}
		
		// Read a single character
		if (FileRead(File_ID, &Character, 1, &Read_Bytes_Count) != ERROR_CODE_NO_ERROR)
		{
			FileClose(File_ID);
			return 3;
		}
		Characters_Count++;
		
		// Translate the character if needed
		switch (Character)
		{
			// Tabulation character, replace with 2 spaces
			case '\t':
				Character = ' ';
				Buffer[Current_Offset] = ' '; // Add the first space
				Current_Offset++;
				break;
		};
		
		// Add the character to the buffer
		Buffer[Current_Offset] = Character;
		Current_Offset++;
		
		// Reset the line characters counter if an end of line has been reached
		if (Character == '\n') Characters_Count = 0;
		
		// End of line is reached when the line is longer than a screen width
		if (Characters_Count == TEXT_VIEWER_SCREEN_WIDTH)
		{
			// Add an End Of Line marker to ease reverse line parsing
			Buffer[Current_Offset] = TEXT_VIEWER_END_OF_LINE_CHARACTER;
			Current_Offset++;
			
			Characters_Count = 0;
		}
	}

	// Adjust the file size to take into account the added end of line characters
	File_Size_Bytes = Current_Offset;
	Current_Offset = 0; // Reset buffer offset to display the text from the beginning
	
	FileClose(File_ID);
	return 0;
}

/** Go to the beginning of the previous line. */
static void MoveToPreviousLine(void)
{
	char Character;

	// Bypass last new line character of the previous line if it exists
	if ((Current_Offset >= 2) && (Buffer[Current_Offset - 1] == '\n')) Current_Offset--;

	// Go to previous End Of Line marker or previous new line character
	while (Current_Offset > 0)
	{
		Current_Offset--;
		Character = Buffer[Current_Offset];

		// Put the offset just after the new line character
		if (Character == '\n')
		{
			Current_Offset++;
			break;
		}

		// Stop when an end of line character is reached
		if (Character == TEXT_VIEWER_END_OF_LINE_CHARACTER) break;
	}
}

/** Display the next file line.
 * @return 0 if the last line is reached,
 * @return 1 if the last line is not reached.
 */
static int DisplayNextLine(void)
{
	unsigned int Characters_Count = 0;
	char Character;

	// Show characters from the buffer only if the end of the file has not been reached, the end of the line has not been reached, and a new line has not been found
	while (Characters_Count < TEXT_VIEWER_SCREEN_WIDTH)
	{
		// Is end of file reached ?
		if (Current_Offset >= File_Size_Bytes) return 0;
		
		// Get the next character to display
		Character = Buffer[Current_Offset];
		Current_Offset++;
			
		switch (Character)
		{
			// Exit if a new line has been found
			case '\n':
				ScreenWriteCharacter('\n');
				if (Current_Offset >= File_Size_Bytes) return 0;
				return 1;
				
			// Ignore end of line markers has they are located after the SCREEN_WIDTH'th character, so they will appear at the beginning of the next line
			case TEXT_VIEWER_END_OF_LINE_CHARACTER:
				break;
			
			// Show the character
			default:
				ScreenWriteCharacter(Character);
				Characters_Count++;
				break;
		}
	}
	
	if (Current_Offset >= File_Size_Bytes) return 0;
	return 1;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int i;
	unsigned int Row, Column;
	char *String_File_Name;
	
	// Check parameters
	if (argc != 2)
	{
		ScreenWriteString(STRING_USAGE_1);
		ScreenWriteString(argv[0]);
		ScreenWriteString(STRING_USAGE_2);
		return -1;
	}
	String_File_Name = argv[1];

	// Load the file
	switch (LoadFile(String_File_Name))
	{
		case 1:
			ScreenWriteString(STRING_ERROR_FILE_NOT_FOUND_1);
			ScreenWriteString(String_File_Name);
			ScreenWriteString(STRING_ERROR_FILE_NOT_FOUND_2);
			return -1;
			
		case 2:
			ScreenWriteString(STRING_ERROR_FILE_TOO_BIG_1);
			ScreenWriteUnsignedInteger(sizeof(Buffer));
			ScreenWriteString(STRING_ERROR_FILE_TOO_BIG_2);
			return -1;
			
		case 3:
			ScreenWriteString(STRING_ERROR_READ_FAILED);
			return -1;
	}
	
	// Display the first lines
	for (i = 0; i < TEXT_VIEWER_SCREEN_HEIGHT - 1; i++) DisplayNextLine();

	// Display text
	while (1)
	{
		switch (KeyboardReadCharacter())
		{
			// Display next line
			case KEYBOARD_KEY_CODE_ARROW_DOWN:
				if (DisplayNextLine()) Line_Number++;
				break;
			
			case KEYBOARD_KEY_CODE_ARROW_UP:
				// Go up only if the top of the screen is not reached (to avoid flickering effects)
				if (Line_Number > 0)
				{
					ScreenClear();
				
					// Go to the line before the first which is currently displayed
					for (i = 0; i < TEXT_VIEWER_SCREEN_HEIGHT; i++) MoveToPreviousLine();
					
					// Display this line and the following
					for (i = 0; i < TEXT_VIEWER_SCREEN_HEIGHT - 1; i++) DisplayNextLine();
					
					Line_Number--;
				}
				break;
			
			// Exit program
			case KEYBOARD_KEY_CODE_ESCAPE:
				// Go to next line if needed
				ScreenGetCursorPosition(&Row, &Column);
				if (Column > 0) ScreenWriteCharacter('\n');
				return 0;
		}
	}
}