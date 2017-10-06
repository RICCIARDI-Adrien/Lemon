/** @file System_File_List_Initialize.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemFileListInitialize(void)
{
	SystemCall(SYSTEM_CALL_FILE_LIST_INITIALIZE, 0, 0, NULL, NULL);
}
