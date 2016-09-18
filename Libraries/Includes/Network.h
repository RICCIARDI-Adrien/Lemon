/** @file Network.h
 * Provide a TCP and UDP network stack able to communicate with the internet.
 * @author Adrien RICCIARDI
 */
#ifndef H_NETWORK_H
#define H_NETWORK_H

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
/** A packet buffer maximum length in bytes. */
#define NETWORK_PACKET_BUFFER_MAXIMUM_SIZE 2048

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

// TNetworkIPProtocol (tcp ou udp)

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

/** TODO
 * dire swap valeurs
 */
//void NetworkUDPReceiveDatagram(unsigned int Listening_IP_Address, unsigned short Listening_Port, unsigned int *Pointer_Data_Size, void *Pointer_Buffer);

//int NetworkUDPReceiveBuffer(port, siz, buf)
//int NetworkUDPSendBuffer(IP, port, siz, buf)

#endif