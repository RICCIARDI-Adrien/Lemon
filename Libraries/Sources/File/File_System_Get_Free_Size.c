/** @file File_System_Get_Free_Size.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesFileSystemGetFreeSize(unsigned int *Pointer_Blocks_Count, unsigned int *Pointer_Files_Count)
{
	LibrariesSystemCall(SYSTEM_CALL_SYSTEM_GET_PARAMETER, SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_FREE_BLOCKS_LIST_ENTRIES_COUNT, 0, Pointer_Blocks_Count, NULL);
	LibrariesSystemCall(SYSTEM_CALL_SYSTEM_GET_PARAMETER, SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_FREE_FILES_LIST_ENTRIES_COUNT, 0, Pointer_Files_Count, NULL);
}
