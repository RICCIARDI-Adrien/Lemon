/** @file System_File_Write.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int SystemFileWrite(unsigned int File_ID, void *Pointer_Buffer, unsigned int Bytes_Count)
{
	return SystemCall(SYSTEM_CALL_FILE_WRITE, File_ID, Bytes_Count, Pointer_Buffer, NULL);
}
