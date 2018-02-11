/** @file System_File_Exists.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int SystemFileExists(char *String_File_Name)
{
	return LibrariesSystemCall(SYSTEM_CALL_FILE_EXISTS, 0, 0, String_File_Name, NULL);
}
