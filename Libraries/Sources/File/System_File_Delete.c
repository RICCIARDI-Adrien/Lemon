/** @file System_File_Delete.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int SystemFileDelete(char *String_File_Name)
{
	return SystemCall(SYSTEM_CALL_FILE_DELETE, 0, 0, String_File_Name, NULL);
}
