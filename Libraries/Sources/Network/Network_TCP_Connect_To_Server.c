/** @file Network_TCP_Connect_To_Server.c
 * @author Adrien RICCIARDI
 */
#include <Network_Base.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int NetworkTCPConnectToServer(TNetworkSocket *Pointer_Socket)
{
	unsigned char Packet_Buffer[NETWORK_MAXIMUM_PACKET_SIZE];
	TNetworkTCPHeader *Pointer_TCP_Header = (TNetworkTCPHeader *) (Packet_Buffer + sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header));
	unsigned int Timeout_Milliseconds = 5000, Data_Size, New_Sequence_Number, New_Acknowledgement_Number;
	int Result, Is_Packet_Received = 0;
	
	// Make sure the socket is considered not connected until it is really connected
	Pointer_Socket->Is_TCP_Connection_Established = 0;
	
	// Initiate the three-way handshake with the server
	// Prepare a SYN packet
	NetworkBaseTCPPrepareHeader(Pointer_Socket, NETWORK_TCP_FLAG_SYN, Pointer_TCP_Header);
	Pointer_TCP_Header->Sequence_Number = RandomGenerateNumber(); // This value must always vary between two connections or the server will mark the connection as "TCP port reused" and nothing will work
	Pointer_TCP_Header->Acknowledgment_Number = 0;
	// Compute checksum
	Pointer_TCP_Header->Checksum = NetworkBaseTCPComputeChecksum(Pointer_Socket, Pointer_TCP_Header, sizeof(TNetworkTCPHeader));
	if (NetworkBaseIPSendPacket(Pointer_Socket, sizeof(TNetworkTCPHeader), Packet_Buffer) != 0) return 1;
	
	// Wait for the SYN-ACK packet sent by the server
	Timeout_Milliseconds += SystemGetTimerValue();
	while (SystemGetTimerValue() <= Timeout_Milliseconds)
	{
		// Check if a packet is available
		Result = NetworkBaseIPReceivePacket(Pointer_Socket, 0, &Data_Size, Packet_Buffer);
		if (Result == 1) return 1; // An error occurred
		
		// A packet was received
		if (Result == 0)
		{
			// Make sure that the destination port is the good one
			if (Pointer_TCP_Header->Destination_Port != Pointer_Socket->Source_Port) continue; // Wait for another packet
			
			// Make sure both SYN and ACK flags are set
			if (!(Pointer_TCP_Header->Header_Size_And_Flags & NETWORK_SWAP_WORD(NETWORK_TCP_FLAG_SYN)) || !(Pointer_TCP_Header->Header_Size_And_Flags & NETWORK_SWAP_WORD(NETWORK_TCP_FLAG_ACK))) continue;
			
			Is_Packet_Received = 1;
			break;
		}
	}
	if (!Is_Packet_Received) return 2;
	
	// Send the ACK packet to the server
	// Prepare packet
	NetworkBaseTCPPrepareHeader(Pointer_Socket, NETWORK_TCP_FLAG_ACK, Pointer_TCP_Header);
	New_Sequence_Number = NETWORK_SWAP_DOUBLE_WORD(Pointer_TCP_Header->Acknowledgment_Number);
	New_Acknowledgement_Number = NETWORK_SWAP_DOUBLE_WORD(Pointer_TCP_Header->Sequence_Number) + 1;
	Pointer_TCP_Header->Sequence_Number = NETWORK_SWAP_DOUBLE_WORD(New_Sequence_Number);
	Pointer_TCP_Header->Acknowledgment_Number = NETWORK_SWAP_DOUBLE_WORD(New_Acknowledgement_Number);
	// Compute checksum
	Pointer_TCP_Header->Checksum = NetworkBaseTCPComputeChecksum(Pointer_Socket, Pointer_TCP_Header, sizeof(TNetworkTCPHeader));
	if (NetworkBaseIPSendPacket(Pointer_Socket, sizeof(TNetworkTCPHeader), Packet_Buffer) != 0) return 1;
	
	// Initialize socket TCP-specific fields
	Pointer_Socket->TCP_Sequence_Number = New_Sequence_Number;
	Pointer_Socket->TCP_Acknowledgement_Number = New_Acknowledgement_Number;
	Pointer_Socket->Is_TCP_Connection_Established = 1;
	
	return 0;
}