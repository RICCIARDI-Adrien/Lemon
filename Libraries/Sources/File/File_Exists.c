/** @file File_Exists.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 11/12/2011
 * @version 1.1 : 27/04/2012
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int FileExists(char *String_File_Name)
{
	return SystemCall(SYSTEM_CALL_FILE_EXISTS, 0, 0, String_File_Name, NULL);
}