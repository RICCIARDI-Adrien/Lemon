/** @file System_Random.h
 * Generate pseudo-random numbers.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_RANDOM_H
#define H_SYSTEM_RANDOM_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the random generator with the current timer value. */
void SystemRandomInitialize(void);

/** Generate a pseudo-random number.
 * @return A pseudo-random number.
 */
int RandomGenerateNumber(void);

#endif
