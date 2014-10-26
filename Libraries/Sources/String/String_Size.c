/** @file String_Size.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 26/10/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int StringSize(char *String)
{
	unsigned int Size = 0;
	
	while (*String != 0)
	{
		Size++;
		String++;
	}
	
	return Size;
}