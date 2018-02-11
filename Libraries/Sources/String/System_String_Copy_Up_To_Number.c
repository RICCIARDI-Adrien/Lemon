/** @file Libraries_String_Copy_Up_To_Number.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesStringCopyUpToNumber(char *String_Source, char *String_Destination, unsigned int Bytes_Count)
{
	while ((*String_Source != 0) && (Bytes_Count > 0))
	{
		*String_Destination = *String_Source;
		String_Destination++;
		String_Source++;
		Bytes_Count--;
	}
	
	// Append the terminating zero
	*String_Destination = 0;
}
