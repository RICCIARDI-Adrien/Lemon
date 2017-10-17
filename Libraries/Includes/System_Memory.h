/** @file System_Memory.h
 * Various memory manipulation functions.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_MEMORY_H
#define H_SYSTEM_MEMORY_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Fill a memory area with specified value.
 * @param Pointer_Area The memory area starting address.
 * @param Bytes_Count How many bytes to fill.
 * @param Value The value to put in each byte of the memory area.
 */
void SystemMemorySetAreaValue(void *Pointer_Area, unsigned int Bytes_Count, unsigned char Value);

/** Copy an area content into another area.
 * @param Pointer_Source_Area The area to copy from.
 * @param Pointer_Destination_Area The area to copy to.
 * @param Bytes_Count How many bytes to copy.
 * @warning Overlapping areas may lead to an unpredictable result.
 */
void MemoryCopyArea(void *Pointer_Source_Area, void *Pointer_Destination_Area, unsigned int Bytes_Count);

#endif