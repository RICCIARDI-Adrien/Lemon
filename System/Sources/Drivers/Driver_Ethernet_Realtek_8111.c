/** @file Driver_Ethernet_Realtek_8111.c
 * Realtek 8111, 8168 and 8169 ethernet chips driver.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Debug.h>
#include <Drivers/Driver_Ethernet.h>
#include <Drivers/Driver_PCI.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Reset the whole controller when set to 1 (this bit is self-cleared when the reset is finished). */
#define ETHERNET_CONTROLLER_COMMAND_REGISTER_RESET_BIT 4
/** Enable reception when set to 1. */
#define ETHERNET_CONTROLLER_COMMAND_REGISTER_RECEIVER_ENABLE_BIT 3
/** Enable transmission when set to 1. */
#define ETHERNET_CONTROLLER_COMMAND_REGISTER_TRANSMITTER_ENABLE_BIT 2

/** Tell that there are some normal priority packets ready to be transmitted. */
#define ETHERNET_CONTROLLER_TRANSMIT_PRIORITY_POLLING_REGISTER_NORMAL_PRIORITY_QUEUE_POLLING_BIT 6

/** Allow broadcast packets to be received. */
#define ETHERNET_CONTROLLER_RECEIVE_CONFIGURATION_REGISTER_ACCEPT_BROADCAST_PACKETS_BIT 3

/** This bit is set when the link speed is 1Gbit/s. */
#define ETHERNET_CONTROLLER_PHY_STATUS_REGISTER_LINK_SPEED_1000MBPS_BIT 4
/** This bit is set when the link speed is 100Mbit/s. */
#define ETHERNET_CONTROLLER_PHY_STATUS_REGISTER_LINK_SPEED_100MBPS_BIT 3
/** This bit is set when the link speed is 10Mbit/s. */
#define ETHERNET_CONTROLLER_PHY_STATUS_REGISTER_LINK_SPEED_10MBPS_BIT 2
/** This bit is set when the link is up. */
#define ETHERNET_CONTROLLER_PHY_STATUS_REGISTER_LINK_STATUS_BIT 1
/** This bit is set when the link is in full-duplex mode, the bit is cleared when the link is in half-duplex mode. */
#define ETHERNET_CONTROLLER_PHY_STATUS_REGISTER_LINK_FULL_DUPLEX_STATUS_BIT 0

/** Set to 1 to start transmitting data. The bit is automatically cleared when data has been transmitted. */
#define ETHERNET_CONTROLLER_TRANSMIT_DESCRIPTOR_OWNERSHIP_BIT 31
/** Set to 1 to tell that this is the last descriptor in the descriptors ring. */
#define ETHERNET_CONTROLLER_TRANSMIT_DESCRIPTOR_END_OF_DESCRIPTOR_RING_BIT 30
/** Tell that the descriptor contains the packet beginning. */
#define ETHERNET_CONTROLLER_TRANSMIT_DESCRIPTOR_FIRST_SEGMENT_DESCRIPTOR_BIT 29
/** Tell that the descriptor contains the packet end. */
#define ETHERNET_CONTROLLER_TRANSMIT_DESCRIPTOR_LAST_SEGMENT_DESCRIPTOR_BIT 28

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** Ethernet controller registers map. */
typedef struct __attribute__((packed))
{
	// Offset 0
	unsigned char ID_Registers[6]; //!< Hold the MAC address loaded from the EEPROM (warning, this field must be written using double-word accesses).
		
	unsigned char Reserved_1[0x37 - 6];
	
	/*unsigned int ID_Registers_Low;
	unsigned int ID_Registers_High;
		
	unsigned char Reserved_1[0x37 - 8];*/
	
	// Offset 0x37
	unsigned char Command;
	// Offset 0x38
	unsigned char Transmit_Priority_Polling; //!< Tell if there is a packet to send (in high or in normal priority rings).
	
	unsigned char Reserved_2[0x3C - 2];
	
	// Offset 0x3C
	unsigned short Interrupt_Mask;
	// Offset 0x3E
	unsigned short Interrupt_Status;
	
	unsigned char Reserved_3[0x44 - 4];
	
	// Offset 0x44
	unsigned int Receive_Configuration;
	
	unsigned char Reserved_4[0x6C - 4];
	
	// Offset 0x6C
	unsigned char PHY_Status; //!< Reflect link status (speed, duplex mode, state...).
	
	unsigned char Reserved_5[0xDA - 1];
	
	// Offset 0xDA
	unsigned int Receive_Packet_Maximum_Size;
	
	unsigned char Reserved_6[0xE4 - 4];
	
	// Offset 0xE4
	volatile void *Pointer_Receive_Descriptor_Start_Address_High;
	// Offset 0xE8
	volatile void *Pointer_Receive_Descriptor_Start_Address_Low;
	
} TEthernetControllerRegisters;

/** A receive descriptor. */
typedef struct __attribute__((packed))
{
	unsigned short Flags; //!< Bits 31..14.
	unsigned short Buffer_Size; //!< Bits 13..0.
	unsigned int VLAN_Settings; //!< Not used here.
	void *Pointer_Buffer_Address_Low; //!< The buffer address.
	void *Pointer_Buffer_Address_High; //!< Set to 0 when using 32-bit PCI mode.
} TEthernetControllerReceiveDescriptor;

/** A normal transmit descriptor (Large-Send Task Offload transmit descriptors are not needed by this network stack). */
typedef struct __attribute__((packed))
{
	unsigned short Flags; //!< Gather all useful flags for Lemon simple driver model.
	unsigned short Frame_Length; //!< The data to transmit size in bytes.
	unsigned int VLAN_Settings; //!< VLAN hardware offloading, not used here.
	void *Pointer_Buffer_Address_Low; //!< The buffer address.
	void *Pointer_Buffer_Address_High; //!< Set to 0 when using 32-bit PCI mode.
} TEthernetControllerTransmitDescriptor;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The controller registers. */
static TEthernetControllerRegisters *Pointer_Ethernet_Controller_Registers;

/** The receive buffer descriptors list (currently limited to only one descriptor). */
static volatile TEthernetControllerReceiveDescriptor __attribute__((aligned(256))) Ethernet_Controller_Receive_Descriptor; // Must be 256-byte aligned

/** The buffer dedicated to packets reception. */
static unsigned char Ethernet_Controller_Reception_Buffer[CONFIGURATION_ETHERNET_BUFFER_SIZE];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int EthernetInitialize(void)
{
	TPCIDeviceID Device_ID;
	TPCIConfigurationSpaceHeader Configuration_Space_Header;
	unsigned int i;//, MAC_Address_Double_Words[2];
	
	// Try to find an ethernet controller (can't directly use the device ID to find the device as the device ID is programmed by the board manufacturer with an unknown value)
	if (PCIFindDeviceFromClass(PCI_CLASS_CODE_BASE_NETWORK_CONTROLLER, PCI_CLASS_CODE_SUB_CLASS_ETHERNET, &Device_ID) != 0) return 1;
	PCIGetConfigurationSpaceHeader(&Device_ID, &Configuration_Space_Header);
	
	// Is it a Realtek controller ?
	if (Configuration_Space_Header.Vendor_ID != PCI_VENDOR_ID_REALTEK) return 2;
	
	DEBUG_SECTION_START
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
		ScreenWriteString("Ethernet controller found.\nVendor ID : 0x");
		DebugWriteHexadecimalInteger(Configuration_Space_Header.Vendor_ID);
		ScreenWriteString(", device ID : 0x");
		DebugWriteHexadecimalInteger(Configuration_Space_Header.Device_ID);
		ScreenWriteString(", BAR[2] : 0x");
		DebugWriteHexadecimalInteger(PCI_GET_BASE_ADDRESS(Configuration_Space_Header.Base_Address_Registers[2]));
		ScreenWriteCharacter('\n');
		KeyboardReadCharacter();
	DEBUG_SECTION_END
	
	// Give access to registers
	Pointer_Ethernet_Controller_Registers = (TEthernetControllerRegisters *) PCI_GET_BASE_ADDRESS(Configuration_Space_Header.Base_Address_Registers[2]);
	
	// Reset the controller
	/*DEBUG_SECTION_START
	DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
	{
		ScreenWriteString("Resetting ethernet controller.\n");
		KeyboardReadCharacter();
	}
	DEBUG_SECTION_END
	
	Pointer_Ethernet_Controller_Registers->Command = 1 << ETHERNET_CONTROLLER_COMMAND_REGISTER_RESET_BIT;
	while (Pointer_Ethernet_Controller_Registers->Command & (1 << ETHERNET_CONTROLLER_COMMAND_REGISTER_RESET_BIT)); // Wait for the reset to terminate
	*/
	
	// Enable link auto-negotiation
	
	
	
	//===============================================
	// Reception configuration
	//===============================================
	// Set the maximum receivable packet size (in bytes)
	Pointer_Ethernet_Controller_Registers->Receive_Packet_Maximum_Size = CONFIGURATION_ETHERNET_BUFFER_SIZE;
	
	// Configure the reception behavior
	Pointer_Ethernet_Controller_Registers->Receive_Configuration = (0x07 << 13) | (0x07 << 8) | (1 << ETHERNET_CONTROLLER_RECEIVE_CONFIGURATION_REGISTER_ACCEPT_BROADCAST_PACKETS_BIT); // Copy a received packet to host memory when the packet has been fully received, set DMA burst size to unlimited (TODO : why ?), accept broadcast packets, discard erroneous packets
	
	// Initialize the only reception descriptor
	Ethernet_Controller_Receive_Descriptor.Flags = 0; // The descriptor is owned by the system
	Ethernet_Controller_Receive_Descriptor.Pointer_Buffer_Address_High = NULL;
	Ethernet_Controller_Receive_Descriptor.Pointer_Buffer_Address_Low = Ethernet_Controller_Reception_Buffer;
	
	// Set the reception buffers ring
	Pointer_Ethernet_Controller_Registers->Pointer_Receive_Descriptor_Start_Address_High = NULL; // Needed only in 64-bit PCI mode
	Pointer_Ethernet_Controller_Registers->Pointer_Receive_Descriptor_Start_Address_Low = &Ethernet_Controller_Receive_Descriptor;
	
	//===============================================
	// Transmission configuration
	//===============================================
	
	
	// Mask all interrupts
	Pointer_Ethernet_Controller_Registers->Interrupt_Mask = 0;
	
	// The controller MAC address is loaded from the board EEPROM, no need to set it. Get it at the initialization end to make sure the field was not overwritten by previous initialization
	// Get the MAC address (this PCI registers must be read using 32-bit accesses)
	/*MAC_Address_Double_Words[0] = Pointer_Ethernet_Controller_Registers->ID_Registers_Low;
	MAC_Address_Double_Words[1] = Pointer_Ethernet_Controller_Registers->ID_Registers_High;
	
	// Put bytes in displayable order
	Ethernet_Controller_MAC_Address[0] = (unsigned char) MAC_Address_Double_Words[0];
	Ethernet_Controller_MAC_Address[1] = MAC_Address_Double_Words[0] >> 8;
	Ethernet_Controller_MAC_Address[2] = MAC_Address_Double_Words[0] >> 16;
	Ethernet_Controller_MAC_Address[3] = MAC_Address_Double_Words[0] >> 24;
	Ethernet_Controller_MAC_Address[4] = (unsigned char) MAC_Address_Double_Words[1];
	Ethernet_Controller_MAC_Address[5] = MAC_Address_Double_Words[1] >> 8;*/
	for (i = 0; i < sizeof(Ethernet_Controller_MAC_Address); i++) Ethernet_Controller_MAC_Address[i] = Pointer_Ethernet_Controller_Registers->ID_Registers[i];
	
	// Enable transmission and reception
	//Pointer_Ethernet_Controller_Registers->Command |= (1 << ETHERNET_CONTROLLER_COMMAND_REGISTER_RECEIVER_ENABLE_BIT) | (1 << ETHERNET_CONTROLLER_COMMAND_REGISTER_TRANSMITTER_ENABLE_BIT);
	
	DEBUG_SECTION_START
	DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
	{
		unsigned char Device_Status;
		char *String_Text;
		
		// Display MAC address
		ScreenWriteString("MAC address : ");
		for (i = 0; i < sizeof(Ethernet_Controller_MAC_Address); i++)
		{
			DebugWriteHexadecimalByte(Ethernet_Controller_MAC_Address[i]);
			if (i < sizeof(Ethernet_Controller_MAC_Address) - 1) ScreenWriteCharacter(':');
		}
		ScreenWriteCharacter('\n');
		
		// Cache device status
		Device_Status = Pointer_Ethernet_Controller_Registers->PHY_Status;
		
		ScreenWriteString("Link state : ");
		if (Device_Status & (1 << ETHERNET_CONTROLLER_PHY_STATUS_REGISTER_LINK_STATUS_BIT)) String_Text = "up";
		else String_Text = "down";
		ScreenWriteString(String_Text);
		
		ScreenWriteString("\nDuplex mode : ");
		if (Device_Status & (1 << ETHERNET_CONTROLLER_PHY_STATUS_REGISTER_LINK_FULL_DUPLEX_STATUS_BIT)) String_Text = "full";
		else String_Text = "half";
		ScreenWriteString(String_Text);
		
		ScreenWriteString("\nLink speed : ");
		if (Device_Status & (1 << ETHERNET_CONTROLLER_PHY_STATUS_REGISTER_LINK_SPEED_10MBPS_BIT)) String_Text = "10";
		else if (Device_Status & (1 << ETHERNET_CONTROLLER_PHY_STATUS_REGISTER_LINK_SPEED_100MBPS_BIT)) String_Text = "100";
		else String_Text = "1000";
		ScreenWriteString(String_Text);
		ScreenWriteString(" Mb/s\n");
		
		KeyboardReadCharacter();
	}
	DEBUG_SECTION_END
	
	return 0;
}

void EthernetReceivePacket(unsigned int __attribute__((unused)) *Pointer_Buffer_Size, void __attribute__((unused)) *Pointer_Buffer)
{
	// TODO
}

void EthernetSendPacket(unsigned int __attribute__((unused)) Buffer_Size, void __attribute__((unused)) *Pointer_Buffer)
{
	// TODO
	
	// Tell that a packet is available to send
	// Pointer_Ethernet_Controller_Registers->Transmit_Priority_Polling |= ETHERNET_CONTROLLER_TRANSMIT_PRIORITY_POLLING_REGISTER_NORMAL_PRIORITY_QUEUE_POLLING_BIT;
}

int EthernetIsPacketReceived(void)
{
	// TODO
	return 0;
}