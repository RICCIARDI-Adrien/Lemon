/** @file Test_Network_UDP_Transmission.c
 * Transmit as much as possible UDP datagrams.
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Maximum payload size for an UDP packet. */
#define MAXIMUM_PAYLOAD_SIZE (NETWORK_MAXIMUM_PACKET_SIZE - sizeof(TNetworkEthernetHeader) - sizeof(TNetworkIPv4Header) - sizeof(TNetworkUDPHeader))

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	TNetworkIPAddress IP_Address;
	unsigned short Port;
	TNetworkSocket Socket;
	unsigned int Payload_Size;
	unsigned char Payload_Buffer[MAXIMUM_PAYLOAD_SIZE];
	
	// Check parameters
	if (argc != 4)
	{
		ScreenWriteString("Usage : ");
		ScreenWriteString(argv[0]);
		ScreenWriteString(" IP_Address Port Payload_Size_Bytes\n");
		return 1;
	}
	
	// Retrieve parameters
	// IP address
	if (NetworkInitializeIPAddress(argv[1], &IP_Address) != 0)
	{
		ScreenWriteString("Error : malformed IP address.\n");
		return 1;
	}
	// Port number
	Port = StringConvertStringToUnsignedInteger(argv[2]);
	// Payload size
	Payload_Size = StringConvertStringToUnsignedInteger(argv[3]);
	if (Payload_Size > MAXIMUM_PAYLOAD_SIZE)
	{
		ScreenWriteString("Error : maximum allowed payload size : ");
		ScreenWriteUnsignedInteger(MAXIMUM_PAYLOAD_SIZE);
		ScreenWriteString(".\n");
		return 1;
	}
	
	// Initialize network stack
	if (NetworkInitialize() != 0)
	{
		ScreenWriteString("Error : failed to initialize network stack.\n");
		return 1;
	}
	
	// Create the socket used to transmit the datagrams
	if (NetworkInitializeSocket(&IP_Address, Port, NETWORK_IP_PROTOCOL_UDP, &Socket) != 0)
	{
		ScreenWriteString("Error : failed to initialize socket.\n");
		return 1;
	}
	
	// Fill the payload with a recognizable pattern
	SystemMemorySetAreaValue(Payload_Buffer, Payload_Size, 0xCA);
	
	// Transmit forever
	ScreenWriteString("Transmitting at maximum speed. Hit F12 to quit.\n");
	while (1)
	{
		if (NetworkUDPSendBuffer(&Socket, Payload_Size, Payload_Buffer) != 0)
		{
			ScreenWriteString("Error : failed to transmit a packet.\n");
			return 1;
		}
	}
}