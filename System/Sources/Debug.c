/** @file Debug.c
 * See Debug.h for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Debug.h>
#include <Drivers/Driver_Screen.h>

// Debug functions are available only when debug is enabled
#if CONFIGURATION_IS_DEBUG_ENABLED

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void DebugWriteHexadecimalByte(unsigned char Byte)
{
	unsigned char Nibble;
	
	// Convert the high nibble to ASCII
	Nibble = Byte >> 4;
	if (Nibble < 10) Nibble += '0';
	else Nibble += 'A' - 10; // Don't forget that 0x0A is decimal 10...
	ScreenWriteCharacter(Nibble);
	
	// Convert the low nibble to ASCII
	Nibble = Byte & 0x0F;
	if (Nibble < 10) Nibble += '0';
	else Nibble += 'A' - 10;
	ScreenWriteCharacter(Nibble);
}

void DebugWriteHexadecimalInteger(unsigned int Integer)
{
	unsigned int i, Nibble;
	
	// Display each nibble value
	for (i = 0; i < sizeof(Integer) * 2; i++)
	{
		// Get the most significant nibble
		Nibble = Integer >> 28;
		// Shift to left to prepare for the next most significant nibble
		Integer <<= 4;
		
		// Convert the nibble to ASCII
		if (Nibble < 10) Nibble += '0';
		else Nibble += 'A' - 10; // Don't forget that 0x0A is decimal 10...
		
		// Display it
		ScreenWriteCharacter(Nibble);
	}
}

#endif