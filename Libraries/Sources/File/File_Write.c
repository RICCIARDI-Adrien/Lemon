/** @file File_Write.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 26/10/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int FileWrite(unsigned int File_ID, void *Pointer_Buffer, unsigned int Bytes_Count)
{
	return SystemCall(SYSTEM_CALL_FILE_WRITE, File_ID, Bytes_Count, Pointer_Buffer, NULL);
}