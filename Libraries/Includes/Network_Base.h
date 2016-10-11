/** @file Network_Base.h
 * Contain all hidden network operations needed to make a network connection work.
 * @author Adrien RICCIARDI
 */
#ifndef H_NETWORK_BASE_H
#define H_NETWORK_BASE_H

#include <Network.h>

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
/** TODO */
int NetworkBaseInitialize(TNetworkIPAddress *Pointer_System_IP_Address, TNetworkIPAddress *Pointer_Gateway_IP_Address);

/** TODO */
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
int NetworkBaseIPSendPacket(TNetworkSocket *Pointer_Socket, unsigned int Payload_Size, unsigned char *Pointer_Packet_Buffer); // TODO void

#endif