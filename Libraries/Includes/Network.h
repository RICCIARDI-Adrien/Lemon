/** @file Network.h
 * Provide a TCP and UDP network stack able to communicate with the internet.
 * @author Adrien RICCIARDI
 */
#ifndef H_NETWORK_H
#define H_NETWORK_H

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
/** A packet buffer maximum length in bytes (this is the interface Maximum Transfer Unit (MTU)). */
#define NETWORK_MAXIMUM_PACKET_SIZE 1522 // Default ethernet controller value

/** The length in bytes of a MAC address. */
#define NETWORK_MAC_ADDRESS_SIZE 6

/** Swap a 32-bit value from one endianness mode to the other.
 * @param Double_Word The value to swap.
 * @return The swapped value.
 */
#define NETWORK_SWAP_DOUBLE_WORD(Double_Word) (((Double_Word) >> 24) | (((Double_Word) >> 8) & 0x0000FF00) | (((Double_Word) << 8) & 0x00FF0000) | (((Double_Word) << 24) & 0xFF000000))

/** Swap a 16-bit value from one endianness mode to the other.
 * @param Word The value to swap.
 * @return The swapped value.
 */
#define NETWORK_SWAP_WORD(Word) (((Word) >> 8) | (((Word) << 8) & 0xFF00))

/** Build a big endian IP address from the 4 decimal values (like 192.168.1.1).
 * @param Byte_1 The first address byte.
 * @param Byte_2 The second address byte.
 * @param Byte_3 The third address byte.
 * @param Byte_4 The fourth address byte.
 * @return The 32-bit IP address converted to big endian order.
 */
#define NETWORK_INITIALIZE_IP_ADDRESS(Byte_1, Byte_2, Byte_3, Byte_4) (((unsigned char) Byte_4 << 24) | ((unsigned char) Byte_3 << 16) | ((unsigned char) Byte_2 << 8) | (unsigned char) Byte_1)

/** Find the subnet mask according to the IP address class.
 * @param IP_Address The IP address in big endian mode.
 * @return The corresponding subnet mask in big endian mode too.
 */
#define NETWORK_GET_SUBNET_MASK_FROM_IP_ADDRESS(IP_Address) NETWORK_SWAP_DOUBLE_WORD((unsigned int) ((255 << 24) | (((IP_Address & 0x00000080) ? 255 : 0) << 16) | (((IP_Address & 0x00000040) ? 255 : 0) << 8))) // The unsigned int cast must forced or the result will be casted to a signed int, giving bad signed shifts when calling the swapping macro

/** The IPv4 "More Fragment" (MF) flag, tuned for the Flags_And_Fragment_Offset field of the IPv4 header (the field must be converted to little endian before testing the flag). */
#define NETWORK_IPV4_HEADER_FLAG_MORE_FRAGMENT (1 << 13)

/** A TFTP transfer block size in bytes. */
#define NETWORK_TFTP_BLOCK_SIZE 512

/** The TCP header size in 32-bit units shifted the right way to fit in the Header_Size_And_Flags field of the header. */
#define NETWORK_TCP_HEADER_SIZE ((sizeof(TNetworkTCPHeader) / 4) << 12)

/** The SYN flag bit in the TCP headers flags field. */
#define NETWORK_TCP_FLAG_SYN_BIT 1
/** The ACK flag bit in the TCP headers flags field. */
#define NETWORK_TCP_FLAG_ACK_BIT 4

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** An IPv4 address. */
typedef struct
{
	unsigned int Address; //!< The IP address.
	unsigned int Mask; //!< The subnet mask.
} TNetworkIPAddress;

/** Available IP protocols. */
typedef enum
{
	NETWORK_IP_PROTOCOL_TCP = 6, //<! Heavy connection-oriented protocol.
	NETWORK_IP_PROTOCOL_UDP = 17 //<! Lightweight connectionless protocol.
} TNetworkIPProtocol;

/** A network structure that contain all needed information to send and to receive layer 4 data. */
typedef struct
{
	unsigned char Destination_MAC_Address[NETWORK_MAC_ADDRESS_SIZE]; //!< Cache the MAC address corresponding to the IP machine to reach on the LAN when transmitting (target machine or gateway).
	unsigned int Destination_IP_Address; //!< The IP address of the target machine (when transmitting), used also to check the source of a received packet (stored in big endian).
	unsigned short Source_Port; //!< When transmitting, indicate the layer 4 port from where data were transmitted. When receiving, allow to match the received packet destination port with it to be sure that the packet is destined to this socket (stored in big endian).
	unsigned short Destination_Port; //!< Indicate the target machine port on transmission (stored in big endian).
	TNetworkIPProtocol IP_Protocol; //<! The type of the data embedded in the IP packet.
	unsigned int TCP_Sequence_Number; //<! The TCP sequence number (stored in little endian).
	unsigned int TCP_Acknowledgement_Number; //<! The TCP acknowledgement number (stored in little endian).
	int Is_TCP_Connection_Established; //<! Tell if the socket is successfully connected to a server or not.
} TNetworkSocket;

/** An ethernet header. No VLAN field is allowed. */
typedef struct __attribute__((packed))
{
	unsigned char Destination_MAC_Address[NETWORK_MAC_ADDRESS_SIZE]; //!< The receiver address.
	unsigned char Source_MAC_Address[NETWORK_MAC_ADDRESS_SIZE]; //!< The sender address.
	unsigned short Protocol_Type; //!< What protocol is encapsulated in the ethernet frame.
} TNetworkEthernetHeader;

/** An IPv4 header. Header options are not used by this network stack. */
typedef struct __attribute__((packed))
{
	unsigned char Version_And_Size; //!< Bits 7..4 must be set to the value '4', bits 3..0 represent the header length in double word unit.
	unsigned char Differentiated_Services_Code_Point_And_Explicit_Congestion_Notification; //!< Not needed by this network stack.
	unsigned short Total_Length; //!< The IPv4 header length plus the data length (in bytes).
	unsigned short Identification; //!< Not needed by this network stack.
	unsigned short Flags_And_Fragment_Offset; //!< Do not use bit fields as some versions of gcc generate bad code.
	unsigned char Time_To_Live; //!< How many equipment the packet can cross.
	unsigned char Protocol; //!< What is embedded in the IP data.
	unsigned short Header_Checksum; //!< The whole header checksum.
	unsigned int Source_IP_Address; //!< The packet sender address.
	unsigned int Destination_IP_Address; //!< The packet receiver address.
} TNetworkIPv4Header;

/** An UDP header. */
typedef struct __attribute__((packed))
{
	unsigned short Source_Port; //!< The sender's port.
	unsigned short Destination_Port; //!< The receiver's port.
	unsigned short Length; //!< Length in bytes of the UDP header plus the payload.
	unsigned short Checksum; //!< Optional checksum.
} TNetworkUDPHeader;

/** All TFTP opcodes (called packet types in the RFC). */
typedef enum
{
	NETWORK_TFTP_OPCODE_READ_REQUEST = 1,
	NETWORK_TFTP_OPCODE_WRITE_REQUEST,
	NETWORK_TFTP_OPCODE_DATA,
	NETWORK_TFTP_OPCODE_ACKNOWLEDGMENT,
	NETWORK_TFTP_OPCODE_ERROR
} TNetworkTFTPOpcode;

/** All known TFTP error codes. */
typedef enum
{
	NETWORK_TFTP_ERROR_CODE_NOT_DEFINED,
	NETWORK_TFTP_ERROR_CODE_FILE_NOT_FOUND,
	NETWORK_TFTP_ERROR_CODE_ACCESS_VIOLATION,
	NETWORK_TFTP_ERROR_CODE_DISK_FULL,
	NETWORK_TFTP_ERROR_CODE_ILLEGAL_TFTP_OPERATION,
	NETWORK_TFTP_ERROR_CODE_UNKNOWN_TRANSFER_ID,
	NETWORK_TFTP_ERROR_CODE_FILE_ALREADY_EXISTS,
	NETWORK_TFTP_ERROR_CODE_NO_SUCH_USER
} TNetworkTFTPErrorCode;

/** TFTP RRQ and WRQ specific header part. */
typedef struct __attribute__((packed))
{
	char String_File_Name_And_Mode[NETWORK_TFTP_BLOCK_SIZE]; //!< An ASCIIZ string for the file name followed by an ASCIIZ string for the transfer mode.
} TNetworkTFTPPacketRequest;

/** TFTP DATA packet specific header part. */
typedef struct __attribute__((packed))
{
	unsigned short Block_Number; //!< The block number.
	unsigned char Buffer[NETWORK_TFTP_BLOCK_SIZE]; //!< Store the transmitted/received data.
} TNetworkTFTPPacketData;

/** TFTP ACK packet specific header part. */
typedef struct __attribute__((packed))
{
	unsigned short Block_Number; //!< The block number.
} TNetworkTFTPPacketAcknowledgment;

/** TFTP ERROR packet specific header part. */
typedef struct __attribute__((packed))
{
	unsigned short Error_Code; //!< Use an error code from TNetworkTFTPErrorCode.
	char String_Error_Message[NETWORK_TFTP_BLOCK_SIZE]; //!< A human readable error string.
} TNetworkTFTPPacketError;

/** The TFTP protocol header for all existing packets. */
typedef struct __attribute__((packed))
{
	unsigned short Opcode; //!< Use an opcode from TNetworkTFTPOpcode.
	union
	{
		TNetworkTFTPPacketRequest Request; //!< RRQ and WRQ packets specific fields.
		TNetworkTFTPPacketData Data; //!< DATA packet specific fields.
		TNetworkTFTPPacketAcknowledgment Acknowledgment; //!< ACK packet specific fields.
		TNetworkTFTPPacketError Error; //!< ERROR packet specific fields.
	};
} TNetworkTFTPPacket;

/** A TCP header. */
typedef struct __attribute__((packed))
{
	unsigned short Source_Port; //!< The sender's port.
	unsigned short Destination_Port; //!< The receiver's port.
	unsigned int Sequence_Number; //!< TODO
	unsigned int Acknowledgment_Number; //!< TODO
	unsigned short Header_Size_And_Flags; //!< The header size in 32-bit unit (use NETWORK_TCP_HEADER_SIZE as no TCP option is used) ORed with NETWORK_TCP_FLAG_xxx flags.
	unsigned short Window_Size; //<! How many bytes the transmitter can send before waiting for the receiver to acknowledge.
	unsigned short Checksum; //!< Data and header checksum.
	unsigned short Urgent_Pointer; //!< Used only if the URG flag is set, point to urgent data first byte.
} TNetworkTCPHeader;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** This function must be called once at the application startup. It allows to configure essential network parameters.
 * @return 0 if the network subsystem was successfully initialized,
 * @return 1 if no network controller is available,
 * @return 2 if the network parameters could not be retrieved from the configuration file (parameters were missing or bad).
 */
int NetworkInitialize(void);

/** Initialize a socket content to transmit data to the specified IP address and to receive data from it.
 * @param Pointer_Destination_IP_Address The data recipient.
 * @param Destination_Port The IP encapsulated protocol destination port (this value is automatically swapped to big endian).
 * @param Used_Protocol Which protocol is encapsulated in the IP packet.
 * @param Pointer_Socket On output, contain the initialized socket.
 * @return 0 if the socket was successfully initialized,
 * @return 1 if the MAC address corresponding to the destination address could not be retrieved.
 */
int NetworkInitializeSocket(TNetworkIPAddress *Pointer_Destination_IP_Address, unsigned short Destination_Port, TNetworkIPProtocol Used_Protocol, TNetworkSocket *Pointer_Socket);

/** Fill a TNetworkIPAddress structure from an IPv4 dotted address string (network mask is automatically determined).
 * @param String_IP_Address A string representing the IP address using the same format than NetworkIPConvertFromString().
 * @param Pointer_IP_Address On output, contain the filled IP address.
 * @return 0 if the IP address structure was successfully filled,
 * @return The same errors than NetworkIPConvertFromString() in case of error.
 */
int NetworkInitializeIPAddress(char *String_IP_Address, TNetworkIPAddress *Pointer_IP_Address);

/** Convert an IPv4 dotted address string to a big endian double word.
 * @param String_IP_Address The IP address (like "192.168.1.3").
 * @param Pointer_IP_Address On output, contain the IP address converted to a big endian double word.
 * @return 0 if the address was successfully converted,
 * @return 1 if the IP address string is malformed.
 */
int NetworkIPConvertFromString(char *String_IP_Address, unsigned int *Pointer_IP_Address);

/** Send data over an UDP datagram.
 * @param Pointer_Socket The initialized socket indicating the destination machine.
 * @param Buffer_Size The data size in bytes.
 * @param Pointer_Buffer The data content.
 * @return 0 if data were transmitted,
 * @return 1 if data were too big for the device transfer size (lower data size and retry).
 */
int NetworkUDPSendBuffer(TNetworkSocket *Pointer_Socket, unsigned int Buffer_Size, void *Pointer_Buffer);

/** Receive data from carried by an UDP datagram.
 * @param Pointer_Socket The initialized socket describing the sender machine.
 * @param Is_Call_Blocking Set to 1 to wait indefinitely for a packet to be received, set to 0 to immediately exit with a return code telling if a packet has been received or not.
 * @param Pointer_Buffer_Size On output, contain the received data size.
 * @param Pointer_Buffer On output, contain the received data.
 * @return 0 if a packet has been received,
 * @return 1 if an error happened,
 * @return 2 if the received packet was too big and was discarded,
 * @return 3 in non-blocking mode only to tell that no packet was received.
 */
int NetworkUDPReceiveBuffer(TNetworkSocket *Pointer_Socket, int Is_Call_Blocking, unsigned int *Pointer_Buffer_Size, void *Pointer_Buffer);

/** Wait a specified time for a TFTP packet to be received. Automatically handle the server source port change.
 * @param Pointer_Socket The socket talking with the server. The initial destination port must be 69 (or the server one), it will be modified automatically when the first packet from the server is received (the previous port value is not restored).
 * @param Timeout_Milliseconds How many time to wait (in milliseconds) before reporting that no packet was available.
 * @param Pointer_Packet_Size On output, contain the TFTP packet size in bytes (including the TFTP header).
 * @param Pointer_Packet On output, contain the full TFTP packet.
 * @return 0 if a packet was successfully received,
 * @return 1 if an error occurred,
 * @return 2 if no packet was received in the allowed time.
 */
int NetworkTFTPReceivePacket(TNetworkSocket *Pointer_Socket, unsigned int Timeout_Milliseconds, unsigned int *Pointer_Packet_Size, TNetworkTFTPPacket *Pointer_Packet);

/** Connect to a listening TCP server.
 * @param Pointer_Socket A socket initialized with NetworkInitializeSocket().
 * @return 0 if the connection was successfully established,
 * @return 1 if a transmission error occurred,
 * @return 2 if the TCP server did not reply.
 * @note On output, the TCP-specific fields of the provided socket are initialized.
 */
int NetworkTCPConnectToServer(TNetworkSocket *Pointer_Socket);

#endif
