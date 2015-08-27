/** @file String_Write_String.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenWriteString(char *String)
{
	SystemCall(SYSTEM_CALL_SCREEN_WRITE_STRING, 0, 0, String, NULL);
}