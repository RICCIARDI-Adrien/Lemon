/** @file File_Exists.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int FileExists(char *String_File_Name)
{
	return SystemCall(SYSTEM_CALL_FILE_EXISTS, 0, 0, String_File_Name, NULL);
}