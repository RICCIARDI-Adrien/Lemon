/** @file Tests_Memory.c
 * Automatic tests on the Memory functions of the Libraries.
 * @author Adrien RICCIARDI
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
static unsigned char Tests_Memory_Source_Buffer[TESTS_MEMORY_BUFFER_SIZE], Tests_Memory_Destination_Buffer[TESTS_MEMORY_BUFFER_SIZE];

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
			SystemScreenWriteString("Error : the two area are different at offset ");
			SystemScreenWriteUnsignedInteger(Offset);
			SystemScreenWriteString(".\n");
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
			SystemScreenWriteString("Error : the byte at offset ");
			SystemScreenWriteUnsignedInteger(Offset);
			SystemScreenWriteString(" has a bad value.\n");
			
			SystemScreenWriteString("Read value : ");
			SystemScreenWriteUnsignedInteger(Pointer_Memory_Area[Offset]);
			SystemScreenWriteString("\nExpected value : ");
			SystemScreenWriteUnsignedInteger(Value);
			SystemScreenWriteCharacter('\n');
			return 0;
		}
	}
	return 1;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int TestsMemoryCopySmallArea(void)
{
	int i;

	// Use a small area (< 4 bytes) to bypass the "copy by 4 bytes" part of the function
	for (i = 0; i < 3; i++) Tests_Memory_Source_Buffer[i] = (unsigned char) SystemRandomGenerateNumber(); // Fill the buffer with random data
	SystemMemoryCopyArea(Tests_Memory_Source_Buffer, Tests_Memory_Destination_Buffer, 3);
	
	if (!TestsMemoryAreAreasEqual(Tests_Memory_Source_Buffer, Tests_Memory_Destination_Buffer, 3)) return 1;
	return 0;
}

int TestsMemoryCopyBigArea(void)
{
	int i;
	
	// Use a big area to test all parts of the function
	for (i = 0; i < TESTS_MEMORY_BUFFER_SIZE; i++) Tests_Memory_Source_Buffer[i] = (unsigned char) SystemRandomGenerateNumber(); // Fill the buffer with random data
	SystemMemoryCopyArea(Tests_Memory_Source_Buffer, Tests_Memory_Destination_Buffer, TESTS_MEMORY_BUFFER_SIZE);
	
	if (!TestsMemoryAreAreasEqual(Tests_Memory_Source_Buffer, Tests_Memory_Destination_Buffer, TESTS_MEMORY_BUFFER_SIZE)) return 1;
	return 0;
}

int TestsMemorySetSmallAreaValue(void)
{
	unsigned char Value;
	
	// Use a small area (< 4 bytes) to bypass the "copy by 4 bytes" part of the function
	Value = (unsigned char) SystemRandomGenerateNumber(); // Get a random value
	SystemMemorySetAreaValue(Tests_Memory_Source_Buffer, 3, Value);
	
	if (!TestsMemoryCheckAreaValue(Tests_Memory_Source_Buffer, Value, 3)) return 1;
	return 0;
}

int TestsMemorySetBigAreaValue(void)
{
	unsigned char Value;
	
	// Use a big area to test all parts of the function
	Value = (unsigned char) SystemRandomGenerateNumber(); // Get a random value
	SystemMemorySetAreaValue(Tests_Memory_Source_Buffer, TESTS_MEMORY_BUFFER_SIZE, Value);
	
	if (!TestsMemoryCheckAreaValue(Tests_Memory_Source_Buffer, Value, TESTS_MEMORY_BUFFER_SIZE)) return 1;
	return 0;
}