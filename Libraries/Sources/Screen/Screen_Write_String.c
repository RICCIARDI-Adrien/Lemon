/** @file String_Write_String.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/10/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenWriteString(char *String)
{
	SystemCall(SYSTEM_CALL_SCREEN_WRITE_STRING, 0, 0, String, NULL);
}