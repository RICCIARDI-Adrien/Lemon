/** @file System_File_List_Next.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemFileListNext(char *String_File_Name)
{
	LibrariesSystemCall(SYSTEM_CALL_FILE_LIST_NEXT, 0, 0, String_File_Name, NULL);
}
