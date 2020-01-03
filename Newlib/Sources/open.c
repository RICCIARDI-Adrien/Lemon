/** @file open.c
 * Provide open system call Lemon implementation.
 * @author Adrien RICCIARDI
 */
#include <errno.h>
#include <fcntl.h>
#include <Libraries.h>
#include <sys/stat.h>
#include <sys/types.h>

// Errno definition, as specified by newlib documentation
#undef errno
extern int errno;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int open(const char *String_File_Name, int Flags, ...)
{
	TLibrariesFileOpeningMode Opening_Mode;
	unsigned int File_ID;
	
	// Determine requested opening mode
	switch (Flags & 0x03)
	{
		case O_RDONLY:
			Opening_Mode = LIBRARIES_FILE_OPENING_MODE_READ;
			break;
			
		case O_WRONLY:
			Opening_Mode = LIBRARIES_FILE_OPENING_MODE_WRITE;
			break;
			
		default:
			errno = EINVAL;
			return -1;
	}
	
	// Try to open the file
	switch (LibrariesSystemCall(SYSTEM_CALL_FILE_OPEN, Opening_Mode, 0, (char *) String_File_Name, &File_ID))
	{
		case ERROR_CODE_NO_ERROR:
			File_ID += 3; // Bypass stdin, stdout and stderr
			return (int) File_ID;
		
		case ERROR_CODE_CANT_OPEN_MORE_FILES:
			errno = ENFILE;
			return -1;
			
		case ERROR_CODE_FILES_LIST_FULL:
		case ERROR_CODE_BLOCKS_LIST_FULL:
			errno = ENOSPC;
			return -1;
			
		default:
			errno = EACCES; // Generic error
			return -1;
	}
}