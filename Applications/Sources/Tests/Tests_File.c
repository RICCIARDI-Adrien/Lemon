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
	int Return_Code;

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
	Return_Code = SystemCall(SYSTEM_CALL_FILE_OPEN, 'w', 0, "_test_", &File_Descriptor);
	if (Return_Code != ERROR_CODE_NO_ERROR)
	{
		ScreenWriteString("Error when opening the file in write mode : ");
		ScreenWriteString(TestsGetErrorString(Return_Code));
		ScreenWriteCharacter('\n');
		return 0;
	}
	// Write data
	Return_Code = SystemCall(SYSTEM_CALL_FILE_WRITE, File_Descriptor, File_Size_Bytes, Buffer, NULL);
	if (Return_Code != ERROR_CODE_NO_ERROR)
	{
		ScreenWriteString("Error when writing to the file : ");
		ScreenWriteString(TestsGetErrorString(Return_Code));
		ScreenWriteCharacter('\n');
		return 0;
	}
	// Close the file
	SystemCall(SYSTEM_CALL_FILE_CLOSE, File_Descriptor, 0, NULL, NULL);
	ScreenWriteString("done\n");

	// Flush read buffer
	MemorySetAreaValue(Buffer, File_Size_Bytes, 0);

	ScreenWriteString("Reading data from file... ");
	// Open the file
	Return_Code = SystemCall(SYSTEM_CALL_FILE_OPEN, 'r', 0, "_test_", &File_Descriptor);
	if (Return_Code != ERROR_CODE_NO_ERROR)
	{
		ScreenWriteString("Error when opening the file in read mode : ");
		ScreenWriteString(TestsGetErrorString(Return_Code));
		ScreenWriteCharacter('\n');
		return 0;
	}
	// Read data
	Return_Code = SystemCall(SYSTEM_CALL_FILE_READ, File_Descriptor, File_Size_Bytes, Buffer, &Read_Bytes_Count);
	if (Return_Code != ERROR_CODE_NO_ERROR)
	{
		ScreenWriteString("Error when reading from the file : ");
		ScreenWriteString(TestsGetErrorString(Return_Code));
		ScreenWriteCharacter('\n');
		return 0;
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
	Return_Code = FileDelete("_test_");
	if (Return_Code != ERROR_CODE_NO_ERROR)
	{
		ScreenWriteString("Error when deleting the file : ");
		ScreenWriteString(TestsGetErrorString(Return_Code));
		ScreenWriteCharacter('\n');
		return 0;
	}

	if (Read_Data_CRC == Written_Data_CRC) return 1;
	return 0;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int TestsFile(void)
{
	if (TestsFileSystemCalls()) return 1;
	return 0;
}