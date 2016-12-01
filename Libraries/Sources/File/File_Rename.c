/** @file File_Rename.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int FileRename(char *String_Current_File_Name, char *String_New_File_Name)
{
	return SystemCall(SYSTEM_CALL_FILE_RENAME, 0, 0, String_Current_File_Name, String_New_File_Name);
}