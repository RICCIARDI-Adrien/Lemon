/** @file FileListNext.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 14/09/2012
 */
#include <File.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void FileListNext(char *String_File_Name)
{
	SystemCall(SYSTEM_CALL_FILE_LIST_NEXT, 0, 0, String_File_Name, NULL);
}