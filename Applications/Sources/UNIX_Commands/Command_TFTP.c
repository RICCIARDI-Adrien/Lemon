/** @file Command_TFTP.c
 * A simple TFTP client with no command line and a small amount of commands. Only "octet" transfer mode is supported. Implementation is based on RFC 1350.
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>
#include "Commands.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The data transfer mode for read and write requests. */
#define STRING_TFTP_TRANSFER_MODE "octet"

/** How many time to wait for a packet to be received (in milliseconds). */
#define TFTP_PACKET_RECEPTION_TIMEOUT 1000

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The socket used to communicate with the server. */
static TNetworkSocket Socket_Server;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Receive a file from the remote server.
 * @param String_File_Name The remote file name.
 * @return 0 if the file was successfully retrieved,
 * @return 1 if an error occurred.
 */
int TFTPExecuteCommandGet(char *String_File_Name)
{
	TNetworkTFTPPacket Packet;
	unsigned int File_Name_Length, Transfer_Mode_Length, File_ID, Data_Size;
	int Return_Value = 1;
	unsigned short Received_Block_Number, Expected_Block_Number = 1;
	
	// Prepare the read request
	Packet.Opcode = NETWORK_SWAP_WORD(NETWORK_TFTP_OPCODE_READ_REQUEST);
	// Append the requested file name
	LibrariesStringCopyUpToNumber(String_File_Name, Packet.Request.String_File_Name_And_Mode, LIBRARIES_FILE_NAME_LENGTH);
	File_Name_Length = LibrariesStringGetSize(Packet.Request.String_File_Name_And_Mode);
	// Append the transfer mode
	LibrariesStringCopy(STRING_TFTP_TRANSFER_MODE, &Packet.Request.String_File_Name_And_Mode[File_Name_Length + 1]); // Append the string right after the file name string terminating zero
	Transfer_Mode_Length = LibrariesStringGetSize(STRING_TFTP_TRANSFER_MODE);
	
	// Open the file to be ready to write it's content
	if (LibrariesFileOpen(String_File_Name, LIBRARIES_FILE_OPENING_MODE_WRITE, &File_ID) != 0)
	{
		LibrariesScreenWriteString(STRING_COMMAND_TFTP_GET_CANT_OPEN_FILE);
		return 1; // Do not go through Exit label path or a file with the same name than the one that would be opened could be deleted
	}
	
	// Send the read request
	if (NetworkUDPSendBuffer(&Socket_Server, sizeof(Packet.Opcode) + File_Name_Length + 1 + Transfer_Mode_Length + 1, &Packet) != 0) // +2 bytes for both strings terminating zeroes
	{
		LibrariesScreenWriteString(STRING_COMMAND_TFTP_GENERIC_NETWORK_TRANSMISSION_FAILED);
		goto Exit;
	}
	
	LibrariesScreenWriteString(STRING_COMMAND_TFTP_GET_STARTING_DOWNLOAD);
	
	// Receive the file
	do
	{
		// Get a packet
		if (NetworkTFTPReceivePacket(&Socket_Server, TFTP_PACKET_RECEPTION_TIMEOUT, &Data_Size, &Packet) != 0) // Data_Size contains for now the raw TFTP packet size
		{
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GENERIC_NETWORK_RECEPTION_FAILED);
			goto Exit;
		}
		Packet.Opcode = NETWORK_SWAP_WORD(Packet.Opcode);
		
		// Did an error occurred ?
		if (Packet.Opcode == NETWORK_TFTP_OPCODE_ERROR)
		{
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GENERIC_SERVER_ERROR_1);
			LibrariesScreenWriteString(Packet.Error.String_Error_Message);
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GENERIC_SERVER_ERROR_2);
			goto Exit;
		}
		
		// Is it a data packet ?
		if (Packet.Opcode != NETWORK_TFTP_OPCODE_DATA)
		{
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GENERIC_BAD_NETWORK_PACKET_RECEIVED);
			goto Exit;
		}
		
		// Make sure the received block is the expected one
		Received_Block_Number = NETWORK_SWAP_WORD(Packet.Data.Block_Number);
		if (Received_Block_Number != Expected_Block_Number)
		{
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GET_BAD_BLOCK_NUMBER_1);
			LibrariesScreenWriteUnsignedInteger(Received_Block_Number);
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GET_BAD_BLOCK_NUMBER_2);
			LibrariesScreenWriteUnsignedInteger(Expected_Block_Number);
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GET_BAD_BLOCK_NUMBER_3);
			goto Exit;
		}
		Expected_Block_Number++;
		
		// Adjust size to fit only the data size
		Data_Size -= sizeof(Packet.Opcode) + sizeof(Packet.Data.Block_Number);
		
		// Store data in the file
		if (LibrariesFileWrite(File_ID, Packet.Data.Buffer, Data_Size) != 0)
		{
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GET_CANT_WRITE_TO_FILE);
			goto Exit;
		}
		
		// Send an Acknowledgment packet
		Packet.Opcode = NETWORK_SWAP_WORD(NETWORK_TFTP_OPCODE_ACKNOWLEDGMENT); // No need to set the block number as it is at the same place than the one received (and it must have the same value)
		if (NetworkUDPSendBuffer(&Socket_Server, sizeof(Packet.Opcode) + sizeof(Packet.Data.Block_Number), &Packet) != 0)
		{
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GENERIC_NETWORK_TRANSMISSION_FAILED);
			goto Exit;
		}
	} while (Data_Size == NETWORK_TFTP_BLOCK_SIZE); // Exit if the data size is different from a block size
	
	// Display a success message
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
	LibrariesScreenWriteString(STRING_COMMAND_TFTP_GET_DOWNLOAD_SUCCESSFUL_1);
	LibrariesScreenWriteUnsignedInteger(NETWORK_SWAP_WORD(Packet.Data.Block_Number));
	LibrariesScreenWriteString(STRING_COMMAND_TFTP_GET_DOWNLOAD_SUCCESSFUL_2);
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
	Return_Value = 0;
	
Exit:
	LibrariesFileClose(File_ID);
	if (Return_Value != 0) LibrariesFileDelete(String_File_Name); // Remove the partial file
	return Return_Value;
}

/** Send a file to the remote server.
 * @param String_File_Name The local file name.
 * @return 0 if the file was successfully sent,
 * @return 1 if an error occurred.
 */
int TFTPExecuteCommandPut(char *String_File_Name)
{
	TNetworkTFTPPacket Packet;
	unsigned int File_ID, File_Name_Length, Transfer_Mode_Length, Received_Block_Number, Sent_Block_Number = 0, Read_Bytes_Count, Size;
	int Return_Value = 1;
	
	// Try to open the local file
	if (LibrariesFileOpen(String_File_Name, LIBRARIES_FILE_OPENING_MODE_READ, &File_ID) != 0)
	{
		LibrariesScreenWriteString(STRING_COMMAND_TFTP_PUT_CANT_OPEN_FILE);
		goto Exit;
	}
	
	// Prepare the write request
	Packet.Opcode = NETWORK_SWAP_WORD(NETWORK_TFTP_OPCODE_WRITE_REQUEST);
	// Append the sent file name
	LibrariesStringCopyUpToNumber(String_File_Name, Packet.Request.String_File_Name_And_Mode, LIBRARIES_FILE_NAME_LENGTH);
	File_Name_Length = LibrariesStringGetSize(Packet.Request.String_File_Name_And_Mode);
	// Append the transfer mode
	LibrariesStringCopy(STRING_TFTP_TRANSFER_MODE, &Packet.Request.String_File_Name_And_Mode[File_Name_Length + 1]); // Append the string right after the file name string terminating zero
	Transfer_Mode_Length = LibrariesStringGetSize(STRING_TFTP_TRANSFER_MODE);
	
	// Send the write request
	if (NetworkUDPSendBuffer(&Socket_Server, sizeof(Packet.Opcode) + File_Name_Length + 1 + Transfer_Mode_Length + 1, &Packet) != 0) // +2 bytes for both strings terminating zeroes
	{
		LibrariesScreenWriteString(STRING_COMMAND_TFTP_GENERIC_NETWORK_TRANSMISSION_FAILED);
		goto Exit;
	}
	
	LibrariesScreenWriteString(STRING_COMMAND_TFTP_PUT_STARTING_UPLOAD);
	
	// Receive the file
	do
	{
		// Get a packet
		if (NetworkTFTPReceivePacket(&Socket_Server, TFTP_PACKET_RECEPTION_TIMEOUT, &Size, &Packet) != 0)
		{
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GENERIC_NETWORK_RECEPTION_FAILED);
			goto Exit;
		}
		Packet.Opcode = NETWORK_SWAP_WORD(Packet.Opcode);
		
		// Did an error occurred ?
		if (Packet.Opcode == NETWORK_TFTP_OPCODE_ERROR)
		{
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GENERIC_SERVER_ERROR_1);
			LibrariesScreenWriteString(Packet.Error.String_Error_Message);
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GENERIC_SERVER_ERROR_2);
			goto Exit;
		}
		
		// Is it an ACK packet ?
		if (Packet.Opcode != NETWORK_TFTP_OPCODE_ACKNOWLEDGMENT)
		{
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GENERIC_BAD_NETWORK_PACKET_RECEIVED);
			goto Exit;
		}
		
		// Make sure the acknowledged block number is the good one
		Received_Block_Number = NETWORK_SWAP_WORD(Packet.Acknowledgment.Block_Number);
		if (Received_Block_Number != Sent_Block_Number)
		{
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_PUT_BAD_BLOCK_NUMBER_1);
			LibrariesScreenWriteUnsignedInteger(Received_Block_Number);
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_PUT_BAD_BLOCK_NUMBER_2);
			LibrariesScreenWriteUnsignedInteger(Sent_Block_Number);
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_PUT_BAD_BLOCK_NUMBER_3);
			goto Exit;
		}
		Sent_Block_Number++;
		
		// Read the next block from the file
		if (LibrariesFileRead(File_ID, Packet.Data.Buffer, NETWORK_TFTP_BLOCK_SIZE, &Read_Bytes_Count) != 0)
		{
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_PUT_CANT_READ_FROM_FILE);
			goto Exit;
		}
		
		// Prepare the DATA packet
		Packet.Opcode = NETWORK_SWAP_WORD(NETWORK_TFTP_OPCODE_DATA);
		Packet.Data.Block_Number = NETWORK_SWAP_WORD(Sent_Block_Number);
		
		// Send the packet (even if the read data was 0 byte, which will tell the server that the transfer is terminated)
		if (NetworkUDPSendBuffer(&Socket_Server, sizeof(Packet.Opcode) + sizeof(Packet.Data.Block_Number) + Read_Bytes_Count, &Packet) != 0)
		{
			LibrariesScreenWriteString(STRING_COMMAND_TFTP_GENERIC_NETWORK_TRANSMISSION_FAILED);
			goto Exit;
		}
	} while (Read_Bytes_Count == NETWORK_TFTP_BLOCK_SIZE); // Exit when the whole file has been sent
	
	// Display a success message
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_GREEN);
	LibrariesScreenWriteString(STRING_COMMAND_TFTP_PUT_UPLOAD_SUCCESSFUL_1);
	LibrariesScreenWriteUnsignedInteger(Sent_Block_Number);
	LibrariesScreenWriteString(STRING_COMMAND_TFTP_PUT_UPLOAD_SUCCESSFUL_2);
	LibrariesScreenSetFontColor(LIBRARIES_SCREEN_COLOR_BLUE);
	Return_Value = 0;
	
Exit:
	LibrariesFileClose(File_ID);
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
	char *String_File_Name, String_Temp[32];
	
	// Check parameters
	if (argc != 3)
	{
		LibrariesScreenWriteString(STRING_COMMAND_TFTP_USAGE);
		return -1;
	}
	
	// Get parameters
	// Command
	if (LibrariesStringCompare(argv[1], "get") == 1) Is_Get_Command = 1;
	else if (LibrariesStringCompare(argv[1], "put") == 1) Is_Get_Command = 0;
	else
	{
		LibrariesScreenWriteString(STRING_COMMAND_TFTP_INVALID_COMMAND);
		return -1;
	}
	// File name
	String_File_Name = argv[2];
	if (LibrariesStringGetSize(String_File_Name) > LIBRARIES_FILE_NAME_LENGTH)
	{
		LibrariesScreenWriteString(STRING_COMMAND_TFTP_FILE_NAME_TOO_LONG);
		return -1;
	}
	
	// Retrieve server parameters from the configuration file
	// IP address
	if (LibrariesConfigurationReadValue("Network_TFTP_Server_IP_Address", String_Temp) != 0)
	{
		LibrariesScreenWriteString(STRING_COMMAND_TFTP_CAN_RETRIEVE_SERVER_IP_ADDRESS);
		return -1;
	}
	if (NetworkInitializeIPAddress(String_Temp, &IP_Address) != 0) // TODO initialize subnet mask too
	{
		LibrariesScreenWriteString(STRING_COMMAND_TFTP_INVALID_IP_ADDRESS);
		return -1;
	}
	// Port
	if (LibrariesConfigurationReadValue("Network_TFTP_Server_Port", String_Temp) != 0)
	{
		LibrariesScreenWriteString(STRING_COMMAND_TFTP_CAN_RETRIEVE_SERVER_PORT);
		return -1;
	}
	Port = LibrariesStringConvertStringToUnsignedInteger(String_Temp);
	
	// Try to connect to the server
	if (NetworkInitialize() != 0)
	{
		LibrariesScreenWriteString(STRING_COMMAND_TFTP_NETWORK_INITIALIZATION_FAILED);
		return -1;
	}
	// Try to create the socket
	if (NetworkInitializeSocket(&IP_Address, Port, NETWORK_IP_PROTOCOL_UDP, &Socket_Server) != 0)
	{
		LibrariesScreenWriteString(STRING_COMMAND_TFTP_NETWORK_SOCKET_INITIALIZATION_FAILED);
		return -1;
	}
	
	// Execute the requested command
	if (Is_Get_Command) return TFTPExecuteCommandGet(String_File_Name);
	else TFTPExecuteCommandPut(String_File_Name);
	
	return 0;
}