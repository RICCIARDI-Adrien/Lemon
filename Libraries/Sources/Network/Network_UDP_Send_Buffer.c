/** @file Network_UDP_Send_Buffer.c
 * @author Adrien RICCIARDI
 */
#include <Network_Base.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public function
//-------------------------------------------------------------------------------------------------
int NetworkUDPSendBuffer(TNetworkSocket *Pointer_Socket, unsigned int Buffer_Size, void *Pointer_Buffer)
{
	unsigned char Packet_Buffer[NETWORK_MAXIMUM_PACKET_SIZE];
	TNetworkUDPHeader *Pointer_UDP_Header;
	unsigned int IP_Payload_Size;
	
	// Discard the packet if it is too big
	if (sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header) + sizeof(TNetworkUDPHeader) + Buffer_Size > NETWORK_MAXIMUM_PACKET_SIZE) return 1;
	
	// Prepare UDP header
	Pointer_UDP_Header = (TNetworkUDPHeader *) (Packet_Buffer + sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header)); // Let space for preceding headers to avoid memory copies in the downer stack level
	Pointer_UDP_Header->Source_Port = Pointer_Socket->Source_Port;
	Pointer_UDP_Header->Destination_Port = Pointer_Socket->Destination_Port;
	IP_Payload_Size = sizeof(TNetworkUDPHeader) + Buffer_Size;
	Pointer_UDP_Header->Length = NETWORK_SWAP_WORD(IP_Payload_Size);
	Pointer_UDP_Header->Checksum = 0; // Disable checksum validation
	
	// Copy UDP payload
	SystemMemoryCopyArea(Pointer_Buffer, Packet_Buffer + sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header) + sizeof(TNetworkUDPHeader), Buffer_Size);
	
	// Send the packet
	return NetworkBaseIPSendPacket(Pointer_Socket, IP_Payload_Size, Packet_Buffer);
}
