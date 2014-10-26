/** @file Help.c
 * @see Help.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Help.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** The starting column where to display the commands short description. */
#define HELP_SHORT_DESCRIPTION_STARTING_COLUMN 11

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All the available commands. */
static char *String_Commands[] =
{
	"clear",
	"copy",
	"delete",
	"download",
	"help",
	"list",
	"rename",
	"size",
	"text",
	"version",
	NULL
};

/** Commands short description. */
static char *String_Command_Short_Descriptions[] =
{
	STRING_COMMAND_SHORT_DESCRIPTION_CLEAR,
	STRING_COMMAND_SHORT_DESCRIPTION_COPY,
	STRING_COMMAND_SHORT_DESCRIPTION_DELETE,
	STRING_COMMAND_SHORT_DESCRIPTION_DOWNLOAD,
	STRING_COMMAND_SHORT_DESCRIPTION_HELP,
	STRING_COMMAND_SHORT_DESCRIPTION_LIST,
	STRING_COMMAND_SHORT_DESCRIPTION_RENAME,
	STRING_COMMAND_SHORT_DESCRIPTION_SIZE,
	STRING_COMMAND_SHORT_DESCRIPTION_TEXT,
	STRING_COMMAND_SHORT_DESCRIPTION_VERSION
};

/** Commands full description. */
static char *String_Command_Full_Descriptions[] =
{
	STRING_COMMAND_SHORT_DESCRIPTION_CLEAR,
	STRING_COMMAND_SHORT_DESCRIPTION_COPY,
	STRING_COMMAND_SHORT_DESCRIPTION_DELETE,
	STRING_COMMAND_SHORT_DESCRIPTION_DOWNLOAD,
	STRING_COMMAND_FULL_DESCRIPTION_HELP,
	STRING_COMMAND_SHORT_DESCRIPTION_LIST,
	STRING_COMMAND_SHORT_DESCRIPTION_RENAME,
	STRING_COMMAND_SHORT_DESCRIPTION_SIZE,
	STRING_COMMAND_FULL_DESCRIPTION_TEXT,
	STRING_COMMAND_SHORT_DESCRIPTION_VERSION
};
	
//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Display a list of all available commands. */
static void DisplayCommandsList(void)
{
	int i, Spaces_Count;
	
	ScreenWriteString(STRING_AVAILABLE_COMMANDS);
	
	for (i = 0; String_Commands[i] != NULL; i++)
	{
		// Display the command
		ScreenSetFontColor(SCREEN_COLOR_LIGHT_BLUE);
		ScreenWriteString(String_Commands[i]);
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		
		// Display as many spaces as needed to align the command description on the requested column
		Spaces_Count = HELP_SHORT_DESCRIPTION_STARTING_COLUMN - StringSize(String_Commands[i]);
		while (Spaces_Count > 0)
		{
			ScreenWriteCharacter(' ');
			Spaces_Count--;
		}
		
		// Display the command description
		ScreenWriteString(String_Command_Short_Descriptions[i]);
		ScreenWriteCharacter('\n');
	}
}

/** Display a colored title.
 * @param Title The title to display.
 */
static void DisplayTitle(char *String_Title)
{
	ScreenSetFontColor(SCREEN_COLOR_LIGHT_BLUE);
	ScreenWriteString("--- ");
	ScreenWriteString(String_Title);
	ScreenWriteString(" ---\n\n");
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int i;
	char *String_Requested_Command;
	
	// Display the generic help if no argument is provided
	if (argc < 2)
	{
		ScreenWriteString(STRING_USAGE_1);
		ScreenWriteString(argv[0]);
		ScreenWriteString(STRING_USAGE_2);
		
		DisplayCommandsList();
		return -1;
	}
	String_Requested_Command = argv[1];
	
	// Search for a complete command description
	for (i = 0; String_Commands[i] != NULL; i++)
	{
		// Display the command help if the command exists
		if (StringCompare(String_Requested_Command, String_Commands[i]))
		{
			DisplayTitle(String_Requested_Command);
			ScreenWriteString(String_Command_Full_Descriptions[i]);
			ScreenWriteCharacter('\n');
			return 0;
		}
	}
	
	// Unknown command
	ScreenWriteString(STRING_UNKNOWN_COMMAND_1);
	ScreenWriteString(String_Requested_Command);
	ScreenWriteString(STRING_UNKNOWN_COMMAND_2);
	return 0;
}