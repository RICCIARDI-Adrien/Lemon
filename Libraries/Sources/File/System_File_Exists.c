/** @file Libraries_File_Exists.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesFileExists(char *String_File_Name)
{
	return LibrariesSystemCall(SYSTEM_CALL_FILE_EXISTS, 0, 0, String_File_Name, NULL);
}
