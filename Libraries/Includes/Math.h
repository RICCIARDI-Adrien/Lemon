/** @file Math.h
 * Allow calculations on a configurable number of bits.
 * @author Adrien RICCIARDI
 */
#ifndef H_MATH_H
#define H_MATH_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The number of "parts" in an integer number. A part is 32-bit long, so to have a 128-bit integer set the value 4 to this constant. */
#define MATH_INTEGER_SIZE_PARTS 1024

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** An integer number (always signed, stored in little endian). */
typedef union
{
	int Parts[MATH_INTEGER_SIZE_PARTS];
	unsigned char Bytes[MATH_INTEGER_SIZE_PARTS * 4]; // A part is 4-bytes wide
} TMathInteger, *PMathInteger;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize a TMathInteger from a 32-bit signed value.
 * @param Initialization_Value The initialization value.
 * @param Result A valid address of the TMathInteger to initialize.
 */
void MathIntegerInitializeFromInt(int Initialization_Value, PMathInteger Result);

/** Add two signed operands and store the result in a third number (which can be one of the two operands).
 * @param Operand_1 The first operand to add.
 * @param Operand_2 The second operand to add.
 * @param Result Where to store the operation result.
 */
void MathIntegerAdd(PMathInteger Operand_1, PMathInteger Operand_2, PMathInteger Result);

/** Display the operand on the screen as an hexadecimal value. There is no size limit on the operand to display.
 * @param Operand The operand to display.
 */
void MathIntegerDisplayHexadecimal(PMathInteger Operand);

/** Copy an integer number content to another.
 * @param Source_Operand The number to copy.
 * @param Destination_Operand Where to copy the number.
 */ 
void MathIntegerCopy(PMathInteger Source_Operand, PMathInteger Destination_Operand);

#endif