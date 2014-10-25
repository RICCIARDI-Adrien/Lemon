/** @file Shell_Commands.h
 * Commands compiled into the shell.
 * @author Adrien RICCIARDI
 * @version 1.0 : 22/12/2012
 * @version 1.1 : 29/12/2012, added to kernel code.
 */
#ifndef H_SHELL_COMMANDS_H
#define H_SHELL_COMMANDS_H

/** Clear the screen. */
#define SHELL_COMMAND_CLEAR_SCREEN "clear"
/** List existing files. */
#define SHELL_COMMAND_LIST_FILES "list"
/** Download a file from the serial port. */
#define SHELL_COMMAND_DOWNLOAD_FILE "download"
/** Show a list of available shell commands. */
#define SHELL_COMMAND_HELP "help"
/** Delete an existing file. */
#define SHELL_COMMAND_DELETE_FILE "delete"
/** Rename an existing file. */
#define SHELL_COMMAND_RENAME_FILE "rename"
/** Get the size in bytes of an existing file. */
#define SHELL_COMMAND_FILE_SIZE "size"
/** Copy the content of an existing file into a new one. */
#define SHELL_COMMAND_COPY_FILE "copy"

/** List existing files. */
void ShellCommandList(void);

/** Download a file from the serial port and store it into the user space. */
void ShellCommandDownload(void);

/** Show a list of available shell commands. */
void ShellCommandHelp(void);

/** Delete an existing file.
 * @param String_File_Name The name of the file to delete.
 */
void ShellCommandDeleteFile(char *String_File_Name);

/** Rename an existing file by giving it a new unused name. 
 * @param String_Current_File_Name The current name of the file.
 * @param String_New_File_Name The new name to be given to the file (it must not be attributed yet to another existing file).
 */
void ShellCommandRenameFile(char *String_Current_File_Name, char *String_New_File_Name);

/** Get the size in bytes of an existing file. 
 * @param File_Name The name of the file.
 */
void ShellCommandFileSize(char *File_Name);

/** Copy the content of an existing file into a new one.
 * @param String_File_Name_Source The name of the file to copy.
 * @param String_File_Name_Destination The name of the file to be created.
 */
void ShellCommandCopyFile(char *String_File_Name_Source, char *String_File_Name_Destination);

#endif