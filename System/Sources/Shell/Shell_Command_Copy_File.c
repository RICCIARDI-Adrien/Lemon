/** @file Shell_Command_Copy_File.c
 * Copy the content of an existing file into a new one.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Drivers/Driver_Screen.h>
#include <Error_Codes.h>
#include <File_System/File.h>
#include <File_System/File_System.h>
#include <Shell.h>
#include <Standard_Functions.h> // To have the NULL definition
#include <Strings.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ShellCommandCopyFile(char *String_File_Name_Source, char *String_File_Name_Destination)
{
	unsigned int Free_Bytes_Count, File_Size_Bytes, File_ID_Source, File_ID_Destination, Read_Bytes_Count;
	unsigned char Buffer[CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES];

	// Set the console color to red only one time as many errors can occur
	ScreenSetColor(SCREEN_COLOR_RED);
	
	// Try to open the source file
	if (FileOpen(String_File_Name_Source, 'r', &File_ID_Source) != 0)
	{
		ScreenWriteString(STRING_SHELL_COPY_FILE_SOURCE_FILE_NOT_FOUND);
		goto Exit;
	}
	
	// Check if the destination file is not existing
	if (FileExists(String_File_Name_Destination))
	{
		ScreenWriteString(STRING_SHELL_COPY_FILE_DESTINATION_FILE_EXISTING_YET);
		goto Exit;
	}
	
	// Check if there is enough room to store the new file
	Free_Bytes_Count = FileSystemGetFreeBlocksCount() * CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES;
	File_Size_Bytes = FileSize(String_File_Name_Source);
	if (File_Size_Bytes > Free_Bytes_Count)
	{
		ScreenWriteString(STRING_SHELL_COPY_FILE_NOT_ENOUGH_ROOM);
		goto Exit;
	}
	
	// Try to create the destination file
	if (FileOpen(String_File_Name_Destination, 'w', &File_ID_Destination) != 0)
	{
		ScreenWriteString(STRING_SHELL_COPY_FILE_NOT_ENOUGH_ROOM);
		goto Exit;
	}
	
	ScreenSetColor(SCREEN_COLOR_BLUE);
	ScreenWriteString(STRING_SHELL_COPY_FILE_STARTING_COPY);
	ScreenSetColor(SCREEN_COLOR_RED);
	
	// Copy data only if it is not an empty file
	if (File_Size_Bytes > 0)
	{
		do
		{
			// Read data from the source file
			if (FileRead(File_ID_Source, Buffer, sizeof(Buffer), &Read_Bytes_Count) != 0)
			{
				ScreenWriteString(STRING_SHELL_COPY_SOURCE_FILE_READ_ERROR);
				goto Exit;
			}
			
			// Write the read data to the destination file
			if (FileWrite(File_ID_Destination, Buffer, Read_Bytes_Count) != 0)
			{
				ScreenWriteString(STRING_SHELL_COPY_DESTINATION_FILE_WRITE_ERROR);
				goto Exit;
			}
		} while (Read_Bytes_Count == sizeof(Buffer)); // Stop when we read less than expected, telling that the source file's end is reached
	}

	// Backup changes in Files List and Blocks List
	FileSystemSave();

	ScreenSetColor(SCREEN_COLOR_GREEN);
	ScreenWriteString(STRING_SHELL_COPY_FILE_SUCCESS);
	
Exit:
	FileClose(File_ID_Source);
	FileClose(File_ID_Destination);
}