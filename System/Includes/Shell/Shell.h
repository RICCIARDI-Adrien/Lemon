/** @file Shell.h
 * System shell core.
 * @author Adrien RICCIARDI
 */
#ifndef H_SHELL_H
#define H_SHELL_H

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
/** Maximum length of the shell line in characters. */
#define SHELL_MAXIMUM_LINE_LENGTH 77

/** Maximum arguments count into the command line. */
#define SHELL_MAXIMUM_ARGUMENTS_COUNT 20

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Shell entry point. */
void Shell(void);

#endif