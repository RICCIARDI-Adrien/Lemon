/** @file write.c
 * Provide write system call Lemon implementation.
 * @author Adrien RICCIARDI
 */
#include <errno.h>
#include <System.h>
#include <unistd.h>

// Errno definition, as specified by newlib documentation
#undef errno
extern int errno;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int write(int File_Descriptor, const void *Pointer_Buffer, size_t Bytes_Count)
{
	size_t i;
	unsigned char *Pointer_Buffer_Bytes = (unsigned char *) Pointer_Buffer;
	
	// Can't write to stdin
	if (File_Descriptor == 0)
	{
		errno = EBADF; // Invalid file descriptor
		return -1;
	}
	
	// Writing to stdout or stderr prints to the screen
	if (File_Descriptor <= 2)
	{
		// Display the string
		for (i = 0; i < Bytes_Count; i++)
		{
			SystemCall(SYSTEM_CALL_SCREEN_WRITE_CHARACTER, *Pointer_Buffer_Bytes, 0, NULL, NULL);
			Pointer_Buffer_Bytes++;
		}
		
		return Bytes_Count;
	}
	
	// Greater file descriptors correspond to files
	File_Descriptor -= 3; // Lemon file descriptors start from 0, so bypass stdin, stdout and stderr
	switch (SystemCall(SYSTEM_CALL_FILE_WRITE, File_Descriptor, Bytes_Count, (void *) Pointer_Buffer, NULL))
	{
		case ERROR_CODE_NO_ERROR:
			return Bytes_Count;
			
		case ERROR_CODE_BLOCKS_LIST_FULL:
			errno = ENOSPC;
			return -1;
		
		default:
			errno = EBADF;
			return -1;
	}
}