/** @file String_Convert_String_To_Unsigned_Integer.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int StringConvertStringToUnsignedInteger(char *String)
{
	unsigned int Result = 0;
	
	while (*String != 0)
	{
		Result *= 10;
		Result = Result + (*String - 48);
		String++;
	}
	
	return Result;
}