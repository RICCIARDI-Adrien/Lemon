/** @file Random_Generate_Number.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
/** The seed shared with the random initialization function. */
unsigned int Random_Libraries_Seed;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int RandomGenerateNumber(void)
{
	Random_Libraries_Seed = Random_Libraries_Seed * 1103515245 + 12345; // Based on OpenBSD rand serie
	return Random_Libraries_Seed % 2147483648UL;
}