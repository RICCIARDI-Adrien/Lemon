/** @file Network_TCP_Send_Buffer.c
 * @author Adrien RICCIARDI
 */
#include <Network_Base.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int NetworkTCPSendBuffer(TNetworkSocket *Pointer_Socket, unsigned int Buffer_Size, void *Pointer_Buffer)
{
	unsigned char Packet_Buffer[NETWORK_MAXIMUM_PACKET_SIZE];
	TNetworkTCPHeader *Pointer_TCP_Header = (TNetworkTCPHeader *) (Packet_Buffer + sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header));
	unsigned int TCP_Packet_Size, Timeout_Milliseconds = 2000;
	int Result, Is_Packet_Received = 0;
	
	// Make sure the buffer content can be sent in a single packet
	if (Buffer_Size > NETWORK_MAXIMUM_PACKET_SIZE - sizeof(TNetworkEthernetHeader) - sizeof(TNetworkIPv4Header) - sizeof(TNetworkTCPHeader)) return 2;
	
	// Make sure the connection is established
	if (!Pointer_Socket->Is_TCP_Connection_Established) return 3;
	
	// Fill the TCP header
	NetworkBaseTCPPrepareHeader(Pointer_Socket, NETWORK_TCP_FLAG_ACK | NETWORK_TCP_FLAG_PUSH, Pointer_TCP_Header); // Set the PUSH flag to force the recipient to flush its buffer and to send an ACK
	Pointer_TCP_Header->Sequence_Number = NETWORK_SWAP_DOUBLE_WORD(Pointer_Socket->TCP_Sequence_Number);
	Pointer_TCP_Header->Acknowledgment_Number = NETWORK_SWAP_DOUBLE_WORD(Pointer_Socket->TCP_Acknowledgement_Number);
	// Append TCP data
	MemoryCopyArea(Pointer_Buffer, Packet_Buffer + sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header) + sizeof(TNetworkTCPHeader), Buffer_Size);
	// Compute the TCP checksum now that the TCP packet is complete
	TCP_Packet_Size = sizeof(TNetworkTCPHeader) + Buffer_Size;
	Pointer_TCP_Header->Checksum = NetworkBaseTCPComputeChecksum(Pointer_Socket, Pointer_TCP_Header, TCP_Packet_Size);
	
	// Send the packet
	if (NetworkBaseIPSendPacket(Pointer_Socket, TCP_Packet_Size, Packet_Buffer) != 0) return 1;
	
	// Wait for the acknowledge
	Timeout_Milliseconds += SystemGetTimerValue();
	while (SystemGetTimerValue() <= Timeout_Milliseconds)
	{
		// Check if a packet is available
		Result = NetworkBaseIPReceivePacket(Pointer_Socket, 0, &TCP_Packet_Size, Packet_Buffer);
		if (Result == 1) return 1; // An error occurred
		
		// A packet was received
		if (Result == 0)
		{
			// Make sure that the destination port is the good one
			if (Pointer_TCP_Header->Destination_Port != Pointer_Socket->Source_Port) continue; // Wait for another packet
			
			// Make sure ACK flags is set
			if (!(Pointer_TCP_Header->Header_Size_And_Flags & NETWORK_SWAP_WORD(NETWORK_TCP_FLAG_ACK))) continue;
			
			Is_Packet_Received = 1;
			break;
		}
	}
	if (!Is_Packet_Received) return 4;
	
	// Update sequence number with the received acknowledgement number
	Pointer_Socket->TCP_Sequence_Number = NETWORK_SWAP_DOUBLE_WORD(Pointer_TCP_Header->Acknowledgment_Number);
	
	return 0;
}