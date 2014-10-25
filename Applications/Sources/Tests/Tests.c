/** @file Tests.c
 * Contain all automated tests that can be done on the system.
 * @author Adrien RICCIARDI
 * @version 1.0 : 20/05/2014
 * @version 1.1 : 24/05/2014, added FileRead() / FileWrite() test.
 * @version 1.2 : 11/06/2014, added ctype tests.
 * @version 1.3 : 12/06/2014, added assert() tests.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <System.h>
#include <time.h>
#include "crc32.h"
#include "Tests.h"

//-------------------------------------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------------------------------------
static char *Error_Code_Strings[] =
{
	"No error",
	"Files List is full",
	"Block Allocation Table is full",
	"Bad file name",
	"File not found",
	"File already existing",
	"Can't open more files",
	"File opened yet",
	"Unknown file opening mode",
	"The file is not opened",
	"Bad file opening mode",
	"Bad file descriptor (greater than the maximum number of simultaneous opened files)",
	"Bad UART parameters",
	"File to download is larger than RAM",
	"File not executable"
};

//-------------------------------------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Get the text signification of a Lemon-specific error code.
 * @param Error_Code The error code.
 * @return The corresponding string.
 */
static char *ErrorCodeToString(int Error_Code)
{
	return Error_Code_Strings[Error_Code];
}

/** Display the next test to do.
 * @param String_Message The message to display.
 */
static void MessageStartingTest(char *String_Message)
{
	printf("--- Testing %s ---\n", String_Message);
}

/** Display a success message. */
static void MessageTestSuccessful(void)
{
	ScreenSetFontColor(SCREEN_COLOR_GREEN);
	printf("Test successful\n\n");
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
}

/** Halt the program after displaying an error message. */
static void MessageTestFailed(void)
{
	ScreenSetFontColor(SCREEN_COLOR_RED);
	printf("Test failed\n");
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
	exit(EXIT_FAILURE);
}

/** Check FileRead() and FileWrite() system functions.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestFileSystemFileReadFileWrite(void)
{
	unsigned int File_Size_Bytes, Written_Data_CRC, Read_Data_CRC, i, CRC_Seed, File_Descriptor, Read_Bytes_Count;
	int Return_Code;
	static unsigned char Buffer[4 * 1024 * 1024]; // Avoid allocating it on the stack
	
	// Choose a random file size between 100 KB and 3 MB
	File_Size_Bytes = ((rand() % 32) + 1) * 1024 * 100;
	printf("File size : %u\n", File_Size_Bytes);

	printf("Creating file content... ");
	for (i = 0; i < File_Size_Bytes; i++) Buffer[i] = rand() % 256;
	printf("done\n");

	printf("Computing CRC... ");
	CRC_Seed = rand();
	Written_Data_CRC = crc32(CRC_Seed, Buffer, File_Size_Bytes);
	printf("done (CRC = %u)\n", Written_Data_CRC);
	
	printf("Writing data to file... ");
	// Open the file
	Return_Code = SystemCall(SYSTEM_CALL_FILE_OPEN, 'w', 0, "_test_", (char *) &File_Descriptor);
	if (Return_Code != ERROR_CODE_NO_ERROR)
	{
		printf("Error when opening the file in write mode : %s\n", ErrorCodeToString(Return_Code));
		return 0;
	}
	// Write data
	Return_Code = SystemCall(SYSTEM_CALL_FILE_WRITE, File_Descriptor, File_Size_Bytes, Buffer, NULL);
	if (Return_Code != ERROR_CODE_NO_ERROR)
	{
		printf("Error when writing to the file : %s\n", ErrorCodeToString(Return_Code));
		return 0;
	}
	// Close the file
	SystemCall(SYSTEM_CALL_FILE_CLOSE, File_Descriptor, 0, NULL, NULL);
	printf("done\n");
	
	// Flush read buffer
	memset(Buffer, 0, File_Size_Bytes);

	printf("Reading data from file... ");
	// Open the file
	Return_Code = SystemCall(SYSTEM_CALL_FILE_OPEN, 'r', 0, "_test_", (char *) &File_Descriptor);
	if (Return_Code != ERROR_CODE_NO_ERROR)
	{
		printf("Error when opening the file in read mode : %s\n", ErrorCodeToString(Return_Code));
		return 0;
	}
	// Read data
	Return_Code = SystemCall(SYSTEM_CALL_FILE_READ, File_Descriptor, File_Size_Bytes, Buffer, &Read_Bytes_Count);
	if (Return_Code != ERROR_CODE_NO_ERROR)
	{
		printf("Error when reading from the file : %s\n", ErrorCodeToString(Return_Code));
		return 0;
	}
	// Close the file
	SystemCall(SYSTEM_CALL_FILE_CLOSE, File_Descriptor, 0, NULL, NULL);
	printf("done (%u bytes read)\n", Read_Bytes_Count);
	
	printf("Computing CRC... ");
	Read_Data_CRC = crc32(CRC_Seed, Buffer, File_Size_Bytes);
	printf("done (CRC = %u)\n", Read_Data_CRC);

	// Delete the file
	remove("_test_");
	
	if (Read_Data_CRC == Written_Data_CRC) return 1;
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	// Launch the assert() test if requested
	if ((argc == 2) && (strcmp(argv[1], "-assert") == 0)) TestsAssert();

	srand(time(NULL));
	
	printf("### Automated tests ###\n\n");

	MessageStartingTest("ctype standard library");
	if (TestsCtype()) MessageTestSuccessful();
	else MessageTestFailed();

	MessageStartingTest("FileRead() / FileWrite() system calls");
	if (TestFileSystemFileReadFileWrite()) MessageTestSuccessful();
	else MessageTestFailed();

	return EXIT_SUCCESS;
}
