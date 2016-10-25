/** @file Command_TFTP.c
 * A simple TFTP client with no command line and a small amount of commands. Only "octet" transfer mode is supported. Implementation is based on RFC 1350.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Commands.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** A transfer block size in bytes. */
#define TFTP_BLOCK_SIZE 512

/** A file name maximum length (in characters). */
#define TFTP_FILE_NAME_MAXIMUM_SIZE 256

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** All TFTP opcodes (called packet types in the RFC). */
typedef enum
{
	TFTP_OPCODE_READ_REQUEST = 1,
	TFTP_OPCODE_WRITE_REQUEST,
	TFTP_OPCODE_DATA,
	TFTP_OPCODE_ACKNOWLEDGMENT,
	TFTP_OPCODE_ERROR
} TTFTPOpcode;

/** All known error codes. */
typedef enum
{
	TFTP_ERROR_CODE_NOT_DEFINED,
	TFTP_ERROR_CODE_FILE_NOT_FOUND,
	TFTP_ERROR_CODE_ACCESS_VIOLATION,
	TFTP_ERROR_CODE_DISK_FULL,
	TFTP_ERROR_CODE_ILLEGAL_TFTP_OPERATION,
	TFTP_ERROR_CODE_UNKNOWN_TRANSFER_ID,
	TFTP_ERROR_CODE_FILE_ALREADY_EXISTS,
	TFTP_ERROR_CODE_NO_SUCH_USER
} TTFTPErrorCode;

/** RRQ and WRQ specific header part. */
typedef struct __attribute__((packed))
{
	char String_File_Name_And_Mode[TFTP_BLOCK_SIZE]; //!< An ASCIIZ string for the file name followed by an ASCIIZ string for the transfer mode.
} TTFTPPacketRequest;

/** DATA packet specific header part. */
typedef struct __attribute__((packed))
{
	unsigned short Block_Number; //!< The block number.
	unsigned char Buffer[TFTP_BLOCK_SIZE]; //!< Store the transmitted/received data.
} TTFTPPacketData;

/** ACK packet specific header part. */
typedef struct __attribute__((packed))
{
	unsigned short Block_Number; //!< The block number.
} TTFTPPacketAcknowledgment;

/** ERROR packet specific header part. */
typedef struct __attribute__((packed))
{
	unsigned short Error_Code; //!< Use an error code from TTFTPErrorCode.
	char String_Error_Message[TFTP_BLOCK_SIZE]; //!< A human readable error string.
} TTFTPPacketError;

/** The TFTP protocol header for all existing packets. */
typedef struct __attribute__((packed))
{
	unsigned short Opcode; //!< Use an opcode from TTFTPOpcode.
	union
	{
		TTFTPPacketRequest Request; //!< RRQ and WRQ packets specific fields.
		TTFTPPacketData Data; //!< DATA packet specific fields.
		TTFTPPacketAcknowledgment Acknowledgment; //!< ACK packet specific fields.
		TTFTPPacketError Error; //!< ERROR packet specific fields.
	};
} TTFTPPacket;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The socket used to communicate with the server. */
static TNetworkSocket Socket_Server;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** TODO */
int TFTPExecuteCommandGet(char *String_File_Name)
{
	TTFTPPacket Packet;
	unsigned int File_Name_Length, Transfer_Mode_Length;
	char *String_Transfer_Mode = "octet";
	
	// Prepare the read request
	Packet.Opcode = NETWORK_SWAP_WORD(TFTP_OPCODE_READ_REQUEST);
	// Append the requested file name
	StringCopyUpToNumber(String_File_Name, Packet.Request.String_File_Name_And_Mode, TFTP_FILE_NAME_MAXIMUM_SIZE);
	File_Name_Length = StringGetSize(Packet.Request.String_File_Name_And_Mode);
	// Append the transfer mode
	StringCopy(String_Transfer_Mode, &Packet.Request.String_File_Name_And_Mode[File_Name_Length + 1]); // Append the string right after the file name string terminating zero
	Transfer_Mode_Length = StringGetSize(String_Transfer_Mode);
	
	// Send the read request
	if (NetworkUDPSendBuffer(&Socket_Server, sizeof(Packet.Opcode) + File_Name_Length + 1 + Transfer_Mode_Length + 1, &Packet) != 0) // +2 bytes for both strings terminating zeroes
	{
		ScreenWriteString(STRING_COMMAND_TFTP_NETWORK_TRANSMISSION_FAILED);
		return -1;
	}
	
	return 0;
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