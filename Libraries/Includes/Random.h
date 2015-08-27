/** @file Random.h
 * Generate pseudo-random numbers.
 * @author Adrien RICCIARDI
 */
#ifndef H_RANDOM_H
#define H_RANDOM_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the random generator with the current timer value. */
void RandomInitialize(void);

/** Generate a pseudo-random number.
 * @return A pseudo-random number.
 */
int RandomGenerateNumber(void);

#endif