/** @file String_Concatenate.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void StringConcatenate(char *String_Destination, char *String_To_Append)
{
	int i, j;
	
	// Go to the destination string end
	for (i = 0; String_Destination[i] != 0; i++);
	
	// Copy the string to append at the end of the destination string
	for (j = 0; String_To_Append[j] != 0; j++)
	{
		String_Destination[i] = String_To_Append[j];
		i++;
	}
	String_Destination[i] = 0; // Append terminating zero
}