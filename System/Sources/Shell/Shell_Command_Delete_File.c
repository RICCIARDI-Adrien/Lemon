/** @file Shell_Command_Delete_File.c
 * Delete an existing file.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/12/2012
 * @version 1.1 : 29/12/2012, added to kernel code.
 */
#include <Drivers/Driver_Screen.h>
#include <Error_Codes.h>
#include <File_System/File.h>
#include <Shell/Shell_Commands.h>
#include <Strings.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ShellCommandDeleteFile(char *String_File_Name)
{	
	if (FileDelete(String_File_Name) == ERROR_CODE_NO_ERROR)
	{
		ScreenSetColor(SCREEN_COLOR_GREEN);
		ScreenWriteString(STRING_SHELL_DELETE_FILE_SUCCESS);
	}
	else
	{
		ScreenSetColor(SCREEN_COLOR_RED);
		ScreenWriteString(STRING_SHELL_DELETE_FILE_FAILURE_1);
		ScreenWriteString(String_File_Name);
		ScreenWriteString(STRING_SHELL_DELETE_FILE_FAILURE_2);
	}
}