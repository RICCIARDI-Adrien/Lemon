/** @file Test_Network_Ethernet_Reception.c
 * Display an hexadecimal dump of all received packets.
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	unsigned int Packet_Size, i;
	unsigned char Packet_Buffer[NETWORK_MAXIMUM_PACKET_SIZE];
	
	SystemScreenWriteString("Hit F12 to exit.\n");
	
	while (1)
	{
		// Wait for a packet to be received
		LibrariesSystemCall(SYSTEM_CALL_ETHERNET_RECEIVE_PACKET, 0, 0, &Packet_Size, Packet_Buffer);
		
		// Display initial bytes
		if (Packet_Size > 64) Packet_Size = 64;
		for (i = 0; i < Packet_Size; i++)
		{
			SystemScreenWriteUnsignedInteger(Packet_Buffer[i]);
			SystemScreenWriteCharacter(' ');
		}
		SystemScreenWriteCharacter('\n');
	}
}