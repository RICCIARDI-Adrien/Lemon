/** @file Shell.c
 * @see Shell.h for description.
 * @author Adrien RICCIARDI
 */
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_Screen.h>
#include <Error_Codes.h>
#include <File_System/File.h>
#include <File_System/File_System.h>
#include <Kernel.h>
#include <Shell/Shell.h>
#include <Shell/Shell_Commands.h>
#include <Standard_Functions.h>
#include <Strings.h>

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Offset of the Arguments_Value field in the TCommandLineArguments structure. */
#define ARGUMENTS_VALUE_OFFSET (sizeof(int) + (sizeof(char *) * SHELL_MAXIMUM_ARGUMENTS_COUNT))

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** Describe the command line format given to the program. */
typedef struct __attribute__((packed))
{
	int Arguments_Count; //! Contain argc value.
	char *Pointer_Arguments[SHELL_MAXIMUM_ARGUMENTS_COUNT]; //! The *argv[] representation.
	char Arguments_Value[KERNEL_PROGRAM_ENTRY_POINT - ARGUMENTS_VALUE_OFFSET]; //! Hold arguments values. Fill the remaining bytes of the 256-byte area before program beginning.
} TCommandLineArguments;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** This boolean tells if the shell was previously launched or not. */
static char Is_First_Launch = 1;

/** The command line arguments in kernel memory. */
static TCommandLineArguments Command_Line_Arguments;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Read the command line and add support for some special keys.
 * @param Pointer_Buffer The buffer used to store the command line (must me large enough).
 * @note The function automatically stop getting input when it reaches the end of the line.
 * @note Recognized special keys :
 * @note Up arrow : show previous command.
 */
static inline void ShellReadCommandLine(char *Pointer_Buffer)
{
	static char String_Last_Command_Buffer[SHELL_MAXIMUM_LINE_LENGTH + 1] = {0}; // +1 for terminating zero
	unsigned char Key_Code;
	int Characters_Count = 0;
	
	while (1)
	{
		// Read key
		Key_Code = KeyboardReadChar();
		
		// Handle special key Up arrow (use last command)
		if (Key_Code == KEYBOARD_KEY_CODE_ARROW_UP)
		{
			// Do nothing if the last command is empty
			if (String_Last_Command_Buffer[0] == 0) continue;
			
			// Delete previous characters if some were inserted yet
			while (Characters_Count > 0)
			{
				ScreenWriteCharacter('\b');
				Characters_Count--;
			}

			// Show last command
			ScreenWriteString(String_Last_Command_Buffer);
			// Copy to buffer
			strcpy(Pointer_Buffer, String_Last_Command_Buffer);
			Characters_Count = strlen(String_Last_Command_Buffer);
			continue;
		}
		
		// Handle control keys
		switch (Key_Code)
		{
			// Backspace
			case '\b':
				// Delete last character if it exists
				if (Characters_Count > 0)
				{
					Characters_Count--;
					Pointer_Buffer[Characters_Count] = 0;
					ScreenWriteCharacter('\b');
				}
				break;
				
			// Enter
			case '\n':
				// Terminate string
				Pointer_Buffer[Characters_Count] = 0;
				// Go to next line
				ScreenWriteCharacter('\n');
				// Copy this line into the last command buffer
				strcpy(String_Last_Command_Buffer, Pointer_Buffer);
				return;
			
			// Normal keys
			default:
				// Accept only printable characters
				if ((Key_Code < 32) || ((Key_Code > KEYBOARD_KEY_CODE_ARROW_UP) && (Key_Code <= KEYBOARD_KEY_CODE_F11))) break;
				// Can't add more characters than line length
				if (Characters_Count >= SHELL_MAXIMUM_LINE_LENGTH) break;
				// Add character to  the string
				Pointer_Buffer[Characters_Count] = Key_Code;
				Characters_Count++;
				ScreenWriteCharacter(Key_Code);
				break;
		}
	}
}

/** Split command line into arguments (they are separated with spaces).
 * @param Pointer_Buffer The buffer containing the raw command line.
 */
static inline void ShellSplitCommandLine(char *Pointer_Buffer)
{
	int i = 0; // Index in the arguments values area
	
	Command_Line_Arguments.Arguments_Count = 0;
	
	do
	{
		// Remove beginning spaces
		while ((*Pointer_Buffer != 0) && (*Pointer_Buffer == ' ')) Pointer_Buffer++;
		if (*Pointer_Buffer == 0) return;
		
		// Set argv[] pointer
		Command_Line_Arguments.Pointer_Arguments[Command_Line_Arguments.Arguments_Count] = &Command_Line_Arguments.Arguments_Value[i];
		
		// Copy argument string until next space or command line end
		while ((*Pointer_Buffer != 0) && (*Pointer_Buffer != ' '))
		{
			Command_Line_Arguments.Arguments_Value[i] = *Pointer_Buffer;
			Pointer_Buffer++;
			i++;
		}
		// Terminate string
		Command_Line_Arguments.Arguments_Value[i] = 0;
		i++;
	
		Command_Line_Arguments.Arguments_Count++;
	} while ((*Pointer_Buffer != 0) && (Command_Line_Arguments.Arguments_Count < SHELL_MAXIMUM_ARGUMENTS_COUNT));
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void Shell(void)
{
	char String_Buffer[SHELL_MAXIMUM_LINE_LENGTH + 1]; // Length of a console line + 1
	int i, Offset;
	TCommandLineArguments *Pointer_Command_Line_Arguments = (TCommandLineArguments *) KERNEL_USER_SPACE_ADDRESS;
	
	// Show welcoming message
	if (Is_First_Launch)
	{
		ScreenSetColor(SCREEN_COLOR_LIGHT_BLUE);
		ScreenClear();
		ScreenWriteString(STRING_SHELL_WELCOME);
		Is_First_Launch = 0;
	}
	
	// Main loop
	while (1)
	{
		// Print prompt
		ScreenSetColor(SCREEN_COLOR_GREEN);
		ScreenWriteString("-> ");
		ScreenSetColor(SCREEN_COLOR_BLUE);
		
		// Read the command
		ShellReadCommandLine(String_Buffer);
		
		// Retrieve arguments
		ShellSplitCommandLine(String_Buffer);
		// Show another prompt if the user typed only Enter or spaces
		if (Command_Line_Arguments.Arguments_Count == 0) continue;
		
		// Comparison with system commands
		
		// Clear the screen
		//====================================================================================================================
		if (strcmp(Command_Line_Arguments.Pointer_Arguments[0], SHELL_COMMAND_CLEAR_SCREEN) == 0) ScreenClear();
		//====================================================================================================================
		// List existing files
		//====================================================================================================================
		else if (strcmp(Command_Line_Arguments.Pointer_Arguments[0], SHELL_COMMAND_LIST_FILES) == 0) ShellCommandList();
		//====================================================================================================================
		// Delete an existing file
		//====================================================================================================================
		else if (strcmp(Command_Line_Arguments.Pointer_Arguments[0], SHELL_COMMAND_DELETE_FILE) == 0)
		{
			if (Command_Line_Arguments.Arguments_Count != 2)
			{
				ScreenSetColor(SCREEN_COLOR_RED);
				ScreenWriteString(STRING_SHELL_DELETE_FILE_BAD_PARAMETERS_COUNT);
				ScreenSetColor(SCREEN_COLOR_BLUE);
			}
			else ShellCommandDeleteFile(Command_Line_Arguments.Pointer_Arguments[1]);
		}
		//====================================================================================================================
		// Copy an existing file
		//====================================================================================================================
		else if (strcmp(Command_Line_Arguments.Pointer_Arguments[0], SHELL_COMMAND_COPY_FILE) == 0)
		{
			if (Command_Line_Arguments.Arguments_Count != 3)
			{
				ScreenSetColor(SCREEN_COLOR_RED);
				ScreenWriteString(STRING_SHELL_COPY_FILE_BAD_PARAMETERS_COUNT);
				ScreenSetColor(SCREEN_COLOR_BLUE);
			}
			else ShellCommandCopyFile(Command_Line_Arguments.Pointer_Arguments[1], Command_Line_Arguments.Pointer_Arguments[2]);
		}
		//====================================================================================================================
		// Rename an existing file
		//====================================================================================================================
		else if (strcmp(Command_Line_Arguments.Pointer_Arguments[0], SHELL_COMMAND_RENAME_FILE) == 0)
		{
			if (Command_Line_Arguments.Arguments_Count != 3)
			{
				ScreenSetColor(SCREEN_COLOR_RED);
				ScreenWriteString(STRING_SHELL_RENAME_FILE_BAD_PARAMETERS_COUNT);
				ScreenSetColor(SCREEN_COLOR_BLUE);
			}
			else ShellCommandRenameFile(Command_Line_Arguments.Pointer_Arguments[1], Command_Line_Arguments.Pointer_Arguments[2]);
		}
		//====================================================================================================================
		// Get the size in bytes of an existing file
		//====================================================================================================================
		else if (strcmp(Command_Line_Arguments.Pointer_Arguments[0], SHELL_COMMAND_FILE_SIZE) == 0)
		{
			if (Command_Line_Arguments.Arguments_Count != 2)
			{
				ScreenSetColor(SCREEN_COLOR_RED);
				ScreenWriteString(STRING_SHELL_FILE_SIZE_BAD_PARAMETERS_COUNT);
				ScreenSetColor(SCREEN_COLOR_BLUE);
			}
			else ShellCommandFileSize(Command_Line_Arguments.Pointer_Arguments[1]);
		}
		//====================================================================================================================
		// Download a file from the serial port
		//====================================================================================================================
		else if (strcmp(Command_Line_Arguments.Pointer_Arguments[0], SHELL_COMMAND_DOWNLOAD_FILE) == 0) ShellCommandDownload();
		//====================================================================================================================
		// Show the system version
		//====================================================================================================================
		else if (strcmp(Command_Line_Arguments.Pointer_Arguments[0], SHELL_COMMAND_VERSION) == 0) ScreenWriteString(STRING_SHELL_VERSION);
		//====================================================================================================================
		// Execute a program or tell the user that the command is unknown
		//====================================================================================================================
		else 
		{
			// Clear program magic number area so newly loaded program must have a valid magic number
			memset((unsigned char *) KERNEL_PROGRAM_LOAD_ADDRESS, 0, sizeof(KERNEL_PROGRAM_MAGIC_NUMBER)); 
			
			// Set error color only one time
			ScreenSetColor(SCREEN_COLOR_RED);
			
			switch (FileLoad(Command_Line_Arguments.Pointer_Arguments[0], (unsigned char *) KERNEL_PROGRAM_LOAD_ADDRESS, 1))
			{
				case ERROR_CODE_FILE_NOT_FOUND:
					ScreenWriteString(STRING_SHELL_ERROR_UNKNOWN_COMMAND);
					break;
					
				case ERROR_CODE_FILE_LARGER_THAN_RAM:
					ScreenWriteString(STRING_SHELL_ERROR_FILE_TO_LOAD_LARGER_THAN_RAM);
					break;
					
				case ERROR_CODE_FILE_NOT_EXECUTABLE:
					ScreenWriteString(STRING_SHELL_ERROR_FILE_NOT_EXECUTABLE);
					break;
			
				// No error
				default:
					// Copy command line arguments to user space
					memcpy((unsigned char *) Pointer_Command_Line_Arguments, (unsigned char *) &Command_Line_Arguments, sizeof(Command_Line_Arguments));
					
					// Compute the offset between user space argv[] pointers and kernel space ones
					Offset = (unsigned int) Command_Line_Arguments.Arguments_Value - ARGUMENTS_VALUE_OFFSET;
					// Adjust argv[] pointers to fit into user space
					for (i = 0; i < Command_Line_Arguments.Arguments_Count; i++) Pointer_Command_Line_Arguments->Pointer_Arguments[i] -= Offset;
					
					// Reset normal console color
					ScreenSetColor(SCREEN_COLOR_BLUE);
					KernelStartProgram();
					break;
			}
		}
	}
}