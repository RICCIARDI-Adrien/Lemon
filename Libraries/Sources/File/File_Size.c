/** @file FileSize.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 11/12/2011
 * @version 1.1 : 27/04/2012
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int FileSize(char *String_File_Name)
{
	return SystemCall(SYSTEM_CALL_FILE_SIZE, 0, 0, String_File_Name, NULL);
}