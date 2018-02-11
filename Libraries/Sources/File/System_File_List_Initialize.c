/** @file Libraries_File_List_Initialize.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesFileListInitialize(void)
{
	LibrariesSystemCall(SYSTEM_CALL_FILE_LIST_INITIALIZE, 0, 0, NULL, NULL);
}
