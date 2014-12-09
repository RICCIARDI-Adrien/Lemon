/** @file File_Delete.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 09/12/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int FileDelete(char *String_File_Name)
{
	return SystemCall(SYSTEM_CALL_FILE_DELETE, 0, 0, String_File_Name, NULL);
}