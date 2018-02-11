/** @file File_Close.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesFileClose(unsigned int File_ID)
{
	LibrariesSystemCall(SYSTEM_CALL_FILE_CLOSE, File_ID, 0, NULL, NULL);
}
