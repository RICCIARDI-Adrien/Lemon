/** @file String_Copy.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesStringCopy(char *String_Source, char *String_Destination)
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
