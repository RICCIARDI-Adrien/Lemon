/** @file Libraries_File_Read.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesFileRead(unsigned int File_ID, void *Pointer_Buffer, unsigned int Bytes_Count, unsigned int *Pointer_Bytes_Read)
{
	*Pointer_Bytes_Read = 0; // Force to 0 to return 0 in case of error
	
	return LibrariesSystemCall(SYSTEM_CALL_FILE_READ, File_ID, Bytes_Count, Pointer_Buffer, Pointer_Bytes_Read);
}
