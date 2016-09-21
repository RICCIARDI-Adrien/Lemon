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
	int Return_Value;
	
	// Wait for a packet (if the call is blocking) or immediately exit (if the call is not blocking)
	do
	{
		Return_Value = NetworkBaseIPReceivePacket(Pointer_Socket, Is_Call_Blocking, &Data_Size, Packet_Buffer);
		if (Return_Value) return Return_Value; // The function returns 0 in both blocking and non-blocking modes if a packet has been received
	} while ((Is_Call_Blocking) && (Pointer_UDP_Header->Destination_Port != Pointer_Socket->Source_Port)); // In blocking mode only, wait for a matching destination port
	
	// In non-blocking mode, make sure that the destination port is the good one
	if ((!Is_Call_Blocking) && (Pointer_UDP_Header->Destination_Port != Pointer_Socket->Source_Port)) return 2;
	
	// Adjust buffer size to UDP data only
	Data_Size = NETWORK_SWAP_WORD(Pointer_UDP_Header->Length) - sizeof(TNetworkUDPHeader);
	
	// Extract the packet content
	MemoryCopyArea(Packet_Buffer + sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header) + sizeof(TNetworkUDPHeader), Pointer_Buffer, Data_Size);
	*Pointer_Buffer_Size = Data_Size;

	return 0;
}