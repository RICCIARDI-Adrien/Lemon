/** @file File_List_Next.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void FileListNext(char *String_File_Name)
{
	SystemCall(SYSTEM_CALL_FILE_LIST_NEXT, 0, 0, String_File_Name, NULL);
}