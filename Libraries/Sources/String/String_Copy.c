/** @file String_Copy.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/10/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void StringCopy(char *String_Source, char *String_Destination)
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