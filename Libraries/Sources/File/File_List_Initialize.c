/** @file File_List_Initialize.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 14/09/2012
 */
#include <File.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void File_List_Initialize(void)
{
	SystemCall(SYSTEM_CALL_FILE_LIST_INITIALIZE, 0, 0, NULL, NULL);
}