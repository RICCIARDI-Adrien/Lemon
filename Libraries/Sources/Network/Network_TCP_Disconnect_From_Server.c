/** @file Network_TCP_Disconnect_From_Server.c
 * @author Adrien RICCIARDI
 */
#include <Network_Base.h>
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int NetworkTCPDisconnectFromServer(TNetworkSocket *Pointer_Socket)
{
	unsigned char Packet_Buffer[NETWORK_MAXIMUM_PACKET_SIZE];
	TNetworkTCPHeader *Pointer_TCP_Header = (TNetworkTCPHeader *) (Packet_Buffer + sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header));
	unsigned int TCP_Packet_Size;
	int Result;
	
	if (Pointer_Socket->Is_TCP_Connection_Established)
	{
		// Prepare a FIN + ACK packet
		NetworkBaseTCPPrepareHeader(Pointer_Socket, NETWORK_TCP_FLAG_ACK | NETWORK_TCP_FLAG_FIN, Pointer_TCP_Header);
		Pointer_TCP_Header->Sequence_Number = NETWORK_SWAP_DOUBLE_WORD(Pointer_Socket->TCP_Sequence_Number);
		Pointer_TCP_Header->Acknowledgment_Number = NETWORK_SWAP_DOUBLE_WORD(Pointer_Socket->TCP_Acknowledgement_Number);
		Pointer_TCP_Header->Checksum = NetworkBaseTCPComputeChecksum(Pointer_Socket, Pointer_TCP_Header, sizeof(TNetworkTCPHeader));
		// Send it
		if (NetworkBaseIPSendPacket(Pointer_Socket, sizeof(TNetworkTCPHeader), Packet_Buffer) != 0) return 1;
		
		// Wait for the same packet sent from the other side
		Result = NetworkBaseTCPReceivePacket(Pointer_Socket, NETWORK_TCP_FLAG_FIN | NETWORK_TCP_FLAG_ACK, &TCP_Packet_Size, Packet_Buffer);
		if (Result != 0) return Result;
		
		// Update sequence and acknowledgement numbers
		Pointer_Socket->TCP_Sequence_Number = NETWORK_SWAP_DOUBLE_WORD(Pointer_TCP_Header->Acknowledgment_Number);
		Pointer_Socket->TCP_Acknowledgement_Number = NETWORK_SWAP_DOUBLE_WORD(Pointer_TCP_Header->Sequence_Number) + 1;
		
		// Transmit the last ACK
		// Prepare the packet
		NetworkBaseTCPPrepareHeader(Pointer_Socket, NETWORK_TCP_FLAG_ACK, Pointer_TCP_Header);
		Pointer_TCP_Header->Sequence_Number = NETWORK_SWAP_DOUBLE_WORD(Pointer_Socket->TCP_Sequence_Number);
		Pointer_TCP_Header->Acknowledgment_Number = NETWORK_SWAP_DOUBLE_WORD(Pointer_Socket->TCP_Acknowledgement_Number);
		Pointer_TCP_Header->Checksum = NetworkBaseTCPComputeChecksum(Pointer_Socket, Pointer_TCP_Header, sizeof(TNetworkTCPHeader));
		// Send it
		if (NetworkBaseIPSendPacket(Pointer_Socket, sizeof(TNetworkTCPHeader), Packet_Buffer) != 0) return 1;
		
		Pointer_Socket->Is_TCP_Connection_Established = 0;
	}
	// A FIN packet was previously received by sending or receiving functions, which set the connection established flag to false
	else
	{
		// TODO
		// Prepare a FIN + ACK packet
		//NetworkBaseTCPPrepareHeader(Pointer_Socket, 
	}
	
	return 0;
}