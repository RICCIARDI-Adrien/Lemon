/** @file Shell_Command_File_Size.c
 * Get the size in bytes of an existing file.
 * @author Adrien RICCIARDI
 */
#include <Drivers/Driver_Screen.h>
#include <File_System/File.h>
#include <Kernel.h>
#include <Shell.h>
#include <Standard_Functions.h> // To have itoa()
#include <Strings.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ShellCommandFileSize(char *String_File_Name)
{
	unsigned int File_Size_Bytes;
	
	File_Size_Bytes = FileSize(String_File_Name);
	
	if (File_Size_Bytes == 0)
	{
		ScreenSetColor(SCREEN_COLOR_RED);
		ScreenWriteString(STRING_SHELL_FILE_SIZE_BAD_FILE_NAME_1);
		ScreenWriteString(String_File_Name);
		ScreenWriteString(STRING_SHELL_FILE_SIZE_BAD_FILE_NAME_2);
	}
	else
	{
		ScreenWriteString(STRING_SHELL_FILE_SIZE_SHOW_SIZE_1);
		ScreenWriteString(itoa(File_Size_Bytes));
		ScreenWriteString(STRING_SHELL_FILE_SIZE_SHOW_SIZE_2);
	}
}