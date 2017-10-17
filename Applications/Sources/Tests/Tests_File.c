/** @file Tests_File.c
 * Test the Libraries File API.
 * @author Adrien RICCIARDI
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
// Public functions
//-------------------------------------------------------------------------------------------------
int TestsFileFunctionsInputParameters(void)
{
	unsigned int File_ID;
	int Result;
	
	// Testing SystemFileOpen()
	// Bad filename (empty string, checked by kernel)
	ScreenWriteString("Checking SystemFileOpen()...\n");
	Result = SystemFileOpen("\0", 'r', &File_ID);
	if (Result != ERROR_CODE_BAD_FILE_NAME)
	{
		DisplayMessageErrorAndCode("when given a bad file name", Result);
		return 1;
	}
	// Bad filename (NULL string, checked by Libraries)
	Result = SystemFileOpen(NULL, 'r', &File_ID);
	if (Result != ERROR_CODE_BAD_FILE_NAME)
	{
		DisplayMessageErrorAndCode("when given a NULL file name", Result);
		return 1;
	}
	// File not found
	Result = SystemFileOpen("_!azerty", 'r', &File_ID);
	if (Result != ERROR_CODE_FILE_NOT_FOUND)
	{
		DisplayMessageErrorAndCode("when given an unexisting file", Result);
		return 1;
	}
	// Unknown opening mode
	Result = SystemFileOpen("Ignored", 't', &File_ID);
	if (Result != ERROR_CODE_UNKNOWN_OPENING_MODE)
	{
		DisplayMessageErrorAndCode("when given a bad opening mode", Result);
		return 1;
	}

	// TODO
	// Testing SystemFileRead()
	// Open a file 
	// Bad file ID
	/*ScreenWriteString("Checking SystemFileRead()...\n");
	Result = SystemFileRead(123456, );
	if (Result != ERROR_CODE_FILE_NOT_FOUND)
	{
		DisplayMessageErrorAndCode("when given an unexisting file", Result);
		return 1;
	}*/
	// Bad opening mode
	
	return 0;
}

int TestsFileSystemCalls(void)
{
	unsigned int File_Size_Bytes, Written_Data_CRC, Read_Data_CRC, i, CRC_Seed, File_ID, Read_Bytes_Count;
	int Result;

	// Choose a random file size between 100 KB and 3 MB
	File_Size_Bytes = ((SystemRandomGenerateNumber() % 32) + 1) * 1024 * 100;
	ScreenWriteString("File size : ");
	ScreenWriteUnsignedInteger(File_Size_Bytes);
	ScreenWriteCharacter('\n');
 
	ScreenWriteString("Creating file content... ");
	for (i = 0; i < File_Size_Bytes; i++) Buffer[i] = (unsigned char) SystemRandomGenerateNumber();
	ScreenWriteString("done\n");

	ScreenWriteString("Computing CRC... ");
	CRC_Seed = SystemRandomGenerateNumber();
	Written_Data_CRC = crc32(CRC_Seed, Buffer, File_Size_Bytes);
	ScreenWriteString("done (CRC = ");
	ScreenWriteUnsignedInteger(Written_Data_CRC);
	ScreenWriteString(")\n");

	ScreenWriteString("Writing data to file... ");
	// Open the file
	Result = SystemCall(SYSTEM_CALL_FILE_OPEN, 'w', 0, "_test_", &File_ID);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("when opening the file in write mode", Result);
		return 1;
	}
	// Write data
	Result = SystemCall(SYSTEM_CALL_FILE_WRITE, File_ID, File_Size_Bytes, Buffer, NULL);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("when writing to the file", Result);
		return 1;
	}
	// Close the file
	SystemCall(SYSTEM_CALL_FILE_CLOSE, File_ID, 0, NULL, NULL);
	ScreenWriteString("done\n");

	// Flush read buffer
	SystemMemorySetAreaValue(Buffer, File_Size_Bytes, 0);

	ScreenWriteString("Reading data from file... ");
	// Open the file
	Result = SystemCall(SYSTEM_CALL_FILE_OPEN, 'r', 0, "_test_", &File_ID);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("when opening the file in read mode", Result);
		return 1;
	}
	// Read data
	Result = SystemCall(SYSTEM_CALL_FILE_READ, File_ID, File_Size_Bytes, Buffer, &Read_Bytes_Count);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("when reading from the file", Result);
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
	Result = SystemFileDelete("_test_");
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("when deleting the file", Result);
		return 1;
	}

	if (Read_Data_CRC == Written_Data_CRC) return 0;
	return 1;
}

int TestsFileMaximumOpenedFiles(void)
{
	unsigned int File_IDs[SYSTEM_FILE_MAXIMUM_OPENED_COUNT + 1];
	int i, Result;
	char String_File_Name[SYSTEM_FILE_NAME_LENGTH + 1], String_Number[11];
	
	// Open the maximum number of files
	ScreenWriteString("Opening as many files as possible...\n");
	for (i = 0; i < SYSTEM_FILE_MAXIMUM_OPENED_COUNT; i++)
	{
		// Create the file name
		SystemStringConvertUnsignedIntegerToString(i, String_Number);
		SystemStringCopy("_test_", String_File_Name);
		StringConcatenate(String_File_Name, String_Number);
		
		// Open the file in write mode so the file should not exist yet
		Result = SystemFileOpen(String_File_Name, 'w', &File_IDs[i]);
		if (Result != ERROR_CODE_NO_ERROR)
		{
			DisplayMessageErrorAndCode("while opening the files allowed count", Result);
			return 1;
		}
	}
	
	// Opening one more file
	ScreenWriteString("Opening one more file...\n");
	Result = SystemFileOpen("_test_!!!", 'w', &File_IDs[SYSTEM_FILE_MAXIMUM_OPENED_COUNT]);
	if (Result != ERROR_CODE_CANT_OPEN_MORE_FILES)
	{
		DisplayMessageErrorAndCode("while opening too much files in the same time", Result);
		return 1;
	}
	
	// Close and delete all files
	for (i = 0; i < SYSTEM_FILE_MAXIMUM_OPENED_COUNT; i++)
	{
		SystemFileClose(File_IDs[i]);
		
		// Create the file name
		SystemStringConvertUnsignedIntegerToString(i, String_Number);
		SystemStringCopy("_test_", String_File_Name);
		StringConcatenate(String_File_Name, String_Number);
		
		SystemFileDelete(String_File_Name);
	}
	
	return 0;
}

int TestsFileReopenSameFile(void)
{
	unsigned int File_ID, File_ID_2;
	int Result, Return_Value = 1;
	char *String_File_Content = "This is an empty test file.";
	
	// Create a file
	ScreenWriteString("Creating a file...\n");
	Result = SystemFileOpen("_test_", 'w', &File_ID);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while opening the file in write mode", Result);
		goto Exit;
	}
	// Write some data
	Result = SystemFileWrite(File_ID, String_File_Content, sizeof(String_File_Content) - 1);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while writing data to the file", Result);
		goto Exit;
	}
	// Flush file content
	SystemFileClose(File_ID);
	
	// Try to open the same file 2 times
	// The first time must succeed
	ScreenWriteString("Opening the file for the first time...\n");
	Result = SystemFileOpen("_test_", 'r', &File_ID);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while opening the file in read mode", Result);
		goto Exit;
	}
	// The second time must fail
	ScreenWriteString("Opening the file for the second time...\n");
	Result = SystemFileOpen("_test_", 'r', &File_ID_2);
	if (Result != ERROR_CODE_FILE_OPENED_YET)
	{
		DisplayMessageErrorAndCode("while reopening a previously opened file", Result);
		goto Exit;
	}
	
	Return_Value = 0;
	
Exit:
	ScreenWriteString("Removing the file...\n");
	SystemFileClose(File_ID);
	SystemFileDelete("_test_"); // This function can be called even if the file is not existing
	return Return_Value;
}

int TestsFileFillFilesList(void)
{
	unsigned int i, Free_Files_Count, File_ID;
	int Result, Return_Value = 1;
	char String_File_Name[SYSTEM_FILE_NAME_LENGTH + 1], String_Number[11];
	
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
		SystemStringConvertUnsignedIntegerToString(i, String_Number);
		SystemStringCopy("_test_", String_File_Name);
		StringConcatenate(String_File_Name, String_Number);
		
		// Create the file
		Result = SystemFileOpen(String_File_Name, 'w', &File_ID);
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
		
		SystemFileClose(File_ID);
	}
	
	ScreenWriteString("Deleting all test generated files...\n");
	
	// Delete all files
	for (i = 0; i < Free_Files_Count; i++)
	{
		// Create the file name
		SystemStringConvertUnsignedIntegerToString(i, String_Number);
		SystemStringCopy("_test_", String_File_Name);
		StringConcatenate(String_File_Name, String_Number);
		
		SystemFileDelete(String_File_Name);
	}
	
	return Return_Value;
}

int TestsFileFillBlocksList(void)
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
	Result = SystemFileOpen("_test_", 'w', &File_ID);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while creating the file", Result);
		return 1;
	}
	
	// Fill the buffer with crap data
	for (i = 0; i < Block_Size_Bytes; i++) Buffer[i] = (unsigned char) SystemRandomGenerateNumber();
	
	ScreenWriteString("Filling the file...\n");
	
	// Fill the Blocks List with one block more than the available space
	for (i = 0; i <= Free_Blocks_Count; i++)
	{
		Result = SystemFileWrite(File_ID, Buffer, Block_Size_Bytes);
		
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
	SystemFileClose(File_ID);
	SystemFileDelete("_test_");
	return Return_Value;
}

int TestsFileCloseDeletedOpenedFile(void)
{
	unsigned int File_ID;
	int Result, Return_Value = 1;
	char String_Data[] = "Test data";
	
	// Create a file
	Result = SystemFileOpen("_test_", 'w', &File_ID);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while creating the file to delete", Result);
		goto Exit;
	}
	// Write some data to it
	Result = SystemFileWrite(File_ID, String_Data, sizeof(String_Data));
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while writing data to the file", Result);
		goto Exit;
	}
	SystemFileClose(File_ID);
	
	// Open the file for reading
	Result = SystemFileOpen("_test_", 'r', &File_ID);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while opening the file to delete for reading", Result);
		goto Exit;
	}
	
	// Delete the file (the file must be automatically closed)
	SystemFileDelete("_test_");
	
	// Try to open the file another time (this must succeed because the previous file has been closed)
	Result = SystemFileOpen("_test_", 'w', &File_ID);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while opening the deleted file a second time", Result);
		goto Exit;
	}
	
	Return_Value = 0;
	
Exit:
	SystemFileClose(File_ID);
	SystemFileDelete("_test_");
	return Return_Value;
}

int TestsFileRename(void)
{
	int Result, Return_Value = 1;
	unsigned int File_ID;
	
	// Create two empty files
	// First file
	Result = SystemFileOpen("_test1_", 'w', &File_ID);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while creating the first file", Result);
		goto Exit;
	}
	SystemFileClose(File_ID);
	// Second file
	Result = SystemFileOpen("_test2_", 'w', &File_ID);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while creating the second file", Result);
		goto Exit;
	}
	SystemFileClose(File_ID);
	
	// Provide two bad file names
	Result = SystemFileRename("", "");
	if (Result != ERROR_CODE_BAD_FILE_NAME)
	{
		DisplayMessageErrorAndCode(": SystemFileRename(\"\", \"\") did not fail with the expected error", Result);
		goto Exit;
	}
	
	// Provide a bad file name as first argument
	Result = SystemFileRename("", "_test1_");
	if (Result != ERROR_CODE_BAD_FILE_NAME)
	{
		DisplayMessageErrorAndCode(": SystemFileRename(\"\", \"_test1_\") did not fail with the expected error", Result);
		goto Exit;
	}
	
	// Provide a bad file name as second argument
	Result = SystemFileRename("_test1_", "");
	if (Result != ERROR_CODE_BAD_FILE_NAME)
	{
		DisplayMessageErrorAndCode(": SystemFileRename(\"_test1_\", \"\") did not fail with the expected error", Result);
		goto Exit;
	}
	
	// Provide an existing file as the new file name
	Result = SystemFileRename("_test1_", "_test2_");
	if (Result != ERROR_CODE_FILE_ALREADY_EXISTS)
	{
		DisplayMessageErrorAndCode(": SystemFileRename(\"_test1_\", \"_test2_\") while _test2_ file exists did not fail with the expected error", Result);
		goto Exit;
	}
	
	// Provide a non-existing file as the file to rename
	Result = SystemFileRename("_test28_", "_test3_");
	if (Result != ERROR_CODE_FILE_NOT_FOUND)
	{
		DisplayMessageErrorAndCode(": SystemFileRename(\"_test28_\", \"_test2_\") while _test28_ file do not exists did not fail with the expected error", Result);
		goto Exit;
	}
	
	// Do a real rename
	Result = SystemFileRename("_test1_", "_test3_");
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while doing a normal rename operation", Result);
		goto Exit;
	}
	// Make sure the file was successfully renamed
	Result = SystemFileOpen("_test3_", 'r', &File_ID);
	if (Result != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageErrorAndCode("while trying to open the renamed file", Result);
		goto Exit;
	}
	SystemFileClose(File_ID);
	
	Return_Value = 0;
	
Exit:
	SystemFileDelete("_test1_");
	SystemFileDelete("_test2_");
	SystemFileDelete("_test3_");
	return Return_Value;
}