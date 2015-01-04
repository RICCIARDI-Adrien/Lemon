/** @file Math_Integer_Display_Hexadecimal.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 04/01/2015
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MathIntegerDisplayHexadecimal(PMathInteger Operand)
{
	int Current_Part, i, Nibble_Shift_Amount;
	char Nibble; // A 4-bit hexadecimal character
	
	// Parse all parts starting from the most significant one
	for (i = MATH_INTEGER_SIZE_PARTS - 1; i >= 0; i--)
	{
		// Extract the next most significant part
		Current_Part = Operand->Parts[i];
		
		// Extract all nibbles from the part
		for (Nibble_Shift_Amount = 28; Nibble_Shift_Amount >= 0; Nibble_Shift_Amount -= 4)
		{
			// Get the next nibble
			Nibble = (Current_Part >> Nibble_Shift_Amount) & 0x0F;
			
			// Convert the nibble to an ASCII character
			if (Nibble < 10) Nibble += '0'; // Digit
			else Nibble += 'A' - 10; // Letter
			
			// Display the nibble
			ScreenWriteCharacter(Nibble);
		}
	}
}