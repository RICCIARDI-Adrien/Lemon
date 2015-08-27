/** @file File_Open.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int FileOpen(char *String_File_Name, TFileOpeningMode Opening_Mode, unsigned int *Pointer_File_ID)
{
	// Check for a NULL file name as the kernel can't be annoyed with this
	if (String_File_Name == NULL) return ERROR_CODE_BAD_FILE_NAME;
	
	return SystemCall(SYSTEM_CALL_FILE_OPEN, (char) Opening_Mode, 0, String_File_Name, Pointer_File_ID);
}