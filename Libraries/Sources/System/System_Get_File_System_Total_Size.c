/** @file System_Get_File_System_Total_Size.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 09/12/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemGetFileSystemTotalSize(unsigned int *Pointer_Block_Size, unsigned int *Pointer_Blocks_Count, unsigned int *Pointer_Files_Count)
{
	SystemCall(SYSTEM_CALL_SYSTEM_GET_PARAMETER, SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_BLOCK_SIZE, 0, Pointer_Block_Size, NULL);
	SystemCall(SYSTEM_CALL_SYSTEM_GET_PARAMETER, SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_MAXIMUM_BLOCKS_LIST_ENTRIES_COUNT, 0, Pointer_Blocks_Count, NULL);
	SystemCall(SYSTEM_CALL_SYSTEM_GET_PARAMETER, SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES_COUNT, 0, Pointer_Files_Count, NULL);
}