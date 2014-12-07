/** @file Memory_Copy_Area.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/10/2014
 * @version 1.1 : 06/12/2014, improved function speed by copying 4 bytes at a time.
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MemoryCopyArea(void *Pointer_Source_Area, void *Pointer_Destination_Area, unsigned int Bytes_Count)
{
	unsigned int *Pointer_Source_Area_Dword, *Pointer_Destination_Area_Dword;
	unsigned char *Pointer_Source_Area_Byte, *Pointer_Destination_Area_Byte;
	
	// Copy 4 bytes at a time to be faster
	Pointer_Source_Area_Dword = Pointer_Source_Area;
	Pointer_Destination_Area_Dword = Pointer_Destination_Area;
	
	while (Bytes_Count > sizeof(unsigned int))
	{
		*Pointer_Destination_Area_Dword = *Pointer_Source_Area_Dword;
		Pointer_Source_Area_Dword++;
		Pointer_Destination_Area_Dword++;
		Bytes_Count -= sizeof(unsigned int);
	}
	
	// Copy the remaining bytes
	Pointer_Source_Area_Byte = (unsigned char *) Pointer_Source_Area_Dword;
	Pointer_Destination_Area_Byte = (unsigned char *) Pointer_Destination_Area_Dword;
	
	while (Bytes_Count > 0)
	{
		*Pointer_Destination_Area_Byte = *Pointer_Source_Area_Byte;
		Pointer_Source_Area_Byte++;
		Pointer_Destination_Area_Byte++;
		Bytes_Count--;
	}
}