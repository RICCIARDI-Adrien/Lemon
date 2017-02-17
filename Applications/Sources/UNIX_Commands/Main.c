/** @file Main.c
 * This program gathers some useful standard UNIX/Linux commands.
 * @author Adrien RICCIARDI
 * @version 1.0 : 18/03/2015
 */
#include <System.h>
#include "Commands.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** Internal representation of a command. */
typedef struct
{
	char *String_Command_Name; //!< The command name, like "ls" or "df".
	int (*CommandMain)(int argc, char *argv[]); //!< The main() function for each command.
} TCommand;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All the available commands in alphabetical order. */
static TCommand Commands[] =
{
	{
		"df",
		CommandMainDf
	},
	{
		"ls",
		CommandMainLs
	},
	{
		"more",
		CommandMainMore
	},
	{
		"tftp",
		CommandMainTFTP
	},
	{
		"uptime",
		CommandMainUptime
	},
	// The NULL command tells that the list is terminated
	{
		NULL,
		NULL
	}
};

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Display a list of all the available commands.
 * @param String_Program_Name This program name on the user file system.
 */
static void DisplayUsage(char *String_Program_Name)
{
	int i;
	
	ScreenWriteString(STRING_USAGE_1);
	ScreenWriteString(String_Program_Name);
	ScreenWriteString(STRING_USAGE_2);
	ScreenWriteString(String_Program_Name);
	ScreenWriteString(STRING_USAGE_3);
	
	ScreenWriteString(STRING_AVAILABLE_COMMANDS);
	
	ScreenSetFontColor(SCREEN_COLOR_LIGHT_BLUE);
	for (i = 0; Commands[i].String_Command_Name != NULL; i++)
	{
		ScreenWriteString(Commands[i].String_Command_Name);
		ScreenWriteCharacter(' ');
	}
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
	ScreenWriteCharacter('\n');
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int i;
	char *String_Requested_Command;
	
	// Display the program help if no parameter is provided
	if (argc == 1)
	{
		DisplayUsage(argv[0]);
		return -1;
	}
	String_Requested_Command = argv[1];
	
	// Search for the requested command
	for (i = 0; Commands[i].String_Command_Name != NULL; i++)
	{
		if (StringCompare(Commands[i].String_Command_Name, String_Requested_Command))
		{
			// Remove the UNIX program name from the parameters when calling the command
			Commands[i].CommandMain(argc - 1, &argv[1]);
			return 0;
		}
	}
	
	// The command was not found
	ScreenSetFontColor(SCREEN_COLOR_RED);
	ScreenWriteString(STRING_UNKNOWN_COMMAND);
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
	
	return 0;
}