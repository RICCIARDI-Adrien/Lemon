/** @file Shell_Command_Help.c
 * Show a list of available shell commands.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/12/2012
 * @version 1.1 : 29/12/2012, added to kernel code.
 */
#include <Drivers/Driver_Screen.h>
#include <Kernel.h>
#include <Shell/Shell_Commands.h>
#include <Strings.h>

//-------------------------------------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------------------------------------
/** Available commands. */
static char *String_Commands[] =
{
	STRING_SHELL_HELP_COMMAND_CLEAR,
	STRING_SHELL_HELP_COMMAND_COPY,
	STRING_SHELL_HELP_COMMAND_DELETE,
	STRING_SHELL_HELP_COMMAND_DOWNLOAD,
	STRING_SHELL_HELP_COMMAND_HELP,
	STRING_SHELL_HELP_COMMAND_LIST,
	STRING_SHELL_HELP_COMMAND_RENAME,
	STRING_SHELL_HELP_COMMAND_SIZE,
	STRING_SHELL_HELP_COMMAND_TEXT,
	NULL
};

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
void ShellCommandHelp(void)
{
	int i = 0;
	
	// Show title
	ScreenSetColor(SCREEN_COLOR_GREEN);
	ScreenWriteString(STRING_SHELL_HELP_TITLE);
	ScreenSetColor(SCREEN_COLOR_BLUE);
	
	while (String_Commands[i] != NULL)
	{
		ScreenWriteString(String_Commands[i]);
		ScreenWriteChar('\n');
		i++;
	}
	
	// Show system version
	ScreenWriteString(STRING_SHELL_HELP_SYSTEM_VERSION);
}