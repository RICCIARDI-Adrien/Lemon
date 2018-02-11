/** @file System_Screen_Write_Unsigned_Integer.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemScreenWriteUnsignedInteger(unsigned int Unsigned_Integer)
{
	char String_Number[11]; // 10 digits (the maximum digits count for a 32-bit number) + terminating zero
	
	SystemStringConvertUnsignedIntegerToString(Unsigned_Integer, String_Number);
	LibrariesSystemCall(SYSTEM_CALL_SCREEN_WRITE_STRING, 0, 0, String_Number, NULL);
}
