/** @file Network_Initialize.c
 * @author Adrien RICCIARDI
 */
#include <Network_Base.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int NetworkInitialize(TNetworkIPAddress *Pointer_System_IP_Address, TNetworkIPAddress *Pointer_Gateway_IP_Address)
{
	int Is_System_IP_Address_Set = 0, Is_Gateway_IP_Address_Set = 0;
	
	NetworkBaseInitialize(Pointer_System_IP_Address, Pointer_Gateway_IP_Address);
	
	// TODO gw
	
	// Start DHCP protocol if one or both addresses are not set
	/*if (!Is_System_IP_Address_Set || !Is_Gateway_IP_Address_Set)
	{
		if (!Is_System_IP_Address_Set)*/
		
		
	
	
	return 0;
}
