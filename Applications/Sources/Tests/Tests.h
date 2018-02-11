/** @file Tests.h
 * List all available tests.
 * @author Adrien RICCIARDI
 */
#ifndef H_TESTS_H
#define H_TESTS_H

//-------------------------------------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------------------------------------
/** A test function.
 * @return 0 if the test succeeded,
 * @return 1 if an error happened.
 */
typedef int (*TTestFunction)(void);

/** A test content. */
typedef struct
{
	char String_Title[128]; //!< Describe the test goal.
	TTestFunction Function; //!< The test code.
} TTest;

//-------------------------------------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------------------------------------
// File API
/** Check functions input parameters. Not all parameters are tested here as some parameters need special handling and are tested elsewhere.
 * @return 0 if the test was successful,
 * @return 1 if the test failed.
 */
int TestsFileFunctionsInputParameters(void);

/** Check file related system calls.
 * @return 0 if the test was successful,
 * @return 1 if the test failed.
 */
int TestsFileSystemCalls(void);

/** Try to open more than the maximum allowed number of files. The LibrariesFileOpen() function must fail when the limit is reached.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
int TestsFileMaximumOpenedFiles(void);

/** A previously opened file can't be reopened without being closed.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
int TestsFileReopenSameFile(void);

/** Fill all available Files List entries.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
int TestsFileFillFilesList(void);

/** Fill all available Blocks List entries.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
int TestsFileFillBlocksList(void);

/** Check if a previously opened file that goes deleted is really automatically closed.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
int TestsFileCloseDeletedOpenedFile(void);

/** Check the rename function robustness against bad parameters and if renaming a file works.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
int TestsFileRename(void);

// Memory API
/** Copy a small amount of data.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
int TestsMemoryCopySmallArea(void);

/** Copy a big amount of data.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
int TestsMemoryCopyBigArea(void);

/** Set a small amount of data value.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
int TestsMemorySetSmallAreaValue(void);

/** Set a big amount of data value.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
int TestsMemorySetBigAreaValue(void);

// String API
/** Test the StringCompare() function.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
int TestsStringCompare(void);

/** Test the StringConcatenate() function.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
int TestsStringConcatenate(void);

/** Test the StringConcatenateUpToNumber() function.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
int TestsStringConcatenateUpToNumber(void);

/** Test the StringConvertStringToUnsignedInteger() function.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
int TestsStringConvertStringToUnsignedInteger(void);

/** Test the StringConvertUnsignedIntegerToString() function.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
int TestsStringConvertUnsignedIntegerToString(void);

/** Test the StringConvertIntegerToHexadecimal() function.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
int TestsStringConvertIntegerToHexadecimal(void);

/** Test the StringCopy() function.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
int TestsStringCopy(void);

/** Test the StringGetSize() function.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
int TestsStringGetSize(void);

#endif
