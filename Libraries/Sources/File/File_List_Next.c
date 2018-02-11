/** @file File_List_Next.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesFileListNext(char *String_File_Name)
{
	LibrariesSystemCall(SYSTEM_CALL_FILE_LIST_NEXT, 0, 0, String_File_Name, NULL);
}
