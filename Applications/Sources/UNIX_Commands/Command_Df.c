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
	unsigned int Block_Size, Total_Files_Count, Free_Files_Count, Used_Files_Count, Total_Storage, Free_Storage, Used_Storage;
	
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
	Used_Files_Count = Total_Files_Count - Free_Files_Count;
	LibrariesScreenWriteString(STRING_COMMAND_DF_FILES_COUNT);
	LibrariesScreenWriteUnsignedInteger(Used_Files_Count);
	LibrariesScreenWriteCharacter('/');
	LibrariesScreenWriteUnsignedInteger(Total_Files_Count);
	// Display the percentage
	LibrariesScreenWriteString(" (");
	LibrariesScreenWriteUnsignedInteger((100ULL * Used_Files_Count) / Total_Files_Count); // Promote computations to 64 bits as the multiplication per 100 can give big results if file system is quite big
	LibrariesScreenWriteString("%)\n");
	
	// Display the remaining storage
	Used_Storage = Total_Storage - Free_Storage;
	LibrariesScreenWriteString(STRING_COMMAND_DF_REMAINING_STORAGE_1);
	LibrariesScreenWriteUnsignedInteger(Used_Storage);
	LibrariesScreenWriteCharacter('/');
	LibrariesScreenWriteUnsignedInteger(Total_Storage);
	LibrariesScreenWriteString(STRING_COMMAND_DF_REMAINING_STORAGE_2);
	// Display the percentage
	LibrariesScreenWriteString(" (");
	LibrariesScreenWriteUnsignedInteger((100ULL * Used_Storage) / Total_Storage); // Promote computations to 64 bits as the multiplication per 100 can give big results if file system is quite big
	LibrariesScreenWriteString("%)\n");
	
	return 0;
}
