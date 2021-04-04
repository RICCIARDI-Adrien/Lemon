/** @file Help.c
 * System commands help.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
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
		"reboot",
		STRING_COMMAND_SHORT_DESCRIPTION_REBOOT,
		STRING_COMMAND_SHORT_DESCRIPTION_REBOOT
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
		"System",
		STRING_COMMAND_SHORT_DESCRIPTION_SYSTEM,
		STRING_COMMAND_FULL_DESCRIPTION_SYSTEM
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
	
	LibrariesScreenWriteString(STRING_AVAILABLE_COMMANDS);
	
	for (i = 0; Commands[i].String_Name != NULL; i++)
	{
		// Display the command
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_LIGHT_BLUE);
		LibrariesScreenWriteString(Commands[i].String_Name);
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
		
		// Display as many spaces as needed to align the command description on the requested column
		Spaces_Count = HELP_SHORT_DESCRIPTION_STARTING_COLUMN - LibrariesStringGetSize(Commands[i].String_Name);
		while (Spaces_Count > 0)
		{
			LibrariesScreenWriteCharacter(' ');
			Spaces_Count--;
		}
		
		// Display the command description
		LibrariesScreenWriteString(Commands[i].String_Short_Description);
		LibrariesScreenWriteCharacter('\n');
	}
}

/** Display a colored and centered title.
 * @param String_Title The title to display.
 */
static void DisplayTitle(char *String_Title)
{
	char String_Embellished_Title[128];
	
	// Embellish the title
	LibrariesStringCopy("--- ", String_Embellished_Title);
	LibrariesStringConcatenate(String_Embellished_Title, String_Title);
	LibrariesStringConcatenate(String_Embellished_Title, " ---\n\n");
	
	// Display it
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
	LibrariesScreenWriteCenteredString(String_Embellished_Title);
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
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
		LibrariesScreenWriteString(STRING_USAGE_1);
		LibrariesScreenWriteString(argv[0]);
		LibrariesScreenWriteString(STRING_USAGE_2);
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_LIGHT_BLUE);
		LibrariesScreenWriteString(STRING_USAGE_3);
		LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
		LibrariesScreenWriteString(STRING_USAGE_4);
		
		DisplayCommandsList();
		return -1;
	}
	String_Requested_Command = argv[1];
	
	// Search for a complete command description
	for (i = 0; Commands[i].String_Name != NULL; i++)
	{
		// Display the command help if the command exists
		if (LibrariesStringCompare(String_Requested_Command, Commands[i].String_Name))
		{
			DisplayTitle(String_Requested_Command);
			LibrariesScreenWriteString(Commands[i].String_Full_Description);
			LibrariesScreenWriteCharacter('\n');
			return 0;
		}
	}
	
	// Unknown command
	LibrariesScreenWriteString(STRING_UNKNOWN_COMMAND_1);
	LibrariesScreenWriteString(String_Requested_Command);
	LibrariesScreenWriteString(STRING_UNKNOWN_COMMAND_2);
	return 0;
}
