/** @file Interface.h
 * Display chat messages and a prompt to write a message.
 * @author Adrien RICCIARDI
 */
#ifndef H_INTERFACE_H
#define H_INTERFACE_H

#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How long a user message can be. */
#define INTERFACE_USER_MESSAGE_MAXIMUM_SIZE ((LIBRARIES_SCREEN_COLUMNS_COUNT * 4) - 2) // -2 to take the user message prompt into account

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Display a message at the displaying area end.
 * @param String_Message The message to display.
 * @param Message_Color The color to set when displaying the message.
 */
void InterfaceDisplayMessage(char *String_Message, TLibrariesScreenColor Message_Color);

/** Display a prompt at the screen top followed by the user message content.
 * @param String_Message The user message content.
 */
void InterfaceDisplayUserMessage(char *String_Message);

#endif