/** @file System_File_System_Get_Free_Size.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemFileSystemGetFreeSize(unsigned int *Pointer_Blocks_Count, unsigned int *Pointer_Files_Count)
{
	SystemCall(SYSTEM_CALL_SYSTEM_GET_PARAMETER, SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_FREE_BLOCKS_LIST_ENTRIES_COUNT, 0, Pointer_Blocks_Count, NULL);
	SystemCall(SYSTEM_CALL_SYSTEM_GET_PARAMETER, SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_FREE_FILES_LIST_ENTRIES_COUNT, 0, Pointer_Files_Count, NULL);
}
