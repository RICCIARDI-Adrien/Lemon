/** @file Command_More.c
 * A simplified version of the "more" command.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Commands.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The file descriptor. */
static unsigned int File_ID;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Display the next file line.
 * @return 1 if the end of the file is reached,
 * @return 0 if the next line has been successfully displayed,
 * @return -1 if an error occurred.
 */
static int DisplayNextLine(void)
{
	int i;
	char Character;
	unsigned int Read_Bytes_Count;
	
	// Read a whole line
	for (i = 0; i < SYSTEM_SCREEN_COLUMNS_COUNT; i++)
	{
		// Try to read a character
		if (SystemFileRead(File_ID, &Character, 1, &Read_Bytes_Count) != ERROR_CODE_NO_ERROR)
		{
			ScreenWriteString(STRING_COMMAND_MORE_ERROR_READING_FROM_FILE);
			return -1;
		}
		
		// The file end is reached
		if (Read_Bytes_Count == 0) return 0;
		
		// Display the character
		SystemScreenWriteCharacter(Character);
		
		// Exit if the character is a new line
		if (Character == '\n') return 1;
	}

	// A full line of characters has been displayed
	return 1;
}

/** Display the next file page.
 * @return 1 if the end of the file is reached,
 * @return 0 if the next page has been successfully displayed,
 * @return -1 if an error occurred.
 */
static int DisplayNextPage(void)
{
	int i, Result;
	
	// Display a full page of text
	for (i = 0; i < SYSTEM_SCREEN_ROWS_COUNT - 1; i++)
	{
		// Read a line
		Result = DisplayNextLine(); // Recycle i variable
		if (Result == -1) return -1;
		else if (Result == 0) return 0; // The end of file is reached
	}
	
	return 1;
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int CommandMainMore(int argc, char *argv[])
{
	char *String_File_Name;
	int Return_Value = -1, Result;
	unsigned int Cursor_Row, Cursor_Column;
	
	// Check parameters
	if (argc != 2)
	{
		ScreenWriteString(STRING_COMMAND_MORE_USAGE);
		goto Exit;
	}
	
	// Display the help if requested
	if (SystemStringCompare(argv[1], "-h"))
	{
		ScreenWriteString(STRING_COMMAND_MORE_USAGE);
		Return_Value = 0;
		goto Exit;
	}
	String_File_Name = argv[1];
	
	// Try to open the provided file
	if (SystemFileOpen(String_File_Name, SYSTEM_FILE_OPENING_MODE_READ, &File_ID) != ERROR_CODE_NO_ERROR)
	{
		ScreenWriteString(STRING_COMMAND_MORE_FILE_NOT_FOUND_1);
		ScreenWriteString(String_File_Name);
		ScreenWriteString(STRING_COMMAND_MORE_FILE_NOT_FOUND_2);
		goto Exit;
	}
	
	// Display a full page of text
	Result = DisplayNextPage();
	if (Result == -1) goto Exit;
	else if (Result == 0)
	{
		Return_Value = 0;
		goto Exit;
	}
	
	// Wait for a user key
	while (1)
	{
		switch (SystemKeyboardReadCharacter())
		{
			// Display the next line
			case '\n':
				Result = DisplayNextLine();
				if (Result == -1) goto Exit;
				else if (Result == 0)
				{
					Return_Value = 0;
					goto Exit;
				}
				break;
				
			// Display the next page
			case ' ':
				Result = DisplayNextPage();
				if (Result == -1) goto Exit;
				else if (Result == 0)
				{
					Return_Value = 0;
					goto Exit;
				}
				break;
				
			// Exit program
			case 'Q':
			case 'q':
				Return_Value = 0;
				goto Exit;
		}
	}
	
Exit:
	SystemFileClose(File_ID);
	// Append a new line character if the cursor is not at the begining of a line
	SystemScreenGetCursorPosition(&Cursor_Row, &Cursor_Column);
	if (Cursor_Column > 0) SystemScreenWriteCharacter('\n');
	
	return Return_Value;
}