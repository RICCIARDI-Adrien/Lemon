/** @file File_List_Initialize.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void FileListInitialize(void)
{
	SystemCall(SYSTEM_CALL_FILE_LIST_INITIALIZE, 0, 0, NULL, NULL);
}