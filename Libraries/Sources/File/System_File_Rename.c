/** @file Libraries_File_Rename.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesFileRename(char *String_Current_File_Name, char *String_New_File_Name)
{
	return LibrariesSystemCall(SYSTEM_CALL_FILE_RENAME, 0, 0, String_Current_File_Name, String_New_File_Name);
}
