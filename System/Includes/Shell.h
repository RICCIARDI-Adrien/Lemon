/** @file Shell.h
 * System shell core.
 * @author Adrien RICCIARDI
 */
#ifndef H_SHELL_H
#define H_SHELL_H

#include <File_System/File_System.h> // Needed for TFileSystemMasterBootLoaderPartitionTableEntry type

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
/** Maximum length of the shell line in characters. */
#define SHELL_MAXIMUM_LINE_LENGTH 77

/** Maximum arguments count into the command line. */
#define SHELL_MAXIMUM_ARGUMENTS_COUNT 20

/** Clear the screen. */
#define SHELL_COMMAND_CLEAR_SCREEN "clear"
/** List existing files. */
#define SHELL_COMMAND_LIST_FILES "list"
/** Download a file from the serial port. */
#define SHELL_COMMAND_DOWNLOAD_FILE "download"
/** Delete an existing file. */
#define SHELL_COMMAND_DELETE_FILE "delete"
/** Rename an existing file. */
#define SHELL_COMMAND_RENAME_FILE "rename"
/** Get the size in bytes of an existing file. */
#define SHELL_COMMAND_FILE_SIZE "size"
/** Copy the content of an existing file into a new one. */
#define SHELL_COMMAND_COPY_FILE "copy"
/** Show the system version. */
#define SHELL_COMMAND_VERSION "version"

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Shell entry point. */
void Shell(void);

/** List existing files. */
void ShellCommandList(void);

/** Download a file from the serial port and store it into the user space. */
void ShellCommandDownload(void);

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
 * @param String_File_Name The name of the file.
 */
void ShellCommandFileSize(char *String_File_Name);

/** Copy the content of an existing file into a new one.
 * @param String_File_Name_Source The name of the file to copy.
 * @param String_File_Name_Destination The name of the file to be created.
 */
void ShellCommandCopyFile(char *String_File_Name_Source, char *String_File_Name_Destination);

// Installer specific functions
/** Display a section title.
 * @param String_Title The title to display.
 */
void ShellInstallerDisplayTitle(char *String_Title);

/** Allow the user to choose one of the available partitions.
 * @return A pointer on the corresponding partition table to write to the Lemon MBR.
 */
TFileSystemMasterBootLoaderPartitionTableEntry *ShellInstallerPartitionMenu(void);

#endif