/** @file Text_Viewer.c
 * @see Text_Viewer.h for description.
 * @author Adrien RICCIARDI
 */
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <System.h>
#include <unistd.h>
#include "Text_Viewer.h"

//-------------------------------------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------------------------------------
/** The End Of Line arbitrary character. */
#define TEXT_VIEWER_END_OF_LINE_CHARACTER 1 // Smiley character, so it's easy to find it if it is displayed

//-------------------------------------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------------------------------------
static char Buffer[TEXT_VIEWER_BUFFER_SIZE_BYTES];
static unsigned int Current_Offset, File_Size_Bytes;

//-------------------------------------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Try to load the whole file in memory and add new line markers where needed.
 * @param String_File_Name Name of the file to load.
 * @return 0 if the file was successfully loaded,
 * @return 1 if the file was not found,
 * @return 2 if the file is too big to fit in memory,
 * @return 3 if an error occured when reading the file.
 */
static int LoadFile(char *String_File_Name)
{
	FILE *File;
	unsigned int i, Characters_Count = 0;
	char Character;
	
	// Open the file
	File = fopen(String_File_Name, "r");
	if (File == NULL) return 1;
	
	// Get file size
	File_Size_Bytes = FileSize(String_File_Name);
	
	// Load file line after line
	Current_Offset = 0;
	for (i = 0; i < File_Size_Bytes; i++)
	{
		// Is there enough room in buffer to store the character ?
		if (Current_Offset >= TEXT_VIEWER_BUFFER_SIZE_BYTES)
		{
			fclose(File);
			return 2;
		}
		
		// Read a single character
		if (fread(&Character, 1, 1, File) != 1)
		{
			fclose(File);
			return 3;
		}
		Characters_Count++;
		
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

	// Adjust the file size to take into account of the added end of line characters
	File_Size_Bytes = Current_Offset;
	Current_Offset = 0; // Reset buffer offset to display the text from the beginning
	
	fclose(File);
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

/** Display the next file line. */
static void DisplayNextLine(void)
{
	unsigned int Characters_Count = 0;
	char Character;

	// Show characters from the buffer only if the end of the file has not been reached, the end of the line has not been reached, and a new line has not been found
	while (Characters_Count < TEXT_VIEWER_SCREEN_WIDTH)
	{
		// Is end of file reached ?
		if (Current_Offset >= File_Size_Bytes) return;
		
		// Get the next character to display
		Character = Buffer[Current_Offset];
		Current_Offset++;
			
		switch (Character)
		{
			// Exit if a new line has been found
			case '\n':
				putchar('\n');
				return;
				
			// Ignore end of line markers has they are located after the SCREEN_WIDTH'th character, so they will appear at the beginning of the next line
			case TEXT_VIEWER_END_OF_LINE_CHARACTER:
				break;
			
			// Show the character
			default:
				putchar(Character);
				Characters_Count++;
				break;
		}
	}
	fflush(stdout);
}

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int i;
	unsigned int Row, Column;
	char *String_File_Name;
	
	// Check parameters
	if (argc != 2)
	{
		printf(STRING_ERROR_BAD_PARAMETERS, argv[0]);
		return -1;
	}
	String_File_Name = argv[1];

	// Load the file
	switch (LoadFile(String_File_Name))
	{
		case 1:
			printf(STRING_ERROR_FILE_NOT_FOUND, String_File_Name);
			return EXIT_FAILURE;
			
		case 2:
			printf(STRING_ERROR_FILE_TOO_BIG, sizeof(Buffer));
			return EXIT_FAILURE;
			
		case 3:
			printf(STRING_ERROR_READ_FAILED);
			return EXIT_FAILURE;
	}
	
	// Display the first lines
	for (i = 0; i < TEXT_VIEWER_SCREEN_HEIGHT - 1; i++) DisplayNextLine();

	// Display text
	while (1)
	{
		switch (KeyboardReadChar())
		{
			// Display next line
			case KEYBOARD_KEY_CODE_ARROW_DOWN:
				DisplayNextLine();
				break;
			
			case KEYBOARD_KEY_CODE_ARROW_UP:
				// Go up only if the top of the screen is not reached (to avoid flickering effects)
				if (Current_Offset > 0)
				{
					ScreenClear();
				
					// Go to the line before the first which is currently displayed
					for (i = 0; i < TEXT_VIEWER_SCREEN_HEIGHT; i++) MoveToPreviousLine();
					
					// Display this line and the following
					for (i = 0; i < TEXT_VIEWER_SCREEN_HEIGHT - 1; i++) DisplayNextLine();
				}
				break;
			
			// Exit program
			case KEYBOARD_KEY_CODE_ESCAPE:
				// Go to next line if needed
				ScreenGetCursorPosition(&Row, &Column);
				if (Column > 0) putchar('\n');
				return 0;
		}
	}
}