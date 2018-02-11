/** @file Network_Initialize.c
 * @author Adrien RICCIARDI
 */
#include <Network_Base.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int NetworkInitialize(void)
{
	TNetworkIPAddress System_IP_Address, Gateway_IP_Address;
	int Is_Network_Controller_Enabled;
	char String_Configuration_Value[SYSTEM_CONFIGURATION_FILE_MAXIMUM_VALUE_SIZE + 1]; // +1 for the terminating zero
	
	// Is the ethernet controller available ?
	LibrariesSystemCall(SYSTEM_CALL_SYSTEM_GET_PARAMETER, SYSTEM_CALL_SYSTEM_PARAMETER_ID_ETHERNET_CONTROLLER_IS_ENABLED, 0, &Is_Network_Controller_Enabled, NULL);
	if (!Is_Network_Controller_Enabled) return 1;
	
	// Get the IP addresses from the configuration file
	// System IP address
	if (SystemConfigurationReadValue("Network_System_IP_Address", String_Configuration_Value) != 0) return 2;
	if (NetworkIPConvertFromString(String_Configuration_Value, &System_IP_Address.Address) != 0) return 2;
	// System subnet mask
	if (SystemConfigurationReadValue("Network_System_Subnet_Mask", String_Configuration_Value) != 0) return 2;
	if (NetworkIPConvertFromString(String_Configuration_Value, &System_IP_Address.Mask) != 0) return 2;
	// Gateway IP address
	if (SystemConfigurationReadValue("Network_Gateway_IP_Address", String_Configuration_Value) != 0) return 2;
	if (NetworkIPConvertFromString(String_Configuration_Value, &Gateway_IP_Address.Address) != 0) return 2;
	// Gateway subnet mask
	if (SystemConfigurationReadValue("Network_Gateway_Subnet_Mask", String_Configuration_Value) != 0) return 2;
	if (NetworkIPConvertFromString(String_Configuration_Value, &Gateway_IP_Address.Mask) != 0) return 2;
	
	// Initialize the network stack
	NetworkBaseInitialize(&System_IP_Address, &Gateway_IP_Address);
	
	// TODO : DHCP protocol if requested to
	
	return 0;
}
