/** @file Tests.c
 * Contain all automated tests that can be done on the system.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Display_Message.h"
#include "Tests.h"

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All the tests. */
static TTest Tests[] =
{
	// File API tests
	{
		"Libraries File functions input parameters",
		TestsFileFunctionsInputParameters
	},
	{
		"Libraries File system calls",
		TestsFileSystemCalls
	},
	{
		"System maximum opened files limit",
		TestsFileMaximumOpenedFiles
	},
	{
		"Same file reopening",
		TestsFileReopenSameFile
	},
	{
		"Automatic closing of an deleted opened file",
		TestsFileCloseDeletedOpenedFile
	},
	{
		"Files List complete fill",
		TestsFileFillFilesList
	},
	{
		"Blocks List complete fill",
		TestsFileFillBlocksList
	},
	{
		"File renaming",
		TestsFileRename
	},
	// Memory API tests
	{
		"SystemMemoryCopyArea() with a small area size",
		TestsMemoryCopySmallArea
	},
	{
		"SystemMemoryCopyArea() with a big area size",
		TestsMemoryCopyBigArea
	},
	{
		"SystemMemorySetAreaValue() with a small area size",
		TestsMemorySetSmallAreaValue
	},
	{
		"SystemMemorySetAreaValue() with a big area size",
		TestsMemorySetBigAreaValue
	},
	// String API tests
	{
		"SystemStringCompare()",
		TestsSystemStringCompare
	},
	{
		"StringConcatenate()",
		TestsStringConcatenate
	},
	{
		"StringConcatenateUpToNumber()",
		TestsStringConcatenateUpToNumber
	},
	{
		"StringConvertStringToUnsignedInteger()",
		TestsStringConvertStringToUnsignedInteger
	},
	{
		"StringConvertUnsignedIntegerToString()",
		TestsStringConvertUnsignedIntegerToString
	},
	{
		"StringConvertIntegerToHexadecimal()",
		TestsStringConvertIntegerToHexadecimal
	},
	{
		"SystemStringCopy()",
		TestsSystemStringCopy
	},
	{
		"StringGetSize()",
		TestsStringGetSize
	}
};

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	unsigned int i;
	
	SystemRandomInitialize();
	
	SystemScreenWriteString("### Automated tests ###\n\n");
	
	for (i = 0; i < sizeof(Tests) / sizeof(TTest); i++)
	{
		// Display title
		DisplayMessageTestStarting(Tests[i].String_Title);
		
		if (Tests[i].Function() != 0)
		{
			DisplayMessageTestFailed();
			break;
		}
		
		DisplayMessageTestSuccessful();
	}

	return 0;
}
