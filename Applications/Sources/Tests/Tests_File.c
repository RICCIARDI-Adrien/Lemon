/** @file Tests_File.c
 * Test the Libraries File API.
 * @author Adrien RICCIARDI
 * @version 1.0 : 07/12/2014
 */
#include <System.h>
#include "crc32.h"
#include "Display_Message.h"
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
/** Check functions input parameters. Not all parameters are tested here as some parameters need special handling and are tested elsewhere.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestsFileFunctionsInputParameters(void)
{
	unsigned int File_ID;
	int Return_Value;
	
	// Testing FileOpen()
	// Bad filename (empty string, checked by kernel)
	ScreenWriteString("Checking FileOpen()...\n");
	Return_Value = FileOpen("\0", 'r', &File_ID);
	if (Return_Value != ERROR_CODE_BAD_FILE_NAME)
	{
		DisplayMessageErrorAndCode("when given a bad file name", Return_Value);
		return 1;
	}
	// Bad filename (NULL string, checked by Libraries)
	Return_Value = FileOpen(NULL, 'r', &File_ID);
	if (Return_Value != ERROR_CODE_BAD_FILE_NAME)
	{
		DisplayMessageErrorAndCode("when given a NULL file name", Return_Value);
		return 1;
	}
	// File not found
	Return_Value = FileOpen("_!azerty", 'r', &File_ID);
	if (Return_Value != ERROR_CODE_FILE_NOT_FOUND)
	{
		DisplayMessageErrorAndCode("when given an unexisting file", Return_Value);
		return 1;
	}
	// Unknown opening mode
	Return_Value = FileOpen("Ignored", 't', &File_ID);
	if (Return_Value != ERROR_CODE_UNKNOWN_OPENING_MODE)
	{
		DisplayMessageErrorAndCode("when given a bad opening mode", Return_Value);
		return 1;
	}

	// TODO
	// Testing FileRead()
	// Open a file 
	// Bad file ID
	/*ScreenWriteString("Checking FileRead()...\n");
	Return_Value = FileRead(123456, );
	if (Return_Value != ERROR_CODE_FILE_NOT_FOUND)
	{
		DisplayMessageErrorAndCode("when given an unexisting file", Return_Value);
		return 1;
	}*/
	// Bad opening mode
	
	return 0;
}

/** Check file related system calls.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestsFileSystemCalls(void)
{
	unsigned int File_Size_Bytes, Written_Data_CRC, Read_Data_CRC, i, CRC_Seed, File_ID, Read_Bytes_Count;
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
	Return_Value = SystemCall(SYSTEM_CALL_FILE_OPEN, 'w', 0, "_test_", &File_ID);
	if (Return_Value != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("when opening the file in write mode", Return_Value);
		return 1;
	}
	// Write data
	Return_Value = SystemCall(SYSTEM_CALL_FILE_WRITE, File_ID, File_Size_Bytes, Buffer, NULL);
	if (Return_Value != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("when writing to the file", Return_Value);
		return 1;
	}
	// Close the file
	SystemCall(SYSTEM_CALL_FILE_CLOSE, File_ID, 0, NULL, NULL);
	ScreenWriteString("done\n");

	// Flush read buffer
	MemorySetAreaValue(Buffer, File_Size_Bytes, 0);

	ScreenWriteString("Reading data from file... ");
	// Open the file
	Return_Value = SystemCall(SYSTEM_CALL_FILE_OPEN, 'r', 0, "_test_", &File_ID);
	if (Return_Value != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("when opening the file in read mode", Return_Value);
		return 1;
	}
	// Read data
	Return_Value = SystemCall(SYSTEM_CALL_FILE_READ, File_ID, File_Size_Bytes, Buffer, &Read_Bytes_Count);
	if (Return_Value != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("when reading from the file", Return_Value);
		return 1;
	}
	// Close the file
	SystemCall(SYSTEM_CALL_FILE_CLOSE, File_ID, 0, NULL, NULL);
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
		DisplayMessageErrorAndCode("when deleting the file", Return_Value);
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
	unsigned int File_IDs[FILE_MAXIMUM_OPENED_COUNT + 1];
	int i, Return_Value;
	char String_File_Name[FILE_NAME_LENGTH + 1], String_Number[11];
	
	// Open the maximum number of files
	ScreenWriteString("Opening as many files as possible...\n");
	for (i = 0; i < FILE_MAXIMUM_OPENED_COUNT; i++)
	{
		// Create the file name
		StringConvertUnsignedIntegerToString(i, String_Number);
		StringCopy("_test_", String_File_Name);
		StringConcatenate(String_File_Name, String_Number);
		
		// Open the file in write mode so the file should not exist yet
		Return_Value = FileOpen(String_File_Name, 'w', &File_IDs[i]);
		if (Return_Value != ERROR_CODE_NO_ERROR)
		{
			DisplayMessageErrorAndCode("while opening the files allowed count", Return_Value);
			return 1;
		}
	}
	
	// Opening one more file
	ScreenWriteString("Opening one more file...\n");
	Return_Value = FileOpen("_test_!!!", 'w', &File_IDs[FILE_MAXIMUM_OPENED_COUNT]);
	if (Return_Value != ERROR_CODE_CANT_OPEN_MORE_FILES)
	{
		DisplayMessageErrorAndCode("while opening too much files in the same time", Return_Value);
		return 1;
	}
	
	// Close and delete all files
	for (i = 0; i < FILE_MAXIMUM_OPENED_COUNT; i++)
	{
		FileClose(File_IDs[i]);
		
		// Create the file name
		StringConvertUnsignedIntegerToString(i, String_Number);
		StringCopy("_test_", String_File_Name);
		StringConcatenate(String_File_Name, String_Number);
		
		FileDelete(String_File_Name);
	}
	
	return 0;
}

/** A previously opened file can't be reopened without being closed.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
static int TestsFileReopenSameFile(void)
{
	unsigned int File_ID, File_ID_2;
	int Return_Value, Function_Result = 0;
	char *String_File_Content = "This is an empty test file.";
	
	// Create a file
	ScreenWriteString("Creating a file...\n");
	Return_Value = FileOpen("_test_", 'w', &File_ID);
	if (Return_Value != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while opening the file in write mode", Return_Value);
		Function_Result = 1;
		goto Exit;
	}
	// Write some data
	Return_Value = FileWrite(File_ID, String_File_Content, sizeof(String_File_Content) - 1);
	if (Return_Value != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while writing data to the file", Return_Value);
		Function_Result = 1;
		goto Exit;
	}
	// Flush file content
	FileClose(File_ID);
	
	// Try to open the same file 2 times
	// The first time must succeed
	ScreenWriteString("Opening the file for the first time...\n");
	Return_Value = FileOpen("_test_", 'r', &File_ID);
	if (Return_Value != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while opening the file in read mode", Return_Value);
		Function_Result = 1;
		goto Exit;
	}
	// The second time must fail
	ScreenWriteString("Opening the file for the second time...\n");
	Return_Value = FileOpen("_test_", 'r', &File_ID_2);
	if (Return_Value != ERROR_CODE_FILE_OPENED_YET)
	{
		DisplayMessageErrorAndCode("while reopening a previously opened file", Return_Value);
		Function_Result = 1;
	}
	
Exit:
	ScreenWriteString("Removing the file...\n");
	FileClose(File_ID);
	FileDelete("_test_"); // This function can be called even if the file is not existing
	return Function_Result;
}

/** Fill all available Files List entries.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
static int TestsFileFillFilesList(void)
{
	unsigned int i, Free_Files_Count, File_ID;
	int Result, Return_Value = 1;
	char String_File_Name[FILE_NAME_LENGTH + 1], String_Number[11];
	
	// Retrieve the amount of free Files List entries
	SystemGetFileSystemFreeSize(&i, &Free_Files_Count); // The first parameter is not relevant here
	
	ScreenWriteString("There are ");
	ScreenWriteUnsignedInteger(Free_Files_Count);
	ScreenWriteString(" free Files List entries.\n");
	
	ScreenWriteString("Filling all entries...\n");
	
	// Create one more files than the free Files List entries amount
	for (i = 0; i <= Free_Files_Count; i++)
	{
		// Create the file name
		StringConvertUnsignedIntegerToString(i, String_Number);
		StringCopy("_test_", String_File_Name);
		StringConcatenate(String_File_Name, String_Number);
		
		// Create the file
		Result = FileOpen(String_File_Name, 'w', &File_ID);
		if (i == Free_Files_Count) // The one more file
		{
			if (Result == ERROR_CODE_FILES_LIST_FULL) Return_Value = 0;
			else DisplayMessageErrorAndCode("while opening the offending file", Result);
			break;
		}
		else if (Result != ERROR_CODE_NO_ERROR) // Regular files
		{
			DisplayMessageErrorAndCode("while opening a file that could be opened", Result);
			break;
		}
		
		FileClose(File_ID);
	}
	
	ScreenWriteString("Deleting all test generated files...\n");
	
	// Delete all files
	for (i = 0; i < Free_Files_Count; i++)
	{
		// Create the file name
		StringConvertUnsignedIntegerToString(i, String_Number);
		StringCopy("_test_", String_File_Name);
		StringConcatenate(String_File_Name, String_Number);
		
		FileDelete(String_File_Name);
	}
	
	return Return_Value;
}

/** Fill all available Blocks List entries.
 * @return 0 if test was successful,
 * @return 1 if the test failed.
 */
static int TestsFileFillBlocksList(void)
{
	unsigned int Free_Blocks_Count, i, File_ID, Block_Size_Bytes;
	int Result, Return_Value = 1;
	
	// Retrieve the amount of free Blocks List entries
	SystemGetFileSystemFreeSize(&Free_Blocks_Count, &i);
	// Retrieve a block size in bytes
	SystemGetFileSystemTotalSize(&Block_Size_Bytes, &i, &i);
	
	ScreenWriteString("There are ");
	ScreenWriteUnsignedInteger(Free_Blocks_Count);
	ScreenWriteString(" free Blocks List entries.\n");
	
	ScreenWriteString("Creating the test file...\n");
	
	// Create a file
	Result = FileOpen("_test_", 'w', &File_ID);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while creating the file", Result);
		return 1;
	}
	
	// Fill the buffer with crap data
	for (i = 0; i < Block_Size_Bytes; i++) Buffer[i] = (unsigned char) RandomGenerateNumber();
	
	ScreenWriteString("Filling the file...\n");
	
	// Fill the Blocks List with one block more than the available space
	for (i = 0; i <= Free_Blocks_Count; i++)
	{
		Result = FileWrite(File_ID, Buffer, Block_Size_Bytes);
		
		if (i == Free_Blocks_Count) // The offending block is reached
		{
			if (Result != ERROR_CODE_BLOCKS_LIST_FULL)
			{
				DisplayMessageErrorAndCode(": the 'Blocks List full' error did not triggered successfully", Result);
				goto Exit;
			}
			Return_Value = 0;
			break;
		}
		else if (Result != ERROR_CODE_NO_ERROR) // Regular blocks
		{
			DisplayMessageErrorAndCode(": could not write a block whereas there is available space", Result);
			goto Exit;
		}
	}
	
Exit:
	ScreenWriteString("Deleting the test file...\n");
	FileClose(File_ID);
	FileDelete("_test_");
	return Return_Value;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int TestsFile(void)
{
	DisplayMessageTestStarting("Libraries File functions input parameters");
	if (TestsFileFunctionsInputParameters()) return 1;
	DisplayMessageTestSuccessful();
	
	DisplayMessageTestStarting("Libraries File system calls");
	if (TestsFileSystemCalls()) return 1;
	DisplayMessageTestSuccessful();
	
	DisplayMessageTestStarting("System maximum opened files limit");
	if (TestsFileMaximumOpenedFiles()) return 1;
	DisplayMessageTestSuccessful();
	
	DisplayMessageTestStarting("Same file reopening");
	if (TestsFileReopenSameFile()) return 1;
	DisplayMessageTestSuccessful();
	
	DisplayMessageTestStarting("Files List complete fill");
	if (TestsFileFillFilesList()) return 1;
	DisplayMessageTestSuccessful();
	
	DisplayMessageTestStarting("Blocks List complete fill");
	if (TestsFileFillBlocksList()) return 1;
	DisplayMessageTestSuccessful();
	
	return 0;
}