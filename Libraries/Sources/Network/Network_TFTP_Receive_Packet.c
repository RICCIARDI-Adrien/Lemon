/** @file Network_TFTP_Receive_Packet.c
 * @author Adrien RICCIARDI
 */
#include <Network_Base.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int NetworkTFTPReceivePacket(TNetworkSocket *Pointer_Socket, unsigned int Timeout_Milliseconds, unsigned int *Pointer_Packet_Size, TNetworkTFTPPacket *Pointer_Packet)
{
	unsigned char Packet_Buffer[NETWORK_MAXIMUM_PACKET_SIZE];
	unsigned int Data_Size;
	int Result;
	TNetworkUDPHeader *Pointer_UDP_Header = (TNetworkUDPHeader *) (Packet_Buffer + sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header));
	
	// Adjust timeout to the maximum expected value
	Timeout_Milliseconds += SystemGetTimerValue();
	
	// Wait for a packet to be received
	while (SystemGetTimerValue() <= Timeout_Milliseconds)
	{
		// Check if a packet is available
		Result = NetworkBaseIPReceivePacket(Pointer_Socket, 0, &Data_Size, Packet_Buffer);
		if (Result == 1) return 1; // An error occurred
		
		// A packet was received
		if (Result == 0)
		{
			// Make sure that the destination port is the good one
			if (Pointer_UDP_Header->Destination_Port != Pointer_Socket->Source_Port) continue; // Wait for another packet
			
			// Automatically change the server source port to fit the TFTP protocol
			if (Pointer_UDP_Header->Source_Port != Pointer_Socket->Destination_Port) Pointer_Socket->Destination_Port = Pointer_UDP_Header->Source_Port;
			
			// Adjust buffer size to UDP data only to get the raw TFTP packet
			Data_Size = NETWORK_SWAP_WORD(Pointer_UDP_Header->Length) - sizeof(TNetworkUDPHeader);
			if (Data_Size > sizeof(TNetworkTFTPPacket)) continue;
			
			// Extract the packet content
			SystemMemoryCopyArea(Packet_Buffer + sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header) + sizeof(TNetworkUDPHeader), Pointer_Packet, Data_Size);
			*Pointer_Packet_Size = Data_Size;
			
			return 0;
		}
	}
	
	return 2; // Timeout
}