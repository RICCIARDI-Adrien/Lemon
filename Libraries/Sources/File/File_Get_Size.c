/** @file File_Get_Size.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int FileGetSize(char *String_File_Name)
{
	return SystemCall(SYSTEM_CALL_FILE_SIZE, 0, 0, String_File_Name, NULL);
}