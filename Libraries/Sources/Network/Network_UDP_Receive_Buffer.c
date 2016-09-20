/** @file Network_UDP_Receive_Buffer.c
 * @author Adrien RICCIARDI
 */
#include <Network_Base.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public function
//-------------------------------------------------------------------------------------------------
int NetworkUDPReceiveBuffer(TNetworkSocket *Pointer_Socket, int Is_Call_Blocking, unsigned int *Pointer_Buffer_Size, void *Pointer_Buffer)
{
	unsigned char Packet_Buffer[NETWORK_MAXIMUM_PACKET_SIZE];
	TNetworkUDPHeader *Pointer_UDP_Header = (TNetworkUDPHeader *) (Packet_Buffer + sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header));
	unsigned int Data_Size;
	
	do
	{
		if (NetworkBaseIPReceivePacket(Pointer_Socket, &Data_Size, Packet_Buffer) != 0) return 1;
	} while (Pointer_UDP_Header->Destination_Port != Pointer_Socket->Destination_Port);
	
	// Adjust buffer size to UDP data only
	Data_Size = NETWORK_SWAP_WORD(Pointer_UDP_Header->Length) - sizeof(TNetworkUDPHeader);
	
	// Extract the packet content
	MemoryCopyArea(Packet_Buffer + sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header) + sizeof(TNetworkUDPHeader), Pointer_Buffer, Data_Size);
	*Pointer_Buffer_Size = Data_Size;
		
	return 0;
}