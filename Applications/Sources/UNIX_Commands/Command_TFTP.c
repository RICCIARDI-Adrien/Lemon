/** @file Command_TFTP.c
 * A simple TFTP client with no command line and a small amount of commands. Only "octet" transfer mode is supported. Implementation is based on RFC 1350.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Commands.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The socket used to communicate with the server. */
static TNetworkSocket Socket_Server;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Get a file from the remote server.
 * @param String_File_Name The remove file name.
 * @return 0 if the file was successfully retrieved,
 * @return 1 if an error occurred.
 */
int TFTPExecuteCommandGet(char *String_File_Name)
{
	TNetworkTFTPPacket Packet;
	unsigned int File_Name_Length, Transfer_Mode_Length, File_ID, Data_Size;
	char *String_Transfer_Mode = "octet";
	int Return_Value = 1;
	unsigned short Received_Block_Number, Expected_Block_Number = 1;
	
	// Prepare the read request
	Packet.Opcode = NETWORK_SWAP_WORD(NETWORK_TFTP_OPCODE_READ_REQUEST);
	// Append the requested file name
	StringCopyUpToNumber(String_File_Name, Packet.Request.String_File_Name_And_Mode, FILE_NAME_LENGTH);
	File_Name_Length = StringGetSize(Packet.Request.String_File_Name_And_Mode);
	// Append the transfer mode
	StringCopy(String_Transfer_Mode, &Packet.Request.String_File_Name_And_Mode[File_Name_Length + 1]); // Append the string right after the file name string terminating zero
	Transfer_Mode_Length = StringGetSize(String_Transfer_Mode);
	
	// Open the file to be ready to write it's content
	if (FileOpen(String_File_Name, FILE_OPENING_MODE_WRITE, &File_ID) != 0)
	{
		ScreenWriteString(STRING_COMMAND_TFTP_CANT_OPEN_FILE);
		return 1; // Do not go through Exit label path or a file with the same name than the one that would be opened could be deleted
	}
	
	// Send the read request
	if (NetworkUDPSendBuffer(&Socket_Server, sizeof(Packet.Opcode) + File_Name_Length + 1 + Transfer_Mode_Length + 1, &Packet) != 0) // +2 bytes for both strings terminating zeroes
	{
		ScreenWriteString(STRING_COMMAND_TFTP_NETWORK_TRANSMISSION_FAILED);
		return -1;
	}
	
	ScreenWriteString(STRING_COMMAND_TFTP_STARTING_DOWNLOAD);
	
	// Receive the file
	while (1)
	{
		// Get a packet
		if (NetworkTFTPReceivePacket(&Socket_Server, 1000, &Data_Size, &Packet) != 0) // Data_Size contains for now the raw TFTP packet size
		{
			ScreenWriteString(STRING_COMMAND_TFTP_NETWORK_RECEPTION_FAILED);
			goto Exit;
		}
		Packet.Opcode = NETWORK_SWAP_WORD(Packet.Opcode);
		
		// Did an error occurred ?
		if (Packet.Opcode == NETWORK_TFTP_OPCODE_ERROR)
		{
			ScreenWriteString(STRING_COMMAND_TFTP_NETWORK_SERVER_ERROR_1);
			ScreenWriteString(Packet.Error.String_Error_Message);
			ScreenWriteString(STRING_COMMAND_TFTP_NETWORK_SERVER_ERROR_2);
			goto Exit;
		}
		
		// Is it a data packet ?
		if (Packet.Opcode != NETWORK_TFTP_OPCODE_DATA)
		{
			ScreenWriteString(STRING_COMMAND_TFTP_NETWORK_BAD_PACKET_RECEIVED);
			goto Exit;
		}
		
		// Make sure the received block is the expected one
		Received_Block_Number = NETWORK_SWAP_WORD(Packet.Data.Block_Number);
		if (Received_Block_Number != Expected_Block_Number)
		{
			ScreenWriteString(STRING_COMMAND_TFTP_BAD_BLOCK_NUMBER_1);
			ScreenWriteUnsignedInteger(Received_Block_Number);
			ScreenWriteString(STRING_COMMAND_TFTP_BAD_BLOCK_NUMBER_2);
			ScreenWriteUnsignedInteger(Expected_Block_Number);
			ScreenWriteString(STRING_COMMAND_TFTP_BAD_BLOCK_NUMBER_3);
			goto Exit;
		}
		Expected_Block_Number++;
		
		// Adjust size to fit only the data size
		Data_Size -= sizeof(Packet.Opcode) + sizeof(Packet.Data.Block_Number);
		
		// Store data in the file
		if (FileWrite(File_ID, Packet.Data.Buffer, Data_Size) != 0)
		{
			ScreenWriteString(STRING_COMMAND_TFTP_CANT_WRITE_TO_FILE);
			goto Exit;
		}
		
		// Send an Acknowledgment packet
		Packet.Opcode = NETWORK_SWAP_WORD(NETWORK_TFTP_OPCODE_ACKNOWLEDGMENT); // No need to set the block number as it is at the same place than the one received (and it must have the same value)
		if (NetworkUDPSendBuffer(&Socket_Server, sizeof(Packet.Opcode) + sizeof(Packet.Data.Block_Number), &Packet) != 0)
		{
			ScreenWriteString(STRING_COMMAND_TFTP_NETWORK_TRANSMISSION_FAILED);
			goto Exit;
		}
		
		// Exit if the data size is different from a block size
		if (Data_Size != NETWORK_TFTP_BLOCK_SIZE) break;
	}
	
	// Display a success message
	ScreenSetFontColor(SCREEN_COLOR_GREEN);
	ScreenWriteString(STRING_COMMAND_TFTP_DOWNLOAD_SUCCESSFUL_1);
	ScreenWriteUnsignedInteger(NETWORK_SWAP_WORD(Packet.Data.Block_Number));
	ScreenWriteString(STRING_COMMAND_TFTP_DOWNLOAD_SUCCESSFUL_2);
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
	Return_Value = 0;
	
Exit:
	FileClose(File_ID);
	if (Return_Value != 0) FileDelete(String_File_Name); // Remove the partial file
	return Return_Value;
}

//-------------------------------------------------------------------------------------------------
// Command entry point
//-------------------------------------------------------------------------------------------------
int CommandMainTFTP(int argc, char __attribute__((unused)) *argv[])
{
	TNetworkIPAddress IP_Address;
	unsigned short Port;
	int Is_Get_Command;
	char *String_File_Name;
	
	// Check parameters
	if (argc != 5)
	{
		ScreenWriteString(STRING_COMMAND_TFTP_USAGE);
		return -1;
	}
	
	// Get parameters
	// IP address
	if (NetworkInitializeIPAddress(argv[1], &IP_Address) != 0)
	{
		ScreenWriteString(STRING_COMMAND_TFTP_INVALID_IP_ADDRESS);
		return -1;
	}
	// Port
	Port = StringConvertStringToUnsignedInteger(argv[2]);
	// Command
	if (StringCompare(argv[3], "get") == 1) Is_Get_Command = 1;
	else if (StringCompare(argv[3], "put") == 1) Is_Get_Command = 0;
	else
	{
		ScreenWriteString(STRING_COMMAND_TFTP_INVALID_COMMAND);
		return -1;
	}
	// File name
	String_File_Name = argv[4];
	if (StringGetSize(String_File_Name) > FILE_NAME_LENGTH)
	{
		ScreenWriteString(STRING_COMMAND_TFTP_FILE_NAME_TOO_LONG);
		return -1;
	}
	
	// Try to connect to the server
	if (NetworkInitialize() != 0)
	{
		ScreenWriteString(STRING_COMMAND_TFTP_NETWORK_INITIALIZATION_FAILED);
		return -1;
	}
	// Try to create the socket
	if (NetworkInitializeSocket(&IP_Address, Port, NETWORK_IP_PROTOCOL_UDP, &Socket_Server) != 0)
	{
		ScreenWriteString(STRING_COMMAND_TFTP_NETWORK_SOCKET_INITIALIZATION_FAILED);
		return -1;
	}
	
	// Execute the requested command
	if (Is_Get_Command) return TFTPExecuteCommandGet(String_File_Name);
	//else TODO
	
	return 0;
}