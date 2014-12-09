/** @file Tests_File.c
 * Test the Libraries File API.
 * @author Adrien RICCIARDI
 * @version 1.0 : 07/12/2014
 */
#include <System.h>
#include "crc32.h"
#include "Tests.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** The buffer size in bytes. */
#define TESTS_FILE_BUFFER_SIZE (4 * 1024 * 1024)

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Store the file data. */
static unsigned char Buffer[TESTS_FILE_BUFFER_SIZE];

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Check file related system calls.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestsFileSystemCalls(void)
{
	unsigned int File_Size_Bytes, Written_Data_CRC, Read_Data_CRC, i, CRC_Seed, File_Descriptor, Read_Bytes_Count;
	int Return_Value;

	// Choose a random file size between 100 KB and 3 MB
	File_Size_Bytes = ((RandomGenerateNumber() % 32) + 1) * 1024 * 100;
	ScreenWriteString("File size : ");
	ScreenWriteUnsignedInteger(File_Size_Bytes);
	ScreenWriteCharacter('\n');
 
	ScreenWriteString("Creating file content... ");
	for (i = 0; i < File_Size_Bytes; i++) Buffer[i] = (unsigned char) RandomGenerateNumber();
	ScreenWriteString("done\n");

	ScreenWriteString("Computing CRC... ");
	CRC_Seed = RandomGenerateNumber();
	Written_Data_CRC = crc32(CRC_Seed, Buffer, File_Size_Bytes);
	ScreenWriteString("done (CRC = ");
	ScreenWriteUnsignedInteger(Written_Data_CRC);
	ScreenWriteString(")\n");

	ScreenWriteString("Writing data to file... ");
	// Open the file
	Return_Value = SystemCall(SYSTEM_CALL_FILE_OPEN, 'w', 0, "_test_", &File_Descriptor);
	if (Return_Value != ERROR_CODE_NO_ERROR)
	{
		ScreenWriteString("Error when opening the file in write mode : ");
		ScreenWriteString(TestsGetErrorString(Return_Value));
		ScreenWriteCharacter('\n');
		return 1;
	}
	// Write data
	Return_Value = SystemCall(SYSTEM_CALL_FILE_WRITE, File_Descriptor, File_Size_Bytes, Buffer, NULL);
	if (Return_Value != ERROR_CODE_NO_ERROR)
	{
		ScreenWriteString("Error when writing to the file : ");
		ScreenWriteString(TestsGetErrorString(Return_Value));
		ScreenWriteCharacter('\n');
		return 1;
	}
	// Close the file
	SystemCall(SYSTEM_CALL_FILE_CLOSE, File_Descriptor, 0, NULL, NULL);
	ScreenWriteString("done\n");

	// Flush read buffer
	MemorySetAreaValue(Buffer, File_Size_Bytes, 0);

	ScreenWriteString("Reading data from file... ");
	// Open the file
	Return_Value = SystemCall(SYSTEM_CALL_FILE_OPEN, 'r', 0, "_test_", &File_Descriptor);
	if (Return_Value != ERROR_CODE_NO_ERROR)
	{
		ScreenWriteString("Error when opening the file in read mode : ");
		ScreenWriteString(TestsGetErrorString(Return_Value));
		ScreenWriteCharacter('\n');
		return 1;
	}
	// Read data
	Return_Value = SystemCall(SYSTEM_CALL_FILE_READ, File_Descriptor, File_Size_Bytes, Buffer, &Read_Bytes_Count);
	if (Return_Value != ERROR_CODE_NO_ERROR)
	{
		ScreenWriteString("Error when reading from the file : ");
		ScreenWriteString(TestsGetErrorString(Return_Value));
		ScreenWriteCharacter('\n');
		return 1;
	}
	// Close the file
	SystemCall(SYSTEM_CALL_FILE_CLOSE, File_Descriptor, 0, NULL, NULL);
	ScreenWriteString("done (");
	ScreenWriteUnsignedInteger(Read_Bytes_Count);
	ScreenWriteString(" bytes read)\n");

	ScreenWriteString("Computing CRC... ");
	Read_Data_CRC = crc32(CRC_Seed, Buffer, File_Size_Bytes);
	ScreenWriteString("done (CRC = ");
	ScreenWriteUnsignedInteger(Read_Data_CRC);
	ScreenWriteString(")\n");

	// Delete the file
	Return_Value = FileDelete("_test_");
	if (Return_Value != ERROR_CODE_NO_ERROR)
	{
		ScreenWriteString("Error when deleting the file : ");
		ScreenWriteString(TestsGetErrorString(Return_Value));
		ScreenWriteCharacter('\n');
		return 1;
	}

	if (Read_Data_CRC == Written_Data_CRC) return 0;
	return 1;
}

/** Try to open more than the maximum allowed number of files. The FileOpen() function must fail when the limit is reached.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
static int TestsFileMaximumOpenedFiles(void)
{
	unsigned int File_Descriptors[FILE_MAXIMUM_OPENED_COUNT + 1];
	int i, Return_Value;
	char String_File_Name[FILE_NAME_LENGTH + 1], String_Number[11];
	
	// Open the maximum number of files
	ScreenWriteString("Opening as many files as possible...\n");
	for (i = 0; i < FILE_MAXIMUM_OPENED_COUNT; i++)
	{
		// Create the file name
		StringConvertUnsignedIntegerToString(i, String_Number);
		StringCopy("!*test", String_File_Name);
		StringConcatenate(String_File_Name, String_Number);
		
		// Open the file in write mode so the file has not exist yet
		Return_Value = FileOpen(String_File_Name, 'w', &File_Descriptors[i]);
		if (Return_Value != ERROR_CODE_NO_ERROR)
		{
			ScreenWriteString("Error while opening the files allowed count : ");
			ScreenWriteString(TestsGetErrorString(Return_Value));
			ScreenWriteCharacter('\n');
			return 1;
		}
	}
	
	// Opening one more file
	ScreenWriteString("Opening one more file...\n");
	Return_Value = FileOpen("!!!test", 'w', &File_Descriptors[FILE_MAXIMUM_OPENED_COUNT]);
	if (Return_Value != ERROR_CODE_CANT_OPEN_MORE_FILES)
	{
		ScreenWriteString("Not expected error while opening too much files in the same time : ");
		ScreenWriteString(TestsGetErrorString(Return_Value));
		ScreenWriteCharacter('\n');
		return 1;
	}
	
	// Close all files
	for (i = 0; i < FILE_MAXIMUM_OPENED_COUNT; i++) FileClose(File_Descriptors[i]);
	
	return 0;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int TestsFile(void)
{
	TestsDisplayMessageTestStarting("Libraries File system calls");
	if (TestsFileSystemCalls()) return 1;
	TestsDisplayMessageTestSuccessful();
	
	TestsDisplayMessageTestStarting("System maximum opened files limit");
	if (TestsFileMaximumOpenedFiles()) return 1;
	TestsDisplayMessageTestSuccessful();
	
	return 0;
}