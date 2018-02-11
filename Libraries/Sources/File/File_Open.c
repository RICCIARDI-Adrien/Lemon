/** @file File_Open.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesFileOpen(char *String_File_Name, TLibrariesFileOpeningMode Opening_Mode, unsigned int *Pointer_File_ID)
{
	// Check for a NULL file name as the kernel can't be annoyed with this
	if (String_File_Name == NULL) return ERROR_CODE_BAD_FILE_NAME;
	
	return LibrariesSystemCall(SYSTEM_CALL_FILE_OPEN, (char) Opening_Mode, 0, String_File_Name, Pointer_File_ID);
}
