/** @file String_Concatenate_Up_To_Number.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesStringConcatenateUpToNumber(char *String_Destination, char *String_To_Append, unsigned int Characters_Count)
{
	// Go to the destination string end
	while (*String_Destination != 0) String_Destination++;
	
	// Copy the string to append at the end of the destination string
	while ((*String_To_Append != 0) && (Characters_Count > 0))
	{
		*String_Destination = *String_To_Append;
		String_Destination++;
		String_To_Append++;
		Characters_Count--;
	}
	*String_Destination = 0; // Append terminating zero
}
