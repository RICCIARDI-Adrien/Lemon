/** @file System_File_Close.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemFileClose(unsigned int File_ID)
{
	LibrariesSystemCall(SYSTEM_CALL_FILE_CLOSE, File_ID, 0, NULL, NULL);
}
