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

/** TODO */
int NetworkBaseIPSendPacket(TNetworkSocket *Pointer_Socket, unsigned int Payload_Size, unsigned char *Pointer_Payload_Buffer);

#endif