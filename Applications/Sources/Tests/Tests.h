/** @file Tests.h
 * List all available tests.
 * @author Adrien RICCIARDI
 * @version 1.0 : 09/06/2014
 */
#ifndef H_TESTS_H
#define H_TESTS_H

//-------------------------------------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Check the assert() macro.
 * @note The program will crash if the test is successful.
 */
void TestsAssert(void);

/** Check the whole ctype library.
 * @return 1 if the tests were successful,
 * @return 0 if a test failed.
 */
int TestsCtype(void);

#endif
