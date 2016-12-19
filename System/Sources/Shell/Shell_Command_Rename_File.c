/** @file Shell_Command_Rename_File.c
 * Rename an existing file.
 * @author Adrien RICCIARDI
 */
#include <Drivers/Driver_Screen.h>
#include <Error_Codes.h>
#include <File_System/File.h>
#include <Shell.h>
#include <Strings.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ShellCommandRenameFile(char *String_Current_File_Name, char *String_New_File_Name)
{
	// Set color here as it is the one used by the most cases
	ScreenSetColor(SCREEN_COLOR_RED);
	
	switch (FileRename(String_Current_File_Name, String_New_File_Name))
	{
		case ERROR_CODE_FILE_ALREADY_EXISTS:
			ScreenWriteString(STRING_SHELL_RENAME_FILE_ALREADY_EXISTS_1);
			ScreenWriteString(String_New_File_Name);
			ScreenWriteString(STRING_SHELL_RENAME_FILE_ALREADY_EXISTS_2);
			break;
			
		case ERROR_CODE_FILE_NOT_FOUND:
			ScreenWriteString(STRING_SHELL_RENAME_FILE_NOT_FOUND_1);
			ScreenWriteString(String_Current_File_Name);
			ScreenWriteString(STRING_SHELL_RENAME_FILE_NOT_FOUND_2);
			break;
			
		default:
			ScreenSetColor(SCREEN_COLOR_GREEN);
			ScreenWriteString(STRING_SHELL_RENAME_FILE_SUCCESS);
			break;
	}
}