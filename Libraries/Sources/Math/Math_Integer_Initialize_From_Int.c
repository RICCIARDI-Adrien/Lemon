/** @file Math_Integer_Initialize_From_Int.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MathIntegerInitializeFromInt(int Initialization_Value, PMathInteger Result)
{
	int i, Higher_Parts_Value;
	
	// Initialize the least significant part
	Result->Parts[0] = Initialization_Value;
	
	// Propagate the sign bit to the other parts
	if (Initialization_Value < 0) Higher_Parts_Value = -1; // A negative number has all upper bits set to '1'
	else Higher_Parts_Value = 0;
	for (i = 1; i < MATH_INTEGER_SIZE_PARTS; i++) Result->Parts[i] = Higher_Parts_Value;
}