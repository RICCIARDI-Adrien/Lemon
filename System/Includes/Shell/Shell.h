/** @file Shell.h
 * System shell core.
 * @author Adrien RICCIARDI
 * @version 1.0 : 22/12/2012
 * @version 1.1 : 29/12/2012, added to kernel code.
 * @version 1.2 : 23/08/2013, simplified command line arguments code.
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