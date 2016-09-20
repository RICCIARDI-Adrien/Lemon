/** @file Network_Initialize_Reception_Socket.c
 * @author Adrien RICCIARDI
 */
#include <Network_Base.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int NetworkInitializeReceptionSocket(TNetworkIPAddress *Pointer_Source_IP_Address, unsigned short Destination_Port, TNetworkIPProtocol Used_Protocol, TNetworkSocket *Pointer_Socket)
{
	Pointer_Socket->Destination_Port = NETWORK_SWAP_WORD(Destination_Port);
	Pointer_Socket->IP_Protocol = Used_Protocol;
	
	return 0;
}