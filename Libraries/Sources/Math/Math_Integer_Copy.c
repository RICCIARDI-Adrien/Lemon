/** @file Math_Integer_Copy.c
 * @author Adrien RICCIARDI
 * @version 1.0 : 04/01/2015
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MathIntegerCopy(PMathInteger Source_Operand, PMathInteger Destination_Operand)
{
	unsigned int i;
	
	for (i = 0; i < MATH_INTEGER_SIZE_PARTS; i++) Destination_Operand->Parts[i] = Source_Operand->Parts[i];
}