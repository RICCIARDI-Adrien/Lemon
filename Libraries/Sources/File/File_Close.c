/** @file File_Close.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 26/10/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void FileClose(unsigned int File_ID)
{
	SystemCall(SYSTEM_CALL_FILE_CLOSE, File_ID, 0, NULL, NULL);
}