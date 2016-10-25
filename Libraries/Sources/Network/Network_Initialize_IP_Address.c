/** @file Network_Initialize_IP_Address.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int NetworkInitializeIPAddress(char *String_IP_Address, TNetworkIPAddress *Pointer_IP_Address)
{
	int Return_Value;
	
	// Convert the IP address
	Return_Value = NetworkIPConvertFromString(String_IP_Address, &Pointer_IP_Address->Address);
	if (Return_Value != 0) return Return_Value;
	
	// Set the default network mask
	Pointer_IP_Address->Mask = NETWORK_GET_SUBNET_MASK_FROM_IP_ADDRESS(Pointer_IP_Address->Address); // The address has been swapped to big endian by NetworkIPConvertFromString()
	
	return 0;
}