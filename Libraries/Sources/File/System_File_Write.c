/** @file Libraries_File_Write.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesFileWrite(unsigned int File_ID, void *Pointer_Buffer, unsigned int Bytes_Count)
{
	return LibrariesSystemCall(SYSTEM_CALL_FILE_WRITE, File_ID, Bytes_Count, Pointer_Buffer, NULL);
}
