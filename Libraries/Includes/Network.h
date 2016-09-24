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
#define NETWORK_MAXIMUM_PACKET_SIZE 1500

/** The length in bytes of a MAC address. */
#define NETWORK_MAC_ADDRESS_SIZE 6

/** Swap a 32-bit value from one endianness mode to the other.
 * @param Double_Word The value to swap.
 * @return The swapped value.
 */
#define NETWORK_SWAP_DOUBLE_WORD(Double_Word) ((Double_Word >> 24) | ((Double_Word >> 8) & 0x0000FF00) | ((Double_Word << 8) & 0x00FF0000) | ((Double_Word << 24) & 0xFF000000))

/** Swap a 16-bit value from one endianness mode to the other.
 * @param Word The value to swap.
 * @return The swapped value.
 */
#define NETWORK_SWAP_WORD(Word) ((Word >> 8) | ((Word << 8) & 0xFF00))

/** Build a big endian IP address from the 4 decimal values (like 192.168.1.1).
 * @param Byte_1 The first address byte.
 * @param Byte_2 The second address byte.
 * @param Byte_3 The third address byte.
 * @param Byte_4 The fourth address byte.
 * @return The 32-bit IP address converted to big endian order.
 */
#define NETWORK_INITIALIZE_IP_ADDRESS(Byte_1, Byte_2, Byte_3, Byte_4) (((unsigned char) Byte_4 << 24) | ((unsigned char) Byte_3 << 16) | ((unsigned char) Byte_2 << 8) | (unsigned char) Byte_1)

/** TODO */
#define NETWORK_INITIALIZE_SUBNET_MASK(Byte_1, Byte_2, Byte_3, Byte_4) NETWORK_SWAP_DOUBLE_WORD((255 << 24) | (((Byte_1 & (1 << 7)) ? 255 : 0) << 16) | (((Byte_1 & (1 << 6)) ? 255 : 0) << 8))

/** Find the subnet mask according to the IP address class.
 * @param IP_Address The IP address in big endian mode.
 * @return The corresponding subnet mask in big endian mode too.
 */
#define NETWORK_GET_SUBNET_MASK_FROM_IP_ADDRESS(IP_Address) ((255 << 24) | (((IP_Address & 0x00000001) ? 255 : 0) << 16) | (((IP_Address & 0x00000002) ? 255 : 0) << 8))

#if 0
/** Initialize a TNetworkSocket with the provided IP address. The subnet mask is automatically determined from the IP address.
 * @param IP_Address_Byte_1 The first address byte.
 * @param IP_Address_Byte_2 The second address byte.
 * @param IP_Address_Byte_3 The third address byte.
 * @param IP_Address_Byte_4 The fourth address byte.
 * @return An initializer for a socket structure.
 */
#define NETWORK_INITIALIZE_SOCKET(IP_Address_Byte_1, IP_Address_Byte_2, IP_Address_Byte_3, IP_Address_Byte_4) \
{ \
	.Address = NETWORK_INITIALIZE_IP_ADDRESS(IP_Address_Byte_1, IP_Address_Byte_2, IP_Address_Byte_3, IP_Address_Byte_4)
#endif

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
	unsigned int Destination_IP_Address; //!< The IP address of the target machine (when transmitting), used also to check the source of a received packet.
	unsigned short Source_Port; //!< When transmitting, indicate the layer 4 port from where data were transmitted. When receiving, allow to match the received packet destination port with it to be sure that the packet is destined to this socket.
	unsigned short Destination_Port; //!< Indicate the target machine port on transmission.
	TNetworkIPProtocol IP_Protocol; //<! The type of the data embedded in the IP packet.
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
	unsigned short Flags_And_Fragment_Offset; //!< Not needed by this network stack.
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
 * @return 2 in non-blocking mode only to tell that no packet was received.
 */
int NetworkUDPReceiveBuffer(TNetworkSocket *Pointer_Socket, int Is_Call_Blocking, unsigned int *Pointer_Buffer_Size, void *Pointer_Buffer);

#endif