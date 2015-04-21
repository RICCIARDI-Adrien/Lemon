/** @file Shell_Command_List.c
 * List existing files.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/12/2012
 * @version 1.1 : 29/12/2012, added to kernel code.
 * @version 1.2 : 21/04/2015, the list stops after displaying a full screen and waits for the user to hit a key.
 */
#include <Configuration.h>
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_Screen.h>
#include <File_System/File.h>
#include <Shell/Shell_Commands.h>
#include <Strings.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ShellCommandList(void)
{
	char String_File_Name[CONFIGURATION_FILE_NAME_LENGTH + 1];
	int Displayed_Files_Count = 0;
	
	FileListInitialize();
		
	while (1)
	{
		// Get the next file name
		FileListNext(String_File_Name);
		if (String_File_Name[0] == 0) break;
		
		// Display it
		ScreenWriteString(String_File_Name);
		ScreenWriteCharacter('\n');
		
		// Wait for the user to press a key if the screen is full of displayed file names
		Displayed_Files_Count++;
		if (Displayed_Files_Count == SCREEN_ROWS_COUNT - 1)
		{
			ScreenSetColor(SCREEN_COLOR_LIGHT_BLUE);
			ScreenWriteString(STRING_SHELL_LIST_WAIT_FOR_USER_INPUT);
			ScreenSetColor(SCREEN_COLOR_BLUE);
			
			KeyboardReadCharacter();
			ScreenWriteCharacter('\n');
			
			Displayed_Files_Count = 0;
		}
	}
}