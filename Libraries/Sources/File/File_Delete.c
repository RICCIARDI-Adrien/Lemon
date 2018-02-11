/** @file File_Delete.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesFileDelete(char *String_File_Name)
{
	return LibrariesSystemCall(SYSTEM_CALL_FILE_DELETE, 0, 0, String_File_Name, NULL);
}
