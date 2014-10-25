/** @file Wrappers.c
 * Linux wrapper for some Lemon functionalities.
 * @author Adrien RICCIARDI
 * @version 1.0 : 14/04/2014
 */
#ifndef H_WRAPPERS_H
#define H_WRAPPERS_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Set to 1 to enable unit tests (in this case a main() function is compiled). */
#define WRAPPERS_ENABLE_UNIT_TESTS 0

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Do all the initialization stuff needed by the various wrappers.
 * @note This function must be called once prior any other wrapper function.
 */
void WrappersInitialize(void);

#endif
