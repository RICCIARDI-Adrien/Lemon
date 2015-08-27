/** @file String_Get_Size.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned int StringGetSize(char *String)
{
	unsigned int Size = 0;
	
	while (*String != 0)
	{
		Size++;
		String++;
	}
	
	return Size;
}