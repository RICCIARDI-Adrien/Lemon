/** @file Help.c
 * System commands help.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** The starting column where to display the commands short description. */
#define HELP_SHORT_DESCRIPTION_STARTING_COLUMN 11

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A command description. */
typedef struct
{
	char *String_Name; //! The command name.
	char *String_Short_Description; //! A short description that can fit on one line.
	char *String_Full_Description; //! A more complete description.
} THelpCommand;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All the available commands. */
static THelpCommand Commands[] =
{
	{
		"chat",
		STRING_COMMAND_SHORT_DESCRIPTION_CHAT,
		STRING_COMMAND_FULL_DESCRIPTION_CHAT
	},
	{
		"clear",
		STRING_COMMAND_SHORT_DESCRIPTION_CLEAR,
		STRING_COMMAND_SHORT_DESCRIPTION_CLEAR
	},
	{
		"copy",
		STRING_COMMAND_SHORT_DESCRIPTION_COPY,
		STRING_COMMAND_SHORT_DESCRIPTION_COPY
	},
	{
		"delete",
		STRING_COMMAND_SHORT_DESCRIPTION_DELETE,
		STRING_COMMAND_SHORT_DESCRIPTION_DELETE
	},
	{
		"download",
		STRING_COMMAND_SHORT_DESCRIPTION_DOWNLOAD,
		STRING_COMMAND_SHORT_DESCRIPTION_DOWNLOAD
	},
	{
		"edit",
		STRING_COMMAND_SHORT_DESCRIPTION_EDIT,
		STRING_COMMAND_FULL_DESCRIPTION_EDIT
	},
	{
		"games",
		STRING_COMMAND_SHORT_DESCRIPTION_GAMES,
		STRING_COMMAND_FULL_DESCRIPTION_GAMES
	},
	{
		"help",
		STRING_COMMAND_SHORT_DESCRIPTION_HELP,
		STRING_COMMAND_FULL_DESCRIPTION_HELP
	},
	{
		"list",
		STRING_COMMAND_SHORT_DESCRIPTION_LIST,
		STRING_COMMAND_SHORT_DESCRIPTION_LIST
	},
	{
		"rename",
		STRING_COMMAND_SHORT_DESCRIPTION_RENAME,
		STRING_COMMAND_SHORT_DESCRIPTION_RENAME
	},
	{
		"size",
		STRING_COMMAND_SHORT_DESCRIPTION_SIZE,
		STRING_COMMAND_SHORT_DESCRIPTION_SIZE
	},
	{
		"u",
		STRING_COMMAND_SHORT_DESCRIPTION_U,
		STRING_COMMAND_FULL_DESCRIPTION_U
	},
	{
		"version",
		STRING_COMMAND_SHORT_DESCRIPTION_VERSION,
		STRING_COMMAND_SHORT_DESCRIPTION_VERSION
	},
	{
		NULL,
		NULL,
		NULL
	}
};

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Display a list of all available commands. */
static void DisplayCommandsList(void)
{
	int i, Spaces_Count;
	
	SystemScreenWriteString(STRING_AVAILABLE_COMMANDS);
	
	for (i = 0; Commands[i].String_Name != NULL; i++)
	{
		// Display the command
		SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_LIGHT_BLUE);
		SystemScreenWriteString(Commands[i].String_Name);
		SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_BLUE);
		
		// Display as many spaces as needed to align the command description on the requested column
		Spaces_Count = HELP_SHORT_DESCRIPTION_STARTING_COLUMN - SystemStringGetSize(Commands[i].String_Name);
		while (Spaces_Count > 0)
		{
			SystemScreenWriteCharacter(' ');
			Spaces_Count--;
		}
		
		// Display the command description
		SystemScreenWriteString(Commands[i].String_Short_Description);
		SystemScreenWriteCharacter('\n');
	}
}

/** Display a colored and centered title.
 * @param String_Title The title to display.
 */
static void DisplayTitle(char *String_Title)
{
	char String_Embellished_Title[128];
	
	// Embellish the title
	SystemStringCopy("--- ", String_Embellished_Title);
	SystemStringConcatenate(String_Embellished_Title, String_Title);
	SystemStringConcatenate(String_Embellished_Title, " ---\n\n");
	
	// Display it
	SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_GREEN);
	SystemScreenWriteCenteredString(String_Embellished_Title);
	SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_BLUE);
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
		SystemScreenWriteString(STRING_USAGE_1);
		SystemScreenWriteString(argv[0]);
		SystemScreenWriteString(STRING_USAGE_2);
		
		DisplayCommandsList();
		return -1;
	}
	String_Requested_Command = argv[1];
	
	// Search for a complete command description
	for (i = 0; Commands[i].String_Name != NULL; i++)
	{
		// Display the command help if the command exists
		if (SystemStringCompare(String_Requested_Command, Commands[i].String_Name))
		{
			DisplayTitle(String_Requested_Command);
			SystemScreenWriteString(Commands[i].String_Full_Description);
			SystemScreenWriteCharacter('\n');
			return 0;
		}
	}
	
	// Unknown command
	SystemScreenWriteString(STRING_UNKNOWN_COMMAND_1);
	SystemScreenWriteString(String_Requested_Command);
	SystemScreenWriteString(STRING_UNKNOWN_COMMAND_2);
	return 0;
}