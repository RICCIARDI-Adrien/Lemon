/** @file Tests_Memory.c
 * Automatic tests on the Memory functions of the Libraries.
 * @author Adrien RICCIARDI
 * @version 1.0 : 07/12/2014
 */
#include <System.h>
#include "Display_Message.h"
#include "Tests.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Each buffer size in bytes. */
#define TESTS_MEMORY_BUFFER_SIZE ((2 * 1024 * 1024) + 3) // This number must not be aligned on 4-byte boundary to allow testing the "bytes copy" part of the TestsMemoryAreAreasEqual() function.

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
static unsigned char Source_Buffer[TESTS_MEMORY_BUFFER_SIZE], Destination_Buffer[TESTS_MEMORY_BUFFER_SIZE];

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Compare two memory areas.
 * @param Pointer_Memory_Area_1 The first area to compare.
 * @param Pointer_Memory_Area_2 The second area to compare.
 * @param Bytes_Count How many bytes to compare.
 * @return 1 if the two areas are equal,
 * @return 0 if the two areas are different.
 */
static int TestsMemoryAreAreasEqual(unsigned char *Pointer_Memory_Area_1, unsigned char *Pointer_Memory_Area_2, unsigned int Bytes_Count)
{
	unsigned int Offset;
	
	for (Offset = 0; Offset < Bytes_Count; Offset++)
	{
		if (Pointer_Memory_Area_1[Offset] != Pointer_Memory_Area_2[Offset])
		{
			ScreenWriteString("Error : the two area are different at offset ");
			ScreenWriteUnsignedInteger(Offset);
			ScreenWriteString(".\n");
			return 0;
		}
	}
	return 1;
}

/** Check if all the bytes of a memory area have the same value.
 * @param *Pointer_Memory_Area The area to check.
 * @param Value The value to check against.
 * @param Bytes_Count How many bytes to check.
 * @return 1 if all the bytes have the right value,
 * @return 0 if a byte has a wrong value.
 */
static int TestsMemoryCheckAreaValue(unsigned char *Pointer_Memory_Area, unsigned char Value, unsigned int Bytes_Count)
{
	unsigned int Offset;
	
	for (Offset = 0; Offset < Bytes_Count; Offset++)
	{
		if (Pointer_Memory_Area[Offset] != Value)
		{
			ScreenWriteString("Error : the byte at offset ");
			ScreenWriteUnsignedInteger(Offset);
			ScreenWriteString(" has a bad value.\n");
			
			ScreenWriteString("ar : ");
			ScreenWriteUnsignedInteger(Pointer_Memory_Area[Offset]);
			ScreenWriteString("\nval : ");
			ScreenWriteUnsignedInteger(Value);
			ScreenWriteString("\n");
			return 0;
		}
	}
	return 1;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int TestsMemory(void)
{
	unsigned int i;
	unsigned char Value;
	
	DisplayMessageTestStarting("Libraries Memory API");

	ScreenWriteString("MemoryCopyArea()...\n");
	// Use a small area (< 4 bytes) to bypass the "copy by 4 bytes" part of the function
	ScreenWriteString("Testing MemoryCopyArea() with a small area size...\n");
	for (i = 0; i < 3; i++) Source_Buffer[i] = (unsigned char) RandomGenerateNumber(); // Fill the buffer with random data
	MemoryCopyArea(Source_Buffer, Destination_Buffer, 3);
	if (!TestsMemoryAreAreasEqual(Source_Buffer, Destination_Buffer, 3)) return 1;
	
	// Use a big area to test all parts of the function
	ScreenWriteString("Testing MemoryCopyArea() with a big area size...\n");
	for (i = 0; i < TESTS_MEMORY_BUFFER_SIZE; i++) Source_Buffer[i] = (unsigned char) RandomGenerateNumber(); // Fill the buffer with random data
	MemoryCopyArea(Source_Buffer, Destination_Buffer, TESTS_MEMORY_BUFFER_SIZE);
	if (!TestsMemoryAreAreasEqual(Source_Buffer, Destination_Buffer, TESTS_MEMORY_BUFFER_SIZE)) return 1;
	
	ScreenWriteString("MemorySetAreaValue()...\n");
	// Use a small area (< 4 bytes) to bypass the "copy by 4 bytes" part of the function
	ScreenWriteString("Testing MemorySetAreaValue() with a small area size...\n");
	Value = (unsigned char) RandomGenerateNumber(); // Get a random value
	MemorySetAreaValue(Source_Buffer, 3, Value);
	if (!TestsMemoryCheckAreaValue(Source_Buffer, Value, 3)) return 1;
	
	// Use a big area to test all parts of the function
	ScreenWriteString("Testing MemorySetAreaValue() with a big area size...\n");
	Value = (unsigned char) RandomGenerateNumber(); // Get a random value
	MemorySetAreaValue(Source_Buffer, TESTS_MEMORY_BUFFER_SIZE, Value);
	if (!TestsMemoryCheckAreaValue(Source_Buffer, Value, TESTS_MEMORY_BUFFER_SIZE)) return 1;
	
	DisplayMessageTestSuccessful();
	return 0;
}