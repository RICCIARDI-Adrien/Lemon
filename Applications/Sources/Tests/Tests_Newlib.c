/** @file Tests_Newlib.c
 * Several basic tests for the newlib C library to make sure newlib system calls are working.
 * @author Adrien RICCIARDI
 */
#include <errno.h>
#include <stdio.h>
#include <System.h>
#include <unistd.h>
#include "Display_Message.h"
#include "Tests.h"

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
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