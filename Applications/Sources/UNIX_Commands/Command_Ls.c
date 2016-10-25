/** @file Command_Ls.c
 * A more complete "list" command, like "ls -lh | more".
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Commands.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** How many files can be stored at all. */
#define MAXIMUM_FILES_COUNT 4096

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** Hold file useful informations. */
typedef struct
{
	char String_File_Name[FILE_NAME_LENGTH];
	unsigned int Size_Bytes;
} TFileInformations;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All the found files. */
static TFileInformations Files_Informations[MAXIMUM_FILES_COUNT];
/** Use an array of pointer to fasten the file names sorting. */
static TFileInformations *Pointer_Sorted_Files_Informations[MAXIMUM_FILES_COUNT];

/** How many files really stay on the file system. */
static unsigned int Files_Count = 0;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Convert a lowercase character to uppercase.
 * @param Character The character to convert.
 * @return The converted character.
 */
static inline char ConvertCharacterToUppercase(char Character)
{
	if ((Character >= 'a') && (Character <= 'z')) Character -= 32;
	return Character;
}

/** Compare two strings according to the alphabetical order.
 * @param String_1 The first string to compare.
 * @param String_2 The second string to compare.
 * @return -1 if String_1 < String_2,
 * @return 0 if String_1 = String_2,
 * @return 1 if String_1 > String_2.
 */
static int StringAlphabeticalCompare(char *String_1, char *String_2)
{
	char Character_1, Character_2;
	
	while ((*String_1 != 0) && (*String_2 != 0))
	{
		// Convert the characters to uppercase
		Character_1 = ConvertCharacterToUppercase(*String_1);
		Character_2 = ConvertCharacterToUppercase(*String_2);
		
		if (Character_1 < Character_2) return -1;
		if (Character_1 > Character_2) return 1;
		String_1++;
		String_2++;
	}
	
	// Are the string the same length ?
	if ((*String_1 == 0) && (*String_2 != 0)) return -1;
	if ((*String_1 != 0) && (*String_2 == 0)) return 1;
	
	// The strings are equal
	return 0;
}

/** Use an inefficient but simple bubble sort to order the file names. */
static void SortFileNames(void)
{
	int Is_Modification_Done, Comparison_Result; // When a full pass has been done without modification, the sorting is finished
	unsigned int i;
	TFileInformations *Pointer_File_Informations;
	
	// Nothing to do if there is only 0 or 1 file
	if (Files_Count < 2) return;
	
	do
	{
		Is_Modification_Done = 0;
		
		// Go through all files
		for (i = 0; i < Files_Count - 1; i++)
		{
			Comparison_Result = StringAlphabeticalCompare(Pointer_Sorted_Files_Informations[i]->String_File_Name, Pointer_Sorted_Files_Informations[i + 1]->String_File_Name);
			
			// The upper string is misplaced, swap them
			if (Comparison_Result > 0)
			{
				Pointer_File_Informations = Pointer_Sorted_Files_Informations[i];
				Pointer_Sorted_Files_Informations[i] = Pointer_Sorted_Files_Informations[i + 1];
				Pointer_Sorted_Files_Informations[i + 1] = Pointer_File_Informations;
				Is_Modification_Done = 1;
			}
		}
	} while (Is_Modification_Done);
}

//-------------------------------------------------------------------------------------------------
// Command entry point
//-------------------------------------------------------------------------------------------------
int CommandMainLs(int argc, char __attribute__((unused)) *argv[])
{
	unsigned int Block_Size, Total_Blocks_Count, Total_Files_Count, i = 0, Remaining_Characters, Displayed_Files_Count = 0;
	
	// Check parameters
	if (argc != 1)
	{
		ScreenWriteString(STRING_COMMAND_LS_USAGE);
		return -1;
	}
	
	// Check if the allocated RAM is enough to store all the file system files
	SystemGetFileSystemTotalSize(&Block_Size, &Total_Blocks_Count, &Total_Files_Count);
	if (Total_Files_Count > MAXIMUM_FILES_COUNT)
	{
		ScreenSetFontColor(SCREEN_COLOR_RED);
		ScreenWriteString(STRING_COMMAND_LS_NOT_ENOUGH_SPACE_TO_STORE_FILES);
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		return -1;
	}
	
	// Get all hard disk files name and size
	FileListInitialize();
	while (1)
	{
		// Get the next file name
		FileListNext(Files_Informations[Files_Count].String_File_Name);
		if (Files_Informations[Files_Count].String_File_Name[0] == 0) break;
		
		// Get the file size
		Files_Informations[Files_Count].Size_Bytes = FileGetSize(Files_Informations[Files_Count].String_File_Name);
		
		// Add the file informations to the pointers array
		Pointer_Sorted_Files_Informations[Files_Count] = &(Files_Informations[Files_Count]);
		
		Files_Count++;
	}
	
	// Sort the file names in alphabetical order
	SortFileNames();
	
	// Display the files informations
	for (i = 0; i < Files_Count; i++)
	{
		// Display the file name
		ScreenWriteString(Pointer_Sorted_Files_Informations[i]->String_File_Name);
		
		// Fill the eventually remaining space up to the beginning of the "file size" column
		Remaining_Characters = (FILE_NAME_LENGTH + 4) - StringGetSize(Pointer_Sorted_Files_Informations[i]->String_File_Name);
		for ( ; Remaining_Characters > 0; Remaining_Characters--) ScreenWriteCharacter(' ');
		
		// Display the file size
		ScreenWriteUnsignedInteger(Pointer_Sorted_Files_Informations[i]->Size_Bytes);
		ScreenWriteString(STRING_COMMAND_LS_DISPLAYED_UNIT);
		
		// Wait for the user to press a key if the screen is full of displayed files
		Displayed_Files_Count++;
		if ((Displayed_Files_Count == SCREEN_ROWS_COUNT - 1) && (i < Files_Count - 1)) // Do not display the wait prompt if there is no more file to display
		{
			ScreenSetFontColor(SCREEN_COLOR_LIGHT_BLUE);
			ScreenWriteString(STRING_COMMAND_LS_WAIT_FOR_USER_INPUT);
			ScreenSetFontColor(SCREEN_COLOR_BLUE);
			
			KeyboardReadCharacter();
			ScreenWriteCharacter('\n');
			
			Displayed_Files_Count = 0;
		}
	}
	
	return 0;
}