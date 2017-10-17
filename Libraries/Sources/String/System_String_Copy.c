/** @file System_String_Copy.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemStringCopy(char *String_Source, char *String_Destination)
{
	while (*String_Source != 0)
	{
		*String_Destination = *String_Source;
		String_Destination++;
		String_Source++;
	}
	
	// Append the terminating zero
	*String_Destination = 0;
}
