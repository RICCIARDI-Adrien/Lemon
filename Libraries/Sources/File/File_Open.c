/** @file File_Open.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 26/10/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int FileOpen(char *String_File_Name, TFileOpeningMode Opening_Mode, unsigned int *Pointer_File_ID)
{
	return SystemCall(SYSTEM_CALL_FILE_OPEN, (char) Opening_Mode, 0, String_File_Name, Pointer_File_ID);
}