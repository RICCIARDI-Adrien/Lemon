/** @file Shell_Command_Copy_File.c
 * Copy the content of an existing file into a new one.
 * @author Adrien RICCIARDI
 * @version 1.0 : 16/04/2013.
 */
#include <Configuration.h>
#include <Drivers/Driver_Screen.h>
#include <Error_Codes.h>
#include <File_System/File.h>
#include <File_System/File_System.h>
#include <Kernel.h> // To have the NULL definition
#include <Shell/Shell_Commands.h>
#include <Strings.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ShellCommandCopyFile(char *String_File_Name_Source, char *String_File_Name_Destination)
{
	unsigned int Free_Bytes_Count, Next_Block_To_Read, Current_Block_To_Write, Next_Block_To_Write, File_Size_Bytes, Blocks_Count;
	TFilesListEntry *Pointer_Files_List_Entry_Source, *Pointer_Files_List_Entry_Destination;
	unsigned char Buffer[CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES];

	// Set the console color to red only one time as many errors can occur
	ScreenSetColor(SCREEN_COLOR_RED);
	
	// Check if the source file exists
	Pointer_Files_List_Entry_Source = FileSystemReadFilesListEntry(String_File_Name_Source);
	if (Pointer_Files_List_Entry_Source == NULL)
	{
		ScreenWriteString(STRING_SHELL_COPY_FILE_SOURCE_FILE_NOT_FOUND);
		return;
	}
	
	// Check if the destination file is not existing
	if (FileExists(String_File_Name_Destination))
	{
		ScreenWriteString(STRING_SHELL_COPY_FILE_DESTINATION_FILE_EXISTING_YET);
		return;
	}

	// Check if there is enough room to store the new file
	Free_Bytes_Count = FileSystemGetFreeBlocksCount() * CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES;
	if (FileSize(String_File_Name_Source) > Free_Bytes_Count)
	{
		ScreenWriteString(STRING_SHELL_COPY_FILE_NOT_ENOUGH_ROOM);
		return;
	}
	
	// Try to create the new file into the Files List
	if (FileSystemWriteFilesListEntry(String_File_Name_Destination, &Pointer_Files_List_Entry_Destination) != ERROR_CODE_NO_ERROR)
	{
		ScreenWriteString(STRING_SHELL_COPY_FILE_NOT_ENOUGH_ROOM);
		return;
	}
	
	ScreenSetColor(SCREEN_COLOR_BLUE);
	ScreenWriteString(STRING_SHELL_COPY_FILE_STARTING_COPY);
	
	// Copy file size from source Files List entry to destination Files List entry
	File_Size_Bytes = Pointer_Files_List_Entry_Source->Size_Bytes;
	Pointer_Files_List_Entry_Destination->Size_Bytes = File_Size_Bytes;

	// Copy data only if it is not an empty file
	if (File_Size_Bytes > 0)
	{
		// Initialize blocks copy
		Next_Block_To_Read = Pointer_Files_List_Entry_Source->Start_Block;
		
		// Set new file starting block
		Next_Block_To_Write = FileSystemGetFirstFreeBlock();
		Pointer_Files_List_Entry_Destination->Start_Block = Next_Block_To_Write;

		// Compute the number of blocks to copy
		Blocks_Count = File_Size_Bytes / CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES;
		if (File_Size_Bytes % CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES != 0) Blocks_Count++;

		while (Blocks_Count > 0)
		{
			// Copy one block
			Next_Block_To_Read = FileSystemReadBlocks(Next_Block_To_Read, 1, Buffer);
			FileSystemWriteBlocks(Next_Block_To_Write, 1, Buffer);

			// Reserve the next destination block and update the Blocks List
			Current_Block_To_Write = Next_Block_To_Write;
			Next_Block_To_Write = FileSystemGetFirstFreeBlock();
			Blocks_List[Current_Block_To_Write] = Next_Block_To_Write;

			Blocks_Count--;
		}

		// Terminate the list
		Blocks_List[Current_Block_To_Write] = FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF;
	}

	// Backup changes in Files List and Blocks List
	FileSystemSave();

	ScreenSetColor(SCREEN_COLOR_GREEN);
	ScreenWriteString(STRING_SHELL_COPY_FILE_SUCCESS);
}