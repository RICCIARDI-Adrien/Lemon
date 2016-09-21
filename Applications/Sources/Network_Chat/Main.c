/** @file Main.c
 * A really simple chat client communicating with UDP datagrams.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Interface.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Contain the last written user message. */
static char String_Main_User_Message[INTERFACE_USER_MESSAGE_MAXIMUM_SIZE + 1] = {0}; // +1 for a terminating zero
/** The user message current length. */
static unsigned int Main_User_Message_Length = 0;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Handle user keyboard input.
 * @return 0 if nothing notable happened,
 * @return 1 if the user message must be redrawn due to a change,
 * @return 2 if the enter key has been pressed, telling that the user message must be sent and displayed,
 * @return 3 if the user hit escape key, telling that the user wants to exit from the program.
 */
static int MainReadUserMessage(void)
{
	unsigned char Key;
	
	// Get the last pressed key
	Key = KeyboardReadCharacter();
	
	switch (Key)
	{
		// Exit program
		case KEYBOARD_KEY_CODE_ESCAPE:
			return 3;
			
		case KEYBOARD_KEY_CODE_ENTER:
			if (Main_User_Message_Length == 0) return 0; // The message must be almost one-character long to be displayed
			return 2;
			
		// Remove the last typed character
		case KEYBOARD_KEY_CODE_BACKSPACE:
			if (Main_User_Message_Length == 0) return 0; // Nothing to delete if the message is empty
			Main_User_Message_Length--;
			String_Main_User_Message[Main_User_Message_Length] = 0;
			return 1;
			
		// Printable characters
		default:
			// Make sure the characters are printable
			if (((Key != 248) && (Key != 253)) && ((Key < 32) || (Key > 156))) return 0; // Special rule to allow '°' and '²' characters
			
			// Is there enough room in the buffer to add the character ?
			if (Main_User_Message_Length == INTERFACE_USER_MESSAGE_MAXIMUM_SIZE) return 0;
			
			// Append the character
			String_Main_User_Message[Main_User_Message_Length] = Key;
			Main_User_Message_Length++;
			return 1;
	}
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int __attribute__((unused)) argc, char __attribute__((unused)) *argv[])
{
	TNetworkIPAddress System_IP_Address, Gateway_IP_Address, Destination_IP_Address;
	TNetworkSocket Socket;
	
	// TEST
	System_IP_Address.Address = NETWORK_MAKE_IP_ADDRESS(192, 168, 60, 2);
	System_IP_Address.Mask = NETWORK_MAKE_IP_ADDRESS(255, 255, 255, 0);
	Gateway_IP_Address.Address = NETWORK_MAKE_IP_ADDRESS(192, 168, 60, 1);
	Gateway_IP_Address.Mask = NETWORK_MAKE_IP_ADDRESS(255, 255, 255, 0);
	Destination_IP_Address.Address = NETWORK_MAKE_IP_ADDRESS(192, 168, 60, 1);
	Destination_IP_Address.Mask = NETWORK_MAKE_IP_ADDRESS(255, 255, 255, 0);
	
	// Initialize the network stack
	if (NetworkInitialize(&System_IP_Address, &Gateway_IP_Address) != 0)
	{
		ScreenSetFontColor(SCREEN_COLOR_RED);
		ScreenWriteString(STRING_ERROR_NETWORK_INITIALIZATION);
		return 1;
	}
	
	if (NetworkInitializeTransmissionSocket(&Destination_IP_Address, 1234, NETWORK_IP_PROTOCOL_UDP, &Socket) != 0)
	{
		ScreenSetFontColor(SCREEN_COLOR_RED);
		ScreenWriteString(STRING_ERROR_NETWORK_SOCKET_INITIALIZATION);
		return 1;
	}
	
	// Prepare the screen
	ScreenClear();
	InterfaceDisplayUserMessage(""); // Display an empty prompt
	
	while (1)
	{
		// Is a message received from the network ?
		
		// Is a key pressed ?
		switch (MainReadUserMessage())
		{
			// Something changed in the user message, redraw it
			case 1:
				InterfaceDisplayUserMessage(String_Main_User_Message);
				break;
				
			// The user sent the message
			case 2:
				// Display the message
				InterfaceDisplayMessage(String_Main_User_Message, SCREEN_COLOR_BLUE);
				
				// Append a new line character
				String_Main_User_Message[Main_User_Message_Length] = '\n'; // The user message buffer is one byte larger than the maximum message size to allow a terminating zero to be inserted, so the new line character appending can't overflow this buffer
				Main_User_Message_Length++;
				// Send the message
				NetworkUDPSendBuffer(&Socket, Main_User_Message_Length, String_Main_User_Message);
				
				// Display an empty user message prompt
				Main_User_Message_Length = 0;
				MemorySetAreaValue(String_Main_User_Message, sizeof(String_Main_User_Message), 0); // Clear all message bytes because if some data remain, there is a time when the current terminating zero will be overwritten and that the last character will become a previous message's one, resulting in displaying the previous message end
				InterfaceDisplayUserMessage("");
				break;
			
			case 3:
				goto Exit;
				
			default:
				break;
		}
	}
	
//Exit_Error:
	
	
Exit:
	// Put the cursor at the beginning of the last screen line
	InterfaceDisplayMessage("", SCREEN_COLOR_WHITE); // Scroll the screen a last time to make the last line blank
	InterfaceDisplayUserMessage("");
	ScreenSetCursorPosition(SCREEN_ROWS_COUNT - 1, 0);
	return 0;
}