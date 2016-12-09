/** @file Network_Base.h
 * Contain all hidden network operations needed to make a network connection work.
 * @author Adrien RICCIARDI
 */
#ifndef H_NETWORK_BASE_H
#define H_NETWORK_BASE_H

#include <Network.h>

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How many time to wait before considering a packet reception failed (in milliseconds). */
#define NETWORK_BASE_TCP_RECEPTION_TIMEOUT_MILLISECONDS 5000

//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
/** The system IP address. */
TNetworkIPAddress Network_Base_System_IP_Address;
/** The gateway IP address. */
TNetworkIPAddress Network_Base_Gateway_IP_Address;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the low-level part of the network stack (clear the ARP table, flush the network controller buffers...).
 * @param Pointer_System_IP_Address The IP address attributed to the system.
 * @param Pointer_Gateway_IP_Address The IP address used to communicate with a non-local network.
 * @return 0 if the operation was successful.
 */
int NetworkBaseInitialize(TNetworkIPAddress *Pointer_System_IP_Address, TNetworkIPAddress *Pointer_Gateway_IP_Address);

/** Retrieve the MAC address corresponding to the specified IP address. Perform an ARP request if the MAC address is not present in the internal cache.
 * @param Pointer_IP_Address The IP address to find the corresponding MAC address.
 * @param Pointer_MAC_Address On output, contain the retrieved MAC address.
 * @return 0 if the MAC address was successfully retrieved,
 * @return 1 if an error occurred (the recipient did not answer).
 */
int NetworkBaseGetMACAddressFromARPTable(TNetworkIPAddress *Pointer_IP_Address, unsigned char *Pointer_MAC_Address);

/** Retrieve a packet from the network controller.
 * @param Pointer_Socket The socket containing all useful connection parameters.
 * @param Is_Call_Blocking Set to 1 to wait infinitely for a packet to be received, set to 0 to return instantly, telling if a packet was retrieved or not.
 * @param Pointer_Packet_Size On output (and if a packet is available when using non-blocking mode), contain the received packet size in bytes.
 * @param Pointer_Packet_Buffer On output (and if a packet is available when using non-blocking mode), contain the received packet data.
 * @return 0 if a packet was received (in both blocking an non-blocking modes),
 * @return 1 if an error occurred,
 * @return 2 if no packet was available for a non-blocking call.
 */
int NetworkBaseIPReceivePacket(TNetworkSocket *Pointer_Socket, int Is_Call_Blocking, unsigned int *Pointer_Packet_Size, void *Pointer_Packet_Buffer);

/** Encapsulate the provided payload in an IPv4 packet and transmit it.
 * @param Pointer_Socket The socket containing all useful connection parameters.
 * @param Payload_Size The IP packet payload (starting from OSI layer 4) size in bytes.
 * @param Pointer_Packet_Buffer The buffer containing the layer 4 header yet initialized and its payload.
 * @return 0 if the packet was successfully transmitted,
 * @return 1 if the packet was not sent because it's size was too big.
 * @warning The packet buffer layer 4 header must let enough room to put an ethernet header and an IPv4 header before it. The layer 4 header must start at sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header) bytes offset.
 */
int NetworkBaseIPSendPacket(TNetworkSocket *Pointer_Socket, unsigned int Payload_Size, void *Pointer_Packet_Buffer);

/** Fill almost all fields of a TCP header (do not compute the checksum because data will be appended after).
 * @param Pointer_Socket The socket that will be used to transmit the prepared packet.
 * @param Flags The TCP flags to set into the header.
 * @param Pointer_TCP_Header On output, contain the filled TCP header.
 * @note Sequence number and acknowledgement number fields are not filled nor modified by this function.
 */
void NetworkBaseTCPPrepareHeader(TNetworkSocket *Pointer_Socket, unsigned short Flags, TNetworkTCPHeader *Pointer_TCP_Header);

/** Perform the one's complement checksum on the provided TCP header and payload.
 * @param Pointer_Socket The TCP socket used to transmit the packet.
 * @param Pointer_Data The TCP header followed by the TCP payload.
 * @param Bytes_Count The TCP header size plus the TCP payload size in bytes.
 * @return The checksum to put in the corresponding TCP header field.
 * @note The TCP checksum field must be set to zero prior to call this function.
 * @warning The TCP pseudo header needed to compute the checksum will be appended just before the Pointer_Data address.
 */
unsigned short NetworkBaseTCPComputeChecksum(TNetworkSocket *Pointer_Socket, void *Pointer_Data, unsigned int Bytes_Count);

/** Wait NETWORK_BASE_TCP_RECEPTION_TIMEOUT_MILLISECONDS milliseconds for a TCP packet to be received.
 * @param Pointer_Socket The socket used to transmit and receive data (the socket is not required to be connected).
 * @param Flags_To_Check If different from 0, the received TCP packet header flags must fit exactly the provided flags. If equal to 0, all flags configuration are accepted.
 * @param Pointer_Packet_Size On output, contain received packet full size in bytes (including lower layers).
 * @param Pointer_Packet_Buffer On output, contain the packet data (with lower layers too).
 * @return 0 if a packet was successfully received,
 * @return 1 if a reception error occurred,
 * @return 2 if the reception timed out.
 */
int NetworkBaseTCPReceivePacket(TNetworkSocket *Pointer_Socket, unsigned short Flags_To_Check, unsigned int *Pointer_Packet_Size, void *Pointer_Packet_Buffer);

#endif