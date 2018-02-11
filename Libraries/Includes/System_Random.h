/** @file Libraries_Random.h
 * Generate pseudo-random numbers.
 * @author Adrien RICCIARDI
 */
#ifndef H_LIBRARIES_RANDOM_H
#define H_LIBRARIES_RANDOM_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the random generator with the current timer value. */
void LibrariesRandomInitialize(void);

/** Generate a pseudo-random number.
 * @return A pseudo-random number.
 */
int LibrariesRandomGenerateNumber(void);

#endif
