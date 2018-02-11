/** @file String_Write_String.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesScreenWriteString(char *String)
{
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_WRITE_STRING, 0, 0, String, NULL);
}
