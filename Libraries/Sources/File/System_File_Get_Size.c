/** @file Libraries_File_Get_Size.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int LibrariesFileGetSize(char *String_File_Name)
{
	return LibrariesSystemCall(SYSTEM_CALL_FILE_SIZE, 0, 0, String_File_Name, NULL);
}
