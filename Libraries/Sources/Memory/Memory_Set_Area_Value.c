/** @file Memory_Set_Area_Value.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/10/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MemorySetAreaValue(void *Pointer_Area, unsigned int Bytes_Count, unsigned char Value)
{
	unsigned char *Pointer_Area_Bytes;
	
	Pointer_Area_Bytes = Pointer_Area;
	
	while (Bytes_Count > 0)
	{
		*Pointer_Area_Bytes = Value;
		Pointer_Area_Bytes++;
		Bytes_Count--;
	}
}