/** @file Display_Message.c
 * Show specific messages on the screen.
 * @author Adrien RICCIARDI
 * @version 1.0 : 10/12/2014
 */
#ifndef H_DISPLAY_MESSAGE_H
#define H_DISPLAY_MESSAGE_H

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
/** Display the next test to do.
 * @param String_Message The message to display.
 */
void DisplayMessageTestStarting(char *String_Message);

/** Display a success message. */
void DisplayMessageTestSuccessful(void);

/** Halt the program after displaying an error message. */
void DisplayMessageTestFailed(void);

/** Display an error code signification.
 * @param String_Message The message to display (it is automatically prefixed by "Error " and suffixed by " : \<error code signification\>\n").
 * @param Error_Code The error code.
 */
void DisplayMessageErrorAndCode(char *String_Message, int Error_Code);

/** Display a red message prefixed by "Error : ".
 * @param String_Message The message to display.
 */
void DisplayMessageError(char *String_Message);

#endif