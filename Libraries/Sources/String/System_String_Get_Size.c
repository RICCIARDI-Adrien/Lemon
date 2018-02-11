/** @file Libraries_String_Get_Size.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int LibrariesStringGetSize(char *String)
{
	unsigned int Size = 0;
	
	while (*String != 0)
	{
		Size++;
		String++;
	}
	
	return Size;
}
