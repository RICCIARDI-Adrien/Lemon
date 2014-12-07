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
	unsigned int *Pointer_Area_Dword, Value_Dword;
	unsigned char *Pointer_Area_Byte;
	
	// Set the value by 4 bytes at a time to be faster
	Value_Dword = (Value << 24) | (Value << 16) | (Value << 8) | Value; // Extend the value on 32 bits
	Pointer_Area_Dword = Pointer_Area;
	while (Bytes_Count > sizeof(unsigned int))
	{
		*Pointer_Area_Dword = Value_Dword;
		Pointer_Area_Dword++;
		Bytes_Count -= sizeof(unsigned int);
	}
	
	// Set the value of the remaining bytes
	Pointer_Area_Byte = (unsigned char *) Pointer_Area_Dword;
	while (Bytes_Count > 0)
	{
		*Pointer_Area_Byte = Value;
		Pointer_Area_Byte++;
		Bytes_Count--;
	}
}