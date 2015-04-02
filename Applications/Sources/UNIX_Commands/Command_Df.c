/** @file Command_Df.c
 * A UNIX-like disk free command.
 * @author Adrien RICCIARDI
 * @version 1.0 : 20/03/2015
 */
#include <System.h>
#include "Commands.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Command entry point
//-------------------------------------------------------------------------------------------------
int CommandMainDf(int argc, char __attribute__((unused)) *argv[])
{
	unsigned int Block_Size, Total_Files_Count, Free_Files_Count, Total_Storage, Free_Storage;
	
	// Check parameters
	if (argc != 1)
	{
		ScreenWriteString(STRING_COMMAND_DF_USAGE);
		return -1;
	}
	
	// Get the file system properties
	SystemGetFileSystemTotalSize(&Block_Size, &Total_Storage, &Total_Files_Count);
	SystemGetFileSystemFreeSize(&Free_Storage, &Free_Files_Count);
	
	// Convert the blocks unit to bytes
	Total_Storage *= Block_Size;
	Free_Storage *= Block_Size;
	
	// Display the files values
	ScreenWriteString(STRING_COMMAND_DF_FILES_COUNT);
	ScreenWriteUnsignedInteger(Total_Files_Count - Free_Files_Count);
	ScreenWriteCharacter('/');
	ScreenWriteUnsignedInteger(Total_Files_Count);
	// Display the percentage
	ScreenWriteString(" (");
	ScreenWriteUnsignedInteger(100 - ((100 * Free_Files_Count) / Total_Files_Count));
	ScreenWriteString("%)\n");
	
	// Display the remaining storage
	ScreenWriteString(STRING_COMMAND_DF_REMAINING_STORAGE_1);
	ScreenWriteUnsignedInteger(Total_Storage - Free_Storage);
	ScreenWriteCharacter('/');
	ScreenWriteUnsignedInteger(Total_Storage);
	ScreenWriteString(STRING_COMMAND_DF_REMAINING_STORAGE_2);
	// Display the percentage
	ScreenWriteString(" (");
	ScreenWriteUnsignedInteger(100 - ((100 * Free_Storage) / Total_Storage));
	ScreenWriteString("%)\n");
	
	return 0;
}