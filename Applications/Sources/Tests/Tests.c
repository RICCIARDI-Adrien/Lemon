/** @file Tests.c
 * Contain all automated tests that can be done on the system.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
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
		"Automatic closing of a deleted opened file",
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
		"MemoryCopyArea() with a small area size",
		TestsMemoryCopySmallArea
	},
	{
		"MemoryCopyArea() with a big area size",
		TestsMemoryCopyBigArea
	},
	{
		"MemorySetAreaValue() with a small area size",
		TestsMemorySetSmallAreaValue
	},
	{
		"MemorySetAreaValue() with a big area size",
		TestsMemorySetBigAreaValue
	},
	// String API tests
	{
		"StringCompare()",
		TestsStringCompare
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
		"StringCopy()",
		TestsStringCopy
	},
	{
		"StringGetSize()",
		TestsStringGetSize
	}
};

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	unsigned int i;
	int Is_Pause_Requested = 0;
	
	// Check parameters
	if (argc > 1)
	{
		if (LibrariesStringCompare(argv[1], "-pause") == 1) Is_Pause_Requested = 1;
		else
		{
			LibrariesScreenWriteString("Usage : ");
			LibrariesScreenWriteString(argv[0]);
			LibrariesScreenWriteString(" [-pause]\n");
			LibrariesScreenWriteString("Specify -pause to wait for a key press after each successful test.\n");
			return 0;
		}
	}
	
	LibrariesRandomInitialize();
	
	LibrariesScreenWriteString("### Automated tests ###\n\n");
	
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
		
		// Wait a key to be pressed if such option is enabled
		if (Is_Pause_Requested)
		{
			LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_LIGHT_BLUE);
			LibrariesScreenWriteString("Press any key to continue...\n\n");
			LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
			LibrariesKeyboardReadCharacter();
		}
		else LibrariesScreenWriteCharacter('\n');
	}

	return 0;
}
