/** @file Commands.h
 * List all the supported commands.
 * @author Adrien RICCIARDI
 * @version 1.0 : 18/03/2015
 */
#ifndef H_COMMANDS_H
#define H_COMMANDS_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** The "disk free" command.
 * @param argc Parameters count.
 * @param argv Parameters value.
 * @return 0 in case of success,
 * @return a negative value if an error happened.
 */
int CommandMainDf(int argc, char *argv[]);

/** The "list" command.
 * @param argc Parameters count.
 * @param argv Parameters value.
 * @return 0 in case of success,
 * @return a negative value if an error happened.
 */
int CommandMainLs(int argc, char *argv[]);

#endif