/** @file Command_Df.c
 * A UNIX-like disk free command.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
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
		LibrariesScreenWriteString(STRING_COMMAND_DF_USAGE);
		return -1;
	}
	
	// Get the file system properties
	LibrariesFileSystemGetTotalSize(&Block_Size, &Total_Storage, &Total_Files_Count);
	LibrariesFileSystemGetFreeSize(&Free_Storage, &Free_Files_Count);
	
	// Convert the blocks unit to bytes
	Total_Storage *= Block_Size;
	Free_Storage *= Block_Size;
	
	// Display the files values
	LibrariesScreenWriteString(STRING_COMMAND_DF_FILES_COUNT);
	LibrariesScreenWriteUnsignedInteger(Total_Files_Count - Free_Files_Count);
	LibrariesScreenWriteCharacter('/');
	LibrariesScreenWriteUnsignedInteger(Total_Files_Count);
	// Display the percentage
	LibrariesScreenWriteString(" (");
	LibrariesScreenWriteUnsignedInteger(100 - ((100 * Free_Files_Count) / Total_Files_Count));
	LibrariesScreenWriteString("%)\n");
	
	// Display the remaining storage
	LibrariesScreenWriteString(STRING_COMMAND_DF_REMAINING_STORAGE_1);
	LibrariesScreenWriteUnsignedInteger(Total_Storage - Free_Storage);
	LibrariesScreenWriteCharacter('/');
	LibrariesScreenWriteUnsignedInteger(Total_Storage);
	LibrariesScreenWriteString(STRING_COMMAND_DF_REMAINING_STORAGE_2);
	// Display the percentage
	LibrariesScreenWriteString(" (");
	LibrariesScreenWriteUnsignedInteger(100 - ((100 * Free_Storage) / Total_Storage));
	LibrariesScreenWriteString("%)\n");
	
	return 0;
}
