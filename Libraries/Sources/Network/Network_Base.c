/** @file Network_Base.c
 * @see Network_Base.h for description.
 * @author Adrien RICCIARDI
 */
#include <Network_Base.h>
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Tell that the ethernet frame contains an IP packet. */
#define NETWORK_BASE_ETHERNET_PROTOCOL_TYPE_IP NETWORK_SWAP_WORD(0x0800)
/** Tell that the ethernet frame contains an ARP packet. */
#define NETWORK_BASE_ETHERNET_PROTOCOL_TYPE_ARP NETWORK_SWAP_WORD(0x0806)

/** The hardware address space corresponding to ethernet. */
#define NETWORK_BASE_ARP_HARDWARE_ADDRESS_SPACE_ETHERNET NETWORK_SWAP_WORD(0x0001)
/** The protocol address space corresponding to IP. */
#define NETWORK_BASE_ARP_PROTOCOL_ADDRESS_SPACE_IP NETWORK_SWAP_WORD(0x0800)

/** An ARP request opcode value. */
#define NETWORK_BASE_ARP_OPCODE_REQUEST NETWORK_SWAP_WORD(0x0001)
/** An ARP reply opcode value. */
#define NETWORK_BASE_ARP_OPCODE_REPLY NETWORK_SWAP_WORD(0x0002)

/** The length in bytes of an IPv4 address. */
#define NETWORK_BASE_IP_ADDRESS_SIZE sizeof(int)

/** The ARP table entries count. */
#define NETWORK_BASE_ARP_TABLE_SIZE 16

/** How many equipments the packet can cross before being dropped. */
#define NETWORK_BASE_IP_DEFAULT_TIME_TO_LIVE_VALUE 255

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** An ARP protocol payload (ethernet network version). */
typedef struct __attribute__((packed))
{
	unsigned short Hardware_Address_Space; //!< When issuing a request, this is the type of the data we want to retrieve.
	unsigned short Protocol_Address_Space; //!< When issuing a request, this is the type of data we transmit.
	unsigned char Hardware_Address_Size; //!< The size in bytes of the hardware address.
	unsigned char Protocol_Address_Size; //!< The size in bytes of the protocol address.
	unsigned short Opcode; //!< Tell if it is a request or a reply.
	unsigned char Sender_Hardware_Address[NETWORK_MAC_ADDRESS_SIZE]; //!< The sender MAC address.
	unsigned int Sender_Protocol_Address; //!< The sender IPv4 address;
	unsigned char Target_Harware_Address[NETWORK_MAC_ADDRESS_SIZE]; //!< The destination MAC address.
	unsigned int Target_Protocol_Address; //!< The destination IPv4 address.
} TNetworkBaseARPPayload;

/** An ARP table entry (does the IP to MAC matching). */
typedef struct
{
	unsigned int IP_Address; //!< The IPv4 address (no need for subnet mask here).
	unsigned char MAC_Address[NETWORK_MAC_ADDRESS_SIZE]; //!< The MAC address corresponding to the IP address.
	int Is_Entry_Free; //!< Tell if this record contains valid data or not.
} TNetworkBaseARPTableEntry;

/** The TCP pseudo header used to compute the TCP header checksum. */
typedef struct __attribute__((packed))
{
	unsigned int Source_Address; //!< The packet IP source address.
	unsigned int Destination_Address; //!< The packet IP destination address.
	unsigned char Zero; //!< This field is always zero.
	unsigned char Protocol; //!< This field is always set to the TCP protocol value.
	unsigned short TCP_Header_And_Payload_Length; //!< Normal TCP header plus TCP payload size in bytes.
} TNetworkBaseTCPPseudoHeader;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
TNetworkIPAddress Network_Base_System_IP_Address;
TNetworkIPAddress Network_Base_Gateway_IP_Address;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The system MAC address. */
static unsigned char Network_Base_System_MAC_Address[NETWORK_MAC_ADDRESS_SIZE];

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
 * @param Pointer_MAC_Address On output, contain the board MAC address. The MAC address buffer must be NETWORK_MAC_ADDRESS_SIZE bytes long.
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
	unsigned char Transmission_Packet[NETWORK_MAXIMUM_PACKET_SIZE], Reception_Packet[NETWORK_MAXIMUM_PACKET_SIZE];
	TNetworkEthernetHeader *Pointer_Ethernet_Header = (TNetworkEthernetHeader *) Transmission_Packet;
	TNetworkBaseARPPayload *Pointer_ARP_Payload = (TNetworkBaseARPPayload *) &Transmission_Packet[sizeof(TNetworkEthernetHeader)];
	int i, Is_Packet_Received;
	unsigned int Timer_Value, Timeout_Value, Received_Packet_Size;
	
	// Prepare the ethernet header
	MemorySetAreaValue(Pointer_Ethernet_Header->Destination_MAC_Address, NETWORK_MAC_ADDRESS_SIZE, 0xFF); // Broadcast the frame to everyone on the local network
	Pointer_Ethernet_Header->Protocol_Type = NETWORK_BASE_ETHERNET_PROTOCOL_TYPE_ARP;
	
	// Prepare the ARP request
	Pointer_ARP_Payload->Hardware_Address_Space = NETWORK_BASE_ARP_HARDWARE_ADDRESS_SPACE_ETHERNET;
	Pointer_ARP_Payload->Protocol_Address_Space = NETWORK_BASE_ARP_PROTOCOL_ADDRESS_SPACE_IP;
	Pointer_ARP_Payload->Hardware_Address_Size = NETWORK_MAC_ADDRESS_SIZE;
	Pointer_ARP_Payload->Protocol_Address_Size = NETWORK_BASE_IP_ADDRESS_SIZE;
	Pointer_ARP_Payload->Opcode = NETWORK_BASE_ARP_OPCODE_REQUEST;
	MemoryCopyArea(Network_Base_System_MAC_Address, Pointer_ARP_Payload->Sender_Hardware_Address, NETWORK_MAC_ADDRESS_SIZE);
	Pointer_ARP_Payload->Sender_Protocol_Address = Network_Base_System_IP_Address.Address;
	Pointer_ARP_Payload->Target_Protocol_Address = Pointer_Known_IP_Address->Address;
	
	// Now that the transmission packet is built, reuse the pointers for the reception packet
	Pointer_Ethernet_Header = (TNetworkEthernetHeader *) Reception_Packet;
	Pointer_ARP_Payload = (TNetworkBaseARPPayload *) &Reception_Packet[sizeof(TNetworkEthernetHeader)];
	
	// Send the request and try to get a reply
	for (i = 0; i < 5; i++)
	{
		NetworkBaseEthernetSendPacket(sizeof(TNetworkEthernetHeader) + sizeof(TNetworkBaseARPPayload), Transmission_Packet);
		
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
		if (Received_Packet_Size < sizeof(TNetworkEthernetHeader) + sizeof(TNetworkBaseARPPayload)) continue;
		
		// Is it an ARP packet ?
		if (Pointer_Ethernet_Header->Protocol_Type != NETWORK_BASE_ETHERNET_PROTOCOL_TYPE_ARP) continue;
		
		// Is the IP address the one we are looking for (the IP address is faster to compare than the MAC address) ?
		if (Pointer_ARP_Payload->Sender_Protocol_Address != Pointer_Known_IP_Address->Address) continue;
		
		// This is our reply, get the MAC address
		MemoryCopyArea(Pointer_ARP_Payload->Sender_Hardware_Address, Pointer_Found_MAC_Address, NETWORK_MAC_ADDRESS_SIZE);
		return 0;
	}
	
	// Failed to receive a reply
	return 1;
}

/** Reply to an ARP request.
 * @param Pointer_Packet_Buffer The buffer containing the ARP request.
 * @warning The buffer will be modified to an ARP reply, losing the initial packet content.
 */
static void NetworkBaseARPSendReply(void *Pointer_Packet_Buffer)
{
	TNetworkEthernetHeader *Pointer_Ethernet_Header = (TNetworkEthernetHeader *) Pointer_Packet_Buffer;
	TNetworkBaseARPPayload *Pointer_ARP_Payload = (TNetworkBaseARPPayload *) (Pointer_Packet_Buffer + sizeof(TNetworkEthernetHeader));

	// Make sure the ARP request is fine
	if (Pointer_ARP_Payload->Hardware_Address_Space != NETWORK_BASE_ARP_HARDWARE_ADDRESS_SPACE_ETHERNET) return;
	if (Pointer_ARP_Payload->Protocol_Address_Space != NETWORK_BASE_ARP_PROTOCOL_ADDRESS_SPACE_IP) return;
	if (Pointer_ARP_Payload->Hardware_Address_Size != NETWORK_MAC_ADDRESS_SIZE) return;
	if (Pointer_ARP_Payload->Protocol_Address_Size != NETWORK_BASE_IP_ADDRESS_SIZE) return;
	if (Pointer_ARP_Payload->Opcode != NETWORK_BASE_ARP_OPCODE_REQUEST) return;
	
	// Is the provided IP address the one we have ?
	if (Pointer_ARP_Payload->Target_Protocol_Address != Network_Base_System_IP_Address.Address) return;
	
	// Prepare the reply
	Pointer_ARP_Payload->Opcode = NETWORK_BASE_ARP_OPCODE_REPLY; // Tell this is a reply
	// Set the receiver addresses (the one from the equipment that sent the ARP request)
	MemoryCopyArea(Pointer_ARP_Payload->Sender_Hardware_Address, Pointer_ARP_Payload->Target_Harware_Address, NETWORK_MAC_ADDRESS_SIZE);
	Pointer_ARP_Payload->Target_Protocol_Address = Pointer_ARP_Payload->Sender_Protocol_Address;
	// Set the sender addresses (this system ones)
	MemoryCopyArea(Network_Base_System_MAC_Address, Pointer_ARP_Payload->Sender_Hardware_Address, NETWORK_MAC_ADDRESS_SIZE);
	Pointer_ARP_Payload->Sender_Protocol_Address = Network_Base_System_IP_Address.Address;
	
	// Set the recipient MAC
	MemoryCopyArea(Pointer_ARP_Payload->Target_Harware_Address, Pointer_Ethernet_Header->Destination_MAC_Address, NETWORK_MAC_ADDRESS_SIZE);
	
	// Send the reply
	NetworkBaseEthernetSendPacket(sizeof(TNetworkEthernetHeader) + sizeof(TNetworkBaseARPPayload), Pointer_Packet_Buffer);
}

/** Compute one's complement checksum.
 * @param Pointer_Data The data to compute checksum on.
 * @param Bytes_Count The data size in bytes.
 * @return The checksum value (the result is directly in big endian if the data content is in big endian yet).
 */
static inline unsigned short NetworkBaseComputeChecksum(void *Pointer_Data, unsigned int Bytes_Count)
{
	unsigned int Checksum = 0, i, Words_Count;
	unsigned short *Pointer_Header_Words = (unsigned short *) Pointer_Data;
	unsigned char *Pointer_Header_Bytes;
	
	// Get the amount of 16-bit complete words
	Words_Count = Bytes_Count >> 1;
	
	// Compute the checksum on
	for (i = 0; i < Words_Count; i++)
	{
		Checksum += *Pointer_Header_Words;
		Pointer_Header_Words++;
	}
	
	// Add the remaining byte if the amount of data is odd
	if (Bytes_Count & 0x01)
	{
		Pointer_Header_Bytes = (unsigned char *) Pointer_Header_Words;
		Checksum += *Pointer_Header_Bytes;
	}
	
	// Add carry
	Checksum += Checksum >> 16;
	
	// Do checksum one's complement according to RFC
	Checksum = ~Checksum;
	
	return (unsigned short) Checksum;
}

/** Check if the received packet matches all the needed requirements.
 * @param Pointer_Socket The socket containing application requirements.
 * @param Pointer_Packet_Buffer The raw packet buffer (starting with the ethernet header).
 * @return 0 if the packet does not fulfill all requirements,
 * @return 1 if the packet is valid.
 */
static int NetworkBaseIPIsReceivedPacketValid(TNetworkSocket *Pointer_Socket, void *Pointer_Packet_Buffer)
{
	TNetworkEthernetHeader *Pointer_Ethernet_Header = (TNetworkEthernetHeader *) Pointer_Packet_Buffer;
	TNetworkIPv4Header *Pointer_IP_Header = (TNetworkIPv4Header *) (Pointer_Packet_Buffer + sizeof(TNetworkEthernetHeader));
	
	// Is the content an IP packet ?
	if (Pointer_Ethernet_Header->Protocol_Type != NETWORK_BASE_ETHERNET_PROTOCOL_TYPE_IP) return 0;
	
	// Discard fragmented IP packets (fragmentation is not handled by the network stack)
	if (NETWORK_SWAP_WORD(Pointer_IP_Header->Flags_And_Fragment_Offset) & NETWORK_IPV4_HEADER_FLAG_MORE_FRAGMENT) return 0;
	
	// Is the destination IP the system one ?
	if (Pointer_IP_Header->Destination_IP_Address != Network_Base_System_IP_Address.Address) return 0;
	
	// Is it the protocol the socket is listening for ?
	if (Pointer_IP_Header->Protocol != Pointer_Socket->IP_Protocol) return 0;
	
	return 1;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int NetworkBaseInitialize(TNetworkIPAddress *Pointer_System_IP_Address, TNetworkIPAddress *Pointer_Gateway_IP_Address)
{
	int i;
	unsigned char Gateway_MAC_Address[NETWORK_MAC_ADDRESS_SIZE];
	unsigned char Packet_Buffer[NETWORK_MAXIMUM_PACKET_SIZE];
	unsigned int Packet_Size;
	
	// Clear ARP table
	for (i = 0; i < NETWORK_BASE_ARP_TABLE_SIZE; i++) Network_Base_ARP_Table_Entries[i].Is_Entry_Free = 1;
	Network_Base_ARP_Table_Used_Entries_Count = 0;
	
	// Set the network stack source MAC address
	SystemCall(SYSTEM_CALL_SYSTEM_GET_PARAMETER, SYSTEM_CALL_SYSTEM_PARAMETER_ID_ETHERNET_CONTROLLER_MAC_ADDRESS, 0, Network_Base_System_MAC_Address, NULL);
	
	// Set IP addresses
	MemoryCopyArea(Pointer_System_IP_Address, &Network_Base_System_IP_Address, sizeof(TNetworkIPAddress));
	MemoryCopyArea(Pointer_Gateway_IP_Address, &Network_Base_Gateway_IP_Address, sizeof(TNetworkIPAddress)); // Do not cache the gateway MAC address now because this equipment could be down at the network configuration time
	
	// Flush the network controller reception buffer
	if (NetworkBaseEthernetIsPacketReceived()) NetworkBaseEthernetReceivePacket(&Packet_Size, Packet_Buffer);
	
	return 0;
}

int NetworkBaseGetMACAddressFromARPTable(TNetworkIPAddress *Pointer_IP_Address, unsigned char *Pointer_MAC_Address)
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
				MemoryCopyArea(Network_Base_ARP_Table_Entries[i].MAC_Address, Pointer_MAC_Address, NETWORK_MAC_ADDRESS_SIZE);
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
				MemoryCopyArea(Pointer_MAC_Address, Network_Base_ARP_Table_Entries[i].MAC_Address, NETWORK_MAC_ADDRESS_SIZE);
				Network_Base_ARP_Table_Entries[i].Is_Entry_Free = 0;
				
				Network_Base_ARP_Table_Used_Entries_Count++;
				break;
			}
		}
	}
	
	return 0;
}

int NetworkBaseIPReceivePacket(TNetworkSocket *Pointer_Socket, int Is_Call_Blocking, unsigned int *Pointer_Packet_Size, void *Pointer_Packet_Buffer)
{
	TNetworkEthernetHeader *Pointer_Ethernet_Header = (TNetworkEthernetHeader *) Pointer_Packet_Buffer;
	TNetworkIPv4Header *Pointer_IP_Header = (TNetworkIPv4Header *) (Pointer_Packet_Buffer + sizeof(TNetworkEthernetHeader));
	
	if (Is_Call_Blocking)
	{
		while (1)
		{
			// Wait for an IP packet to be received
			do
			{
				NetworkBaseEthernetReceivePacket(Pointer_Packet_Size, Pointer_Packet_Buffer);
				
				// Is it an ARP request for this ethernet controller ?
				if (Pointer_Ethernet_Header->Protocol_Type == NETWORK_BASE_ETHERNET_PROTOCOL_TYPE_ARP) NetworkBaseARPSendReply(Pointer_Packet_Buffer); // Answer the request
			} while (!NetworkBaseIPIsReceivedPacketValid(Pointer_Socket, Pointer_Packet_Buffer));
			
			return 0;
		}
	}
	else
	{
		// Is there a packet available ?
		if (!NetworkBaseEthernetIsPacketReceived()) return 2;
		
		// Get the packet
		NetworkBaseEthernetReceivePacket(Pointer_Packet_Size, Pointer_Packet_Buffer);
		// Is it an ARP request for this ethernet controller ?
		if (Pointer_Ethernet_Header->Protocol_Type == NETWORK_BASE_ETHERNET_PROTOCOL_TYPE_ARP)
		{
			NetworkBaseARPSendReply(Pointer_Packet_Buffer); // Answer the request
			return 2;
		}
		
		if (!NetworkBaseIPIsReceivedPacketValid(Pointer_Socket, Pointer_Packet_Buffer)) return 2;
		return 0;
	}
}

int NetworkBaseIPSendPacket(TNetworkSocket *Pointer_Socket, unsigned int Payload_Size, void *Pointer_Packet_Buffer)
{
	TNetworkEthernetHeader *Pointer_Ethernet_Header = (TNetworkEthernetHeader *) Pointer_Packet_Buffer;
	TNetworkIPv4Header *Pointer_IP_Header = (TNetworkIPv4Header *) (Pointer_Packet_Buffer + sizeof(TNetworkEthernetHeader));
	unsigned int Total_Packet_Size;
	
	// Discard the packet if it is too big
	Total_Packet_Size = sizeof(TNetworkEthernetHeader) + sizeof(TNetworkIPv4Header) + Payload_Size;
	if (Total_Packet_Size > NETWORK_MAXIMUM_PACKET_SIZE) return 1;
	
	// Fill the ethernet header
	MemoryCopyArea(Pointer_Socket->Destination_MAC_Address, Pointer_Ethernet_Header->Destination_MAC_Address, NETWORK_MAC_ADDRESS_SIZE); // Set the destination MAC address
	Pointer_Ethernet_Header->Protocol_Type = NETWORK_BASE_ETHERNET_PROTOCOL_TYPE_IP; // Tell that the frame contains IP data
	
	// Fill the IPv4 header
	Pointer_IP_Header->Version_And_Size = (4 << 4) | ((sizeof(TNetworkIPv4Header) / 4) & 0x0F); // IP version 4, fixed header size (header size will never change as options won't be used)
	Pointer_IP_Header->Differentiated_Services_Code_Point_And_Explicit_Congestion_Notification = 0; // Not used
	Pointer_IP_Header->Total_Length = NETWORK_SWAP_WORD(sizeof(TNetworkIPv4Header) + Payload_Size); // Header length + payload length
	Pointer_IP_Header->Identification = 0; // Unused
	Pointer_IP_Header->Flags_And_Fragment_Offset = 0; // Unused
	Pointer_IP_Header->Time_To_Live = NETWORK_BASE_IP_DEFAULT_TIME_TO_LIVE_VALUE; // How much hops the packet can do before being dropped
	Pointer_IP_Header->Protocol = (unsigned char) Pointer_Socket->IP_Protocol; // What does the payload contain
	Pointer_IP_Header->Source_IP_Address = Network_Base_System_IP_Address.Address; // This system is the packet sender
	Pointer_IP_Header->Destination_IP_Address = Pointer_Socket->Destination_IP_Address; // Who will receive this packet
	
	// Compute the IP header checksum
	Pointer_IP_Header->Header_Checksum = 0; // Set the checksum field to zero as told by the RFC
	Pointer_IP_Header->Header_Checksum = NetworkBaseComputeChecksum(Pointer_IP_Header, sizeof(TNetworkIPv4Header)); // No need to swap result to big endian because input data were swapped yet, giving a good result
	
	// Transmit the packet
	NetworkBaseEthernetSendPacket(Total_Packet_Size, Pointer_Packet_Buffer);
	
	return 0;
}

unsigned short NetworkBaseTCPComputeChecksum(TNetworkSocket *Pointer_Socket, void *Pointer_Data, unsigned int Bytes_Count)
{
	TNetworkBaseTCPPseudoHeader *Pointer_TCP_Pseudo_Header = (TNetworkBaseTCPPseudoHeader *) (Pointer_Data - sizeof(TNetworkBaseTCPPseudoHeader));
	
	// Fill the pseudo header
	Pointer_TCP_Pseudo_Header->Source_Address = Network_Base_System_IP_Address.Address;
	Pointer_TCP_Pseudo_Header->Destination_Address = Pointer_Socket->Destination_IP_Address;
	Pointer_TCP_Pseudo_Header->Zero = 0;
	Pointer_TCP_Pseudo_Header->Protocol = NETWORK_IP_PROTOCOL_TCP;
	Pointer_TCP_Pseudo_Header->TCP_Header_And_Payload_Length = NETWORK_SWAP_WORD(Bytes_Count);
	
	return NetworkBaseComputeChecksum(Pointer_TCP_Pseudo_Header, sizeof(TNetworkBaseTCPPseudoHeader) + Bytes_Count);
}

void NetworkBaseTCPPrepareHeader(TNetworkSocket *Pointer_Socket, unsigned short Flags, TNetworkTCPHeader *Pointer_TCP_Header)
{
	Pointer_TCP_Header->Source_Port = Pointer_Socket->Source_Port;
	Pointer_TCP_Header->Destination_Port = Pointer_Socket->Destination_Port;
	
	Pointer_TCP_Header->Header_Size_And_Flags = NETWORK_SWAP_WORD(((sizeof(TNetworkTCPHeader) / 4) << 12) | Flags);
	Pointer_TCP_Header->Window_Size = NETWORK_SWAP_WORD(NETWORK_MAXIMUM_PACKET_SIZE - sizeof(TNetworkEthernetHeader) - sizeof(TNetworkIPv4Header) - sizeof(TNetworkTCPHeader));
	Pointer_TCP_Header->Urgent_Pointer = 0;
	
	Pointer_TCP_Header->Checksum = 0; // The checksum field must be zero prior to compute the checksum
}
