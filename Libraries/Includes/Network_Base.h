/** @file Network_Base.h
 * Contain all hidden network operations needed to make a network connection work.
 * @author Adrien RICCIARDI
 */
#ifndef H_NETWORK_BASE_H
#define H_NETWORK_BASE_H

#include <Network.h>

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** TODO */
int NetworkBaseInitialize(TNetworkIPAddress *Pointer_System_IP_Address, TNetworkIPAddress *Pointer_Gateway_IP_Address);

#endif