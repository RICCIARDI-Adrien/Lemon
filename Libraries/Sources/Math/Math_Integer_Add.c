/** @file Math_Integer_Add.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void MathIntegerAdd(PMathInteger Operand_1, PMathInteger Operand_2, PMathInteger Result)
{
	asm
	(
		/* Cache all needed values */
		"mov esi, %0\n"
		"mov ebx, %1\n"
		"mov edi, %2\n"
		
		/* Initialize the loop counter */
		"mov ecx, %3\n"
		
		/* Clear the carry flag before the first addition */
		"clc\n"
		
	"Loop_Addition:\n"
		"lodsd\n"
		"adc eax, DWORD PTR [ebx]\n"
		"stosd\n"
		/** Add 4 to ebx without affecting the carry flag */
		"inc ebx\n"
		"inc ebx\n"
		"inc ebx\n"
		"inc ebx\n"
		"loop Loop_Addition\n"
		
		: // No output register
		: "m" (Operand_1), "m" (Operand_2), "m" (Result), "i" (MATH_INTEGER_SIZE_PARTS)
		: "eax", "ebx", "ecx", "esi", "edi"
	);
}