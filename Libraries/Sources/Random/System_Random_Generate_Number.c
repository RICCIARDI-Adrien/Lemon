/** @file Libraries_Random_Generate_Number.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
/** The seed shared with the random initialization function. */
unsigned int Libraries_Random_Libraries_Seed;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesRandomGenerateNumber(void)
{
	Libraries_Random_Libraries_Seed = Libraries_Random_Libraries_Seed * 1103515245 + 12345; // Based on OpenBSD rand serie
	return Libraries_Random_Libraries_Seed % 2147483648UL;
}
