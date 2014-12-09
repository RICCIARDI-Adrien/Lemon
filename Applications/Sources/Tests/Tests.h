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
/** Get the text signification of a Lemon-specific error code.
 * @param Error_Code The error code.
 * @return The corresponding string.
 */
char *TestsGetErrorString(int Error_Code);

/** Display the next test to do.
 * @param String_Message The message to display.
 */
void TestsDisplayMessageTestStarting(char *String_Message);

/** Display a success message. */
void TestsDisplayMessageTestSuccessful(void);

/** Halt the program after displaying an error message. */
void TestsDisplayMessageTestFailed(void);

/** Test all Libraries File functions.
 * @return 0 if the tests were successful,
 * @return 1 if a test failed.
 */
int TestsFile(void);

/** Test the Libraries Memory functions.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
int TestsMemory(void);

#endif
