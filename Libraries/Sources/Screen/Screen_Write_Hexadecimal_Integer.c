/** @file Screen_Write_Hexadecimal_Integer.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenWriteHexadecimalInteger(int Integer)
{
	char String_Hexadecimal[9]; // 8 digits + terminating zero
	
	StringConvertIntegerToHexadecimal(Integer, String_Hexadecimal);
	SystemCall(SYSTEM_CALL_SCREEN_WRITE_STRING, 0, 0, String_Hexadecimal, NULL);
}