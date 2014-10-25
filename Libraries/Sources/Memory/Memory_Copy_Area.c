/** @file Memory_Copy_Area.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/10/2014
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MemoryCopyArea(void *Pointer_Source_Area, void *Pointer_Destination_Area, unsigned int Bytes_Count)
{
	unsigned char *Pointer_Source_Area_Bytes, *Pointer_Destination_Area_Bytes;
	
	Pointer_Source_Area_Bytes = Pointer_Source_Area;
	Pointer_Destination_Area_Bytes = Pointer_Destination_Area;
	
	while (Bytes_Count > 0)
	{
		*Pointer_Destination_Area_Bytes = *Pointer_Source_Area_Bytes;
		Pointer_Source_Area_Bytes++;
		Pointer_Destination_Area_Bytes++;
		Bytes_Count--;
	}
}