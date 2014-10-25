/** @file Shell_Command_List.c
 * List existing files.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/12/2012
 * @version 1.1 : 29/12/2012, added to kernel code.
 */
#include <Configuration.h> // Needed by CONFIGURATION_FILE_NAME_LENGTH
#include <Drivers/Driver_Screen.h>
#include <File_System/File.h>
#include <Shell/Shell_Commands.h>

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
void ShellCommandList(void)
{
	char Buffer[CONFIGURATION_FILE_NAME_LENGTH + 1];
	
	FileListInitialize();
		
	while (1)
	{
		FileListNext(Buffer);
		if (Buffer[0] == 0) break;
		
		ScreenWriteString(Buffer);
		ScreenWriteChar('\n');
	}
}