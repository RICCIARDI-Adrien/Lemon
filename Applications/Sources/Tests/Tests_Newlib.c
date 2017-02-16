/** @file Tests_Newlib.c
 * Several basic tests for the newlib C library to make sure newlib system calls are working.
 * @author Adrien RICCIARDI
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <System.h>
#include <unistd.h>
#include "Display_Message.h"
#include "Tests.h"

// TODO check errno return codes

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
// No need to test complex file scenarios because they are already tested by Test_Files.c tests
int TestsNewlibSystemCallOpen(void)
{
	int Return_Value = 1, UNIX_File_Descriptor;
	unsigned int Lemon_File_Descriptor;
	char String_Test[] = "This is a test string.";
	
	// Create a temporary file
	if (FileOpen("_test_", FILE_OPENING_MODE_WRITE, &Lemon_File_Descriptor) != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageError("could not open temporary file.");
		return 1;
	}
	if (FileWrite(Lemon_File_Descriptor, String_Test, sizeof(String_Test)) != ERROR_CODE_NO_ERROR)
	{
		DisplayMessageError("failed to write to the temporary file.");
		FileClose(Lemon_File_Descriptor);
		return 1;
	}
	FileClose(Lemon_File_Descriptor);
	
	// Can't open a file in both read and write modes
	if (open("_test_", O_RDWR) != -1)
	{
		DisplayMessageError("opening a file in both read an write modes did not fail.");
		goto Exit;
	}
	
	// Can't open a non-existing file in read mode
	if (open("_notexisting", O_RDONLY) != -1)
	{
		DisplayMessageError("opening a non-existing file in read mode did not fail.");
		goto Exit;
	}
	
	// Open an existing file in read mode
	UNIX_File_Descriptor = open("_test_", O_RDONLY);
	if (UNIX_File_Descriptor != 3)
	{
		DisplayMessageError("failed to open an existing file in read mode.");
		goto Exit;
	}
	FileClose(UNIX_File_Descriptor - 3); // Bypass stdin, stdout and stderr (see newlib Lemon open system call implementation for description)
	
	// Open an existing file in write mode
	UNIX_File_Descriptor = open("_test_", O_WRONLY);
	if (UNIX_File_Descriptor != 3)
	{
		DisplayMessageError("opening an existing file in write mode failed.");
		goto Exit;
	}
	FileClose(UNIX_File_Descriptor - 3); // Bypass stdin, stdout and stderr (see newlib Lemon open system call implementation for description)
	
	// Open a non-existing file in write mode
	UNIX_File_Descriptor = open("_notexisting", O_WRONLY);
	if (UNIX_File_Descriptor != 3)
	{
		DisplayMessageError("opening a non-existing file in write mode failed.");
		goto Exit;
	}
	FileClose(UNIX_File_Descriptor - 3); // Bypass stdin, stdout and stderr (see newlib Lemon open system call implementation for description)
	
	Return_Value = 0;
	
Exit:
	FileDelete("_test_");
	FileDelete("_notexisting"); // Closing this file while opened in write mode created it
	return Return_Value;
}

// Warning : 'open' syscall must have been tested prior to test write
int TestsNewlibSystemCallWrite(void)
{
	// Try to write to stdin
	if (write(0, "bla", 3) != -1)
	{
		DisplayMessageError("stdin write did not fail.");
		return 1;
	}
	
	// Try to write to stdout
	ScreenWriteString("Write the string \"test\\n\" to stdout : ");
	if (write(1, "test\n", 5) != 5)
	{
		DisplayMessageError("stdout write failed.");
		return 1;
	}
	
	// Try to write to stderr
	ScreenWriteString("Write the string \"test\\n\" to stderr : ");
	if (write(2, "test\n", 5) != 5)
	{
		DisplayMessageError("stderr write failed.");
		return 1;
	}
	
	return 0;
}