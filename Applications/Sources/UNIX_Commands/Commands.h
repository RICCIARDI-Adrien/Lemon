/** @file Commands.h
 * List all the supported commands.
 * @author Adrien RICCIARDI
 */
#ifndef H_COMMANDS_H
#define H_COMMANDS_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** The "date" command.
 * @param argc Parameters count.
 * @param argv Parameters value.
 * @return 0 in case of success,
 * @return a negative value if an error happened.
 */
int CommandMainDate(int argc, char *argv[]);

/** The "disk free" command.
 * @param argc Parameters count.
 * @param argv Parameters value.
 * @return 0 in case of success,
 * @return a negative value if an error happened.
 */
int CommandMainDf(int argc, char *argv[]);

/** The "diff" command.
 * @param argc Parameters count.
 * @param argv Parameters value.
 * @return 0 in case of success,
 * @return a negative value if an error happened.
 */
int CommandMainDiff(int argc, char *argv[]);

/** The "list" command.
 * @param argc Parameters count.
 * @param argv Parameters value.
 * @return 0 in case of success,
 * @return a negative value if an error happened.
 */
int CommandMainLs(int argc, char *argv[]);

/** The "more" command.
 * @param argc Parameters count.
 * @param argv Parameters value.
 * @return 0 in case of success,
 * @return a negative value if an error happened.
 */
int CommandMainMore(int argc, char *argv[]);

/** The "tftp" command.
 * @param argc Parameters count.
 * @param argv Parameters value.
 * @return 0 in case of success,
 * @return a negative value if an error happened.
 */
int CommandMainTFTP(int argc, char *argv[]);

/** The "uptime" command.
 * @param argc Parameters count.
 * @param argv Parameters value.
 * @return 0 in case of success,
 * @return a negative value if an error happened.
 */
int CommandMainUptime(int argc, char *argv[]);

#endif
