/** @file String_Compare.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesStringCompare(char *String_1, char *String_2)
{
	while (*String_1 != 0)
	{
		if (*String_1 != *String_2) return 0;
		String_1++;
		String_2++;
	}
	
	if (*String_2 != 0) return 0; // Strings are not the same length
	return 1;
}
