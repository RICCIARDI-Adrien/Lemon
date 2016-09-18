/** @file Network_Initialize_Transmission_Socket.c
 * @author Adrien RICCIARDI
 */
#include <Network_Base.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int NetworkInitializeTransmissionSocket(TNetworkIPAddress *Pointer_Destination_IP_Address, unsigned short Destination_Port, TNetworkIPProtocol Used_Protocol, TNetworkSocket *Pointer_Socket)
{
	TNetworkIPAddress *Pointer_ARP_Resolution_IP_Address;
	
	// Set the destination IP address
	Pointer_Socket->Destination_IP_Address = Pointer_Destination_IP_Address->Address;
	
	// Is the IP address in the local area network ?
	if ((Pointer_Destination_IP_Address->Address & Pointer_Destination_IP_Address->Mask) == (Network_Base_System_IP_Address.Address & Network_Base_System_IP_Address.Mask)) Pointer_ARP_Resolution_IP_Address = Pointer_Destination_IP_Address; // Yes, determine the MAC address of this IP
	else Pointer_ARP_Resolution_IP_Address = &Network_Base_Gateway_IP_Address; // No, transmit the packet to the gateway, it will know how to route it
	// Get the destination equipment MAC address
	if (NetworkBaseGetMACAddressFromARPTable(Pointer_ARP_Resolution_IP_Address, Pointer_Socket->Destination_MAC_Address) != 0) return 1;
	
	// Set remaining useful fields
	Pointer_Socket->Destination_Port = NETWORK_SWAP_WORD(Destination_Port);
	Pointer_Socket->IP_Protocol = (unsigned char) Used_Protocol;
	
	return 0;
}