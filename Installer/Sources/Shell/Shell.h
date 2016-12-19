/** @file Shell.h
 * Installer shell.
 * @author Adrien RICCIARDI
 */
#ifndef H_SHELL_H
#define H_SHELL_H

#include <File_System/File_System.h>

//-------------------------------------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Display a section title.
 * @param String_Title The title to display.
 */
void ShellDisplayTitle(char *String_Title);

/** Display an error message.
 * @param String_Error_Message The error message to display.
 */
/*void ShellDisplayErrorMessage(char *String_Error_Message);*/ // TODO

/** Allow the user to choose one of the available partitions.
 * @return A pointer on the corresponding partition table to write to the Lemon MBR.
 */
TFileSystemMasterBootLoaderPartitionTableEntry *ShellPartitionMenu(void);

#endif