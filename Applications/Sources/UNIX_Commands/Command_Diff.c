/** @file Command_Diff.c
 * Compare two files and tell whether they are different or not.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Commands.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Command entry point
//-------------------------------------------------------------------------------------------------
int CommandMainDiff(int argc, char *argv[])
{
	unsigned int File_Descriptor_1 = 0xFFFFFFFF, File_Descriptor_2 = 0xFFFFFFFF, Read_Bytes_Count;
	int Return_Value = -1, Is_End_Of_First_File_Reached = 0, Is_End_Of_Second_File_Reached = 0;
	char *String_File_1_Name, *String_File_2_Name;
	unsigned char Byte_1, Byte_2;
	
	// Check parameters
	if (argc != 3)
	{
		SystemScreenWriteString(STRING_COMMAND_DIFF_USAGE);
		goto Exit;
	}
	String_File_1_Name = argv[1];
	String_File_2_Name = argv[2];
	
	// Try to open the files
	if (SystemFileOpen(String_File_1_Name, LIBRARIES_FILE_OPENING_MODE_READ, &File_Descriptor_1) != ERROR_CODE_NO_ERROR)
	{
		SystemScreenWriteString(STRING_COMMAND_DIFF_FILE_CANT_BE_OPENED_1);
		SystemScreenWriteString(String_File_1_Name);
		SystemScreenWriteString(STRING_COMMAND_DIFF_FILE_CANT_BE_OPENED_2);
		goto Exit;
	}
	if (SystemFileOpen(String_File_2_Name, LIBRARIES_FILE_OPENING_MODE_READ, &File_Descriptor_2) != ERROR_CODE_NO_ERROR)
	{
		SystemScreenWriteString(STRING_COMMAND_DIFF_FILE_CANT_BE_OPENED_1);
		SystemScreenWriteString(String_File_2_Name);
		SystemScreenWriteString(STRING_COMMAND_DIFF_FILE_CANT_BE_OPENED_2);
		SystemScreenWriteString(STRING_COMMAND_DIFF_SAME_FILE_CANT_BE_REOPENED);
		goto Exit;
	}
	
	// Check whether files are different
	while (1)
	{
		// Read a byte from the first file
		if (SystemFileRead(File_Descriptor_1, &Byte_1, 1, &Read_Bytes_Count) != ERROR_CODE_NO_ERROR)
		{
			SystemScreenWriteString(STRING_COMMAND_DIFF_FILE_READ_ERROR_1);
			SystemScreenWriteString(String_File_1_Name);
			SystemScreenWriteString(STRING_COMMAND_DIFF_FILE_READ_ERROR_2);
			goto Exit;
		}
		if (Read_Bytes_Count == 0) Is_End_Of_First_File_Reached = 1;
		
		// Read a byte from the second file
		if (SystemFileRead(File_Descriptor_2, &Byte_2, 1, &Read_Bytes_Count) != ERROR_CODE_NO_ERROR)
		{
			SystemScreenWriteString(STRING_COMMAND_DIFF_FILE_READ_ERROR_1);
			SystemScreenWriteString(String_File_2_Name);
			SystemScreenWriteString(STRING_COMMAND_DIFF_FILE_READ_ERROR_2);
			goto Exit;
		}
		if (Read_Bytes_Count == 0) Is_End_Of_Second_File_Reached = 1;
		
		// Are the file of different size, or are the current read bytes different ?
		if ((Is_End_Of_First_File_Reached && !Is_End_Of_Second_File_Reached) || (!Is_End_Of_First_File_Reached && Is_End_Of_Second_File_Reached) || (Byte_1 != Byte_2))
		{
			SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_RED);
			SystemScreenWriteString(STRING_COMMAND_DIFF_FILES_ARE_DIFFERENT);
			break;
		}
		
		// Coming here when a file end has been reached is possible only if both file are of same length and have same content
		if (Is_End_Of_First_File_Reached) // No need to check for other file end because they are of same length
		{
			SystemScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
			SystemScreenWriteString(STRING_COMMAND_DIFF_FILES_ARE_EQUAL);
			break;
		}
	}
	
	Return_Value = 0;
	
Exit:
	if (File_Descriptor_1 != 0xFFFFFFFF) SystemFileClose(File_Descriptor_1);
	if (File_Descriptor_2 != 0xFFFFFFFF) SystemFileClose(File_Descriptor_2);
	return Return_Value;
}