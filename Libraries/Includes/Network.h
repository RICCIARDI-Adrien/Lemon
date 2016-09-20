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
#define NETWORK_MAKE_IP_ADDRESS(Byte_1, Byte_2, Byte_3, Byte_4) (((unsigned char) Byte_4 << 24) | ((unsigned char) Byte_3 << 16) | ((unsigned char) Byte_2 << 8) | (unsigned char) Byte_1)

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
	unsigned char Destination_MAC_Address[NETWORK_MAC_ADDRESS_SIZE];
	unsigned int Destination_IP_Address;
	unsigned short Source_Port;
	unsigned short Destination_Port;
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
 * @param Pointer_System_IP_Address If set to NULL, use DHCP to get an IP address. If set to a valid IP address, use this address to configure the network interface.
 * @param Pointer_Gateway_IP_Address If set to NULL, use DHCP to get the default gateway address. If set to a valid IP address, use this address to configure the network interface.
 * @return 0 if the network subsystem was successfully initialized,
 * @return TODO others
 */
int NetworkInitialize(TNetworkIPAddress *Pointer_System_IP_Address, TNetworkIPAddress *Pointer_Gateway_IP_Address);

/** TODO */
int NetworkInitializeReceptionSocket(TNetworkIPAddress *Pointer_Source_IP_Address, unsigned short Destination_Port, TNetworkIPProtocol Used_Protocol, TNetworkSocket *Pointer_Socket);

/** Initialize a socket content to transmit data to the specified IP address.
 * @param Pointer_Destination_IP_Address The data recipient.
 * @param Destination_Port The IP encapsulated protocol destination port (this value is automatically swapped to big endian).
 * @param Used_Protocol Which protocol is encapsulated in the IP packet.
 * @param Pointer_Socket On output, contain the initialized socket.
 * @return 0 if the socket was successfully initialized,
 * @return 1 if the MAC address corresponding to the destination address could not be retrieved.
 */
int NetworkInitializeTransmissionSocket(TNetworkIPAddress *Pointer_Destination_IP_Address, unsigned short Destination_Port, TNetworkIPProtocol Used_Protocol, TNetworkSocket *Pointer_Socket);

/** TODO */
int NetworkIPConvertFromString(char *String_IP_Address, char *String_Subnet_Mask, TNetworkIPAddress *Pointer_IP_Address);

/** TODO */
int NetworkUDPSendBuffer(TNetworkSocket *Pointer_Socket, unsigned int Buffer_Size, void *Pointer_Buffer);

/** TODO */
int NetworkUDPReceiveBuffer(TNetworkSocket *Pointer_Socket, int Is_Call_Blocking, unsigned int *Pointer_Buffer_Size, void *Pointer_Buffer);

#endif