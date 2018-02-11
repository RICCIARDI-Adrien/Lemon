/** @file Screen_Write_Hexadecimal_Integer.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesScreenWriteHexadecimalInteger(int Integer)
{
	char String_Hexadecimal[9]; // 8 digits + terminating zero
	
	LibrariesStringConvertIntegerToHexadecimal(Integer, String_Hexadecimal);
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_WRITE_STRING, 0, 0, String_Hexadecimal, NULL);
}
