/** @file Screen_Write_Unsigned_Integer.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenWriteUnsignedInteger(unsigned int Unsigned_Integer)
{
	char String_Number[11]; // 10 digits (the maximum digits count for a 32-bit number) + terminating zero
	
	SystemStringConvertUnsignedIntegerToString(Unsigned_Integer, String_Number);
	SystemCall(SYSTEM_CALL_SCREEN_WRITE_STRING, 0, 0, String_Number, NULL);
}