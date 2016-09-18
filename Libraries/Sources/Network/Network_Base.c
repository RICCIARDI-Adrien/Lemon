/** @file Network_Base.c
 * @see Network_Base.h for description.
 * @author Adrien RICCIARDI
 */
#include <Network.h>
#include <Network_Base.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Tell that the ethernet frame contains an IP packet. */
#define NETWORK_BASE_ETHERNET_TYPE_IP NETWORK_SWAP_WORD(0x0800)
/** Tell that the ethernet frame contains an ARP packet. */
#define NETWORK_BASE_ETHERNET_TYPE_ARP NETWORK_SWAP_WORD(0x0806)

/** The hardware address space corresponding to ethernet. */
#define NETWORK_BASE_ARP_HARDWARE_ADDRESS_SPACE_ETHERNET NETWORK_SWAP_WORD(0x0001)
/** The protocol address space corresponding to IP. */
#define NETWORK_BASE_ARP_PROTOCOL_ADDRESS_SPACE_IP NETWORK_SWAP_WORD(0x0800)

/** An ARP request opcode value. */
#define NETWORK_BASE_ARP_OPCODE_REQUEST NETWORK_SWAP_WORD(0x0001)
/** An ARP reply opcode value. */
#define NETWORK_BASE_ARP_OPCODE_REPLY NETWORK_SWAP_WORD(0x0002)

/** The length in bytes of a MAC address. */
#define NETWORK_BASE_MAC_ADDRESS_SIZE 6
/** The length in bytes of an IPv4 address. */
#define NETWORK_BASE_IP_ADDRESS_SIZE sizeof(int)

/** The ARP table entries count. */
#define NETWORK_BASE_ARP_TABLE_SIZE 16

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** An ethernet header. */
typedef struct __attribute__((packed))
{
	unsigned char Destination_MAC_Address[NETWORK_BASE_MAC_ADDRESS_SIZE]; //!< The receiver address.
	unsigned char Source_MAC_Address[NETWORK_BASE_MAC_ADDRESS_SIZE]; //!< The sender address.
	unsigned short Protocol_Type; //!< What protocol is encapsulated in the ethernet frame.
} TNetworkBaseEthernetHeader;

/** An ARP protocol payload (ethernet network version). */
typedef struct __attribute__((packed))
{
	unsigned short Hardware_Address_Space; //!< When issuing a request, this is the type of the data we want to retrieve.
	unsigned short Protocol_Address_Space; //!< When issuing a request, this is the type of data we transmit.
	unsigned char Hardware_Address_Size; //!< The size in bytes of the hardware address.
	unsigned char Protocol_Address_Size; //!< The size in bytes of the protocol address.
	unsigned short Opcode; //!< Tell if it is a request or a reply.
	unsigned char Sender_Hardware_Address[NETWORK_BASE_MAC_ADDRESS_SIZE]; //!< The sender MAC address.
	unsigned int Sender_Protocol_Address; //!< The sender IPv4 address;
	unsigned char Target_Harware_Address[NETWORK_BASE_MAC_ADDRESS_SIZE]; //!< The destination MAC address.
	unsigned int Target_Protocol_Address; //!< The destination IPv4 address;
} TNetworkBaseARPPayload;

/** An ARP table entry (does the IP to MAC matching). */
typedef struct
{
	unsigned int IP_Address; //!< The IPv4 address (no need for subnet mask here).
	unsigned char MAC_Address[NETWORK_BASE_MAC_ADDRESS_SIZE]; //!< The MAC address corresponding to the IP address.
	int Is_Entry_Free; //!< Tell if this record contains valid data or not.
} TNetworkBaseARPTableEntry;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
/** The system MAC address. */
static unsigned char Network_Base_System_MAC_Address[NETWORK_BASE_MAC_ADDRESS_SIZE];
/** The system IP address. */
static TNetworkIPAddress Network_Base_System_IP_Address;

/** The gateway IP address. */
static TNetworkIPAddress Network_Base_Gateway_IP_Address;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The ARP table. */
static TNetworkBaseARPTableEntry Network_Base_ARP_Table_Entries[NETWORK_BASE_ARP_TABLE_SIZE];
/** The ARP table current size (how many entries are occupied). */
static int Network_Base_ARP_Table_Used_Entries_Count;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Wait for an ethernet frame to be received.
 * @param Pointer_Frame_Size On output, contain the frame size in bytes.
 * @param Pointer_Buffer On output, contain the frame data.
 */
static inline void NetworkBaseEthernetReceivePacket(unsigned int *Pointer_Packet_Size, void *Pointer_Buffer)
{
	SystemCall(SYSTEM_CALL_ETHERNET_RECEIVE_PACKET, 0, 0, Pointer_Packet_Size, Pointer_Buffer);
}

/** Send an ethernet frame.
 * @param Frame_Size The frame size in bytes.
 * @param Pointer_Buffer The frame content.
 */
static inline void NetworkBaseEthernetSendPacket(unsigned int Packet_Size, void *Pointer_Buffer)
{
	SystemCall(SYSTEM_CALL_ETHERNET_SEND_PACKET, Packet_Size, 0, Pointer_Buffer, NULL);
}

/** Tell in a non-blocking way if a packet has been received or not.
 * @return 1 if a packet has been received,
 * @return 0 if no packet was received.
 */
static inline int NetworkBaseEthernetIsPacketReceived(void)
{
	return SystemCall(SYSTEM_CALL_ETHERNET_IS_PACKET_RECEIVED, 0, 0, NULL, NULL);
}

/** Retrieve the board MAC address.
 * @param Pointer_MAC_Address On output, contain the board MAC address. The MAC address buffer must be NETWORK_BASE_MAC_ADDRESS_SIZE bytes long.
 */
static inline void NetworkBaseGetEthernetControllerMACAddress(unsigned char *Pointer_MAC_Address)
{
	SystemCall(SYSTEM_CALL_SYSTEM_GET_PARAMETER, SYSTEM_CALL_SYSTEM_PARAMETER_ID_ETHERNET_CONTROLLER_MAC_ADDRESS, 0, Pointer_MAC_Address, NULL);
}

/** Send an ARP request to find the MAC address corresponding to the provided IP address.
 * @param Pointer_Known_IP_Address The equipment IP address to which MAC address should be determined.
 * @param Pointer_Found_MAC_Address On output, contain the found MAC address.
 * @return 0 if the MAC address was successfully found,
 * @return 1 if the equipment did not answer and the MAC address could not be determined.
 */
static int NetworkBaseARPSendRequest(TNetworkIPAddress *Pointer_Known_IP_Address, unsigned char *Pointer_Found_MAC_Address)
{
	unsigned char Transmission_Packet[NETWORK_PACKET_BUFFER_MAXIMUM_SIZE], Reception_Packet[NETWORK_PACKET_BUFFER_MAXIMUM_SIZE];
	TNetworkBaseEthernetHeader *Pointer_Ethernet_Address = (TNetworkBaseEthernetHeader *) Transmission_Packet;
	TNetworkBaseARPPayload *Pointer_ARP_Payload = (TNetworkBaseARPPayload *) &Transmission_Packet[sizeof(TNetworkBaseEthernetHeader)];
	int i, Is_Packet_Received;
	unsigned int Timer_Value, Timeout_Value, Received_Packet_Size;
	
	// Prepare the ethernet header
	MemorySetAreaValue(Pointer_Ethernet_Address->Destination_MAC_Address, NETWORK_BASE_MAC_ADDRESS_SIZE, 0xFF); // Broadcast the frame to everyone on the local network
	Pointer_Ethernet_Address->Protocol_Type = NETWORK_BASE_ETHERNET_TYPE_ARP;
	
	// Prepare the ARP request
	Pointer_ARP_Payload->Hardware_Address_Space = NETWORK_BASE_ARP_HARDWARE_ADDRESS_SPACE_ETHERNET;
	Pointer_ARP_Payload->Protocol_Address_Space = NETWORK_BASE_ARP_PROTOCOL_ADDRESS_SPACE_IP;
	Pointer_ARP_Payload->Hardware_Address_Size = NETWORK_BASE_MAC_ADDRESS_SIZE;
	Pointer_ARP_Payload->Protocol_Address_Size = NETWORK_BASE_IP_ADDRESS_SIZE;
	Pointer_ARP_Payload->Opcode = NETWORK_BASE_ARP_OPCODE_REQUEST;
	MemoryCopyArea(Network_Base_System_MAC_Address, Pointer_ARP_Payload->Sender_Hardware_Address, NETWORK_BASE_MAC_ADDRESS_SIZE);
	Pointer_ARP_Payload->Sender_Protocol_Address = Network_Base_System_IP_Address.Address;
	Pointer_ARP_Payload->Target_Protocol_Address = Pointer_Known_IP_Address->Address;
	
	// Now that the transmission packet is built, reuse the pointers for the reception packet
	Pointer_Ethernet_Address = (TNetworkBaseEthernetHeader *) Reception_Packet;
	Pointer_ARP_Payload = (TNetworkBaseARPPayload *) &Reception_Packet[sizeof(TNetworkBaseEthernetHeader)];
	
	// Send the request and try to get a reply
	for (i = 0; i < 5; i++)
	{
		NetworkBaseEthernetSendPacket(sizeof(TNetworkBaseEthernetHeader) + sizeof(TNetworkBaseARPPayload), Transmission_Packet);
		
		// Wait 3ms for a reply
		Timeout_Value = SystemGetTimerValue() + 3;
		do
		{
			Is_Packet_Received = NetworkBaseEthernetIsPacketReceived();
			Timer_Value = SystemGetTimerValue();
		} while ((!Is_Packet_Received) && (Timer_Value < Timeout_Value));
		
		if (!Is_Packet_Received) continue; // Nothing was received, retry
		
		// A packet has been received, make sure it is the reply we were waiting for
		NetworkBaseEthernetReceivePacket(&Received_Packet_Size, Reception_Packet);
		
		// Is the packet big enough ?
		if (Received_Packet_Size < sizeof(TNetworkBaseEthernetHeader) + sizeof(TNetworkBaseARPPayload)) continue;
		
		// Is it an ARP packet ?
		if (Pointer_Ethernet_Address->Protocol_Type != NETWORK_BASE_ETHERNET_TYPE_ARP) continue;
		
		// Is the IP address the one we are looking for (the IP address is faster to compare than the MAC address) ?
		if (Pointer_ARP_Payload->Sender_Protocol_Address != Pointer_Known_IP_Address->Address) continue;
		
		// This is our reply, get the MAC address
		MemoryCopyArea(Pointer_ARP_Payload->Sender_Hardware_Address, Pointer_Found_MAC_Address, NETWORK_BASE_MAC_ADDRESS_SIZE);
		return 0;
	}
	
	// Failed to receive a reply
	return 1;
}

/** TODO */
/*static */int NetworkBaseGetMACAddressFromARPTable(TNetworkIPAddress *Pointer_IP_Address, unsigned char *Pointer_MAC_Address)
{
	int i, Return_Value = 1;
	
	// Search in the table if it not empty
	if (Network_Base_ARP_Table_Used_Entries_Count > 0)
	{
		for (i = 0; i < NETWORK_BASE_ARP_TABLE_SIZE; i++)
		{
			if (Network_Base_ARP_Table_Entries[i].Is_Entry_Free) continue;
			
			// Check for a matching IP
			if (Network_Base_ARP_Table_Entries[i].IP_Address == Pointer_IP_Address->Address)
			{
				// The IP was found, get the corresponding MAC
				MemoryCopyArea(Network_Base_ARP_Table_Entries[i].MAC_Address, Pointer_MAC_Address, NETWORK_BASE_MAC_ADDRESS_SIZE);
				return 0;
			}
		}
	}
	
	// No entry matched, need to do an ARP request
	if ((i == NETWORK_BASE_ARP_TABLE_SIZE) || (Network_Base_ARP_Table_Used_Entries_Count == 0))
	{
		if (NetworkBaseARPSendRequest(Pointer_IP_Address, Pointer_MAC_Address) != 0) return 1; // The equipment did not answer, no MAC address could be found
		
		// Add the MAC to the ARP table
		if (Network_Base_ARP_Table_Used_Entries_Count == NETWORK_BASE_ARP_TABLE_SIZE) return 0; // The table is full, can't add the entry
		// Find an empty entry
		for (i = 0; i < NETWORK_BASE_ARP_TABLE_SIZE; i++)
		{
			if (Network_Base_ARP_Table_Entries[i].Is_Entry_Free)
			{
				Network_Base_ARP_Table_Entries[i].IP_Address = Pointer_IP_Address->Address;
				MemoryCopyArea(Pointer_MAC_Address, Network_Base_ARP_Table_Entries[i].MAC_Address, NETWORK_BASE_MAC_ADDRESS_SIZE);
				Network_Base_ARP_Table_Entries[i].Is_Entry_Free = 0;
				
				Network_Base_ARP_Table_Used_Entries_Count++;
				break;
			}
		}
	}
	
	return 0;
}

/** Send an IP packet.
 * @param
 */
//static inline void NetworkBaseIPSendPacket(unsigned int Packet_Size, 
// si gw => envoyer à mac gw
// sinon, regarder table arp pour trouver mac

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int NetworkBaseInitialize(TNetworkIPAddress *Pointer_System_IP_Address, TNetworkIPAddress *Pointer_Gateway_IP_Address)
{
	int i;
	unsigned char Gateway_MAC_Address[NETWORK_BASE_MAC_ADDRESS_SIZE];
	
	// Clear ARP table
	for (i = 0; i < NETWORK_BASE_ARP_TABLE_SIZE; i++) Network_Base_ARP_Table_Entries[i].Is_Entry_Free = 1;
	Network_Base_ARP_Table_Used_Entries_Count = 0;
	
	// Set the network stack source MAC address
	SystemCall(SYSTEM_CALL_SYSTEM_GET_PARAMETER, SYSTEM_CALL_SYSTEM_PARAMETER_ID_ETHERNET_CONTROLLER_MAC_ADDRESS, 0, Network_Base_System_MAC_Address, NULL);
	
	// Set IP addresses
	MemoryCopyArea(Pointer_System_IP_Address, &Network_Base_System_IP_Address, sizeof(TNetworkIPAddress));
	MemoryCopyArea(Pointer_Gateway_IP_Address, &Network_Base_Gateway_IP_Address, sizeof(TNetworkIPAddress)); // Do not cache the gateway MAC address now because this equipment could be down at the network configuration time
	
	return 0;
}

#if 0
void NetworkBaseIPSendPacket(TNetworkIPAddress who, proto, unsigned int Packet_Size, data)
{
	// Is the destination IP address in the local subnet ?
	
	/*oui, ip
	
	non, mettre ip gw
	
	trouver mac
	sinon arp
	
	si pas mac return 1*/
	
	
	
}
#endif