/** @file Driver_Ethernet_Intel_82540EM.c
 * Intel 82540EM Gigabit ethernet controller (also known as Intel PRO/1000) driver.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Debug.h>
#include <Drivers/Driver_Ethernet.h>
#include <Drivers/Driver_PCI.h>
#include <Standard_Functions.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Set to 1 to enable auto-speed detection, set to 0 to force the link speed by software. */
#define ETHERNET_CONTROLLER_DEVICE_CONTROL_REGISTER_AUTO_SPEED_DETECTION_ENABLE_BIT 5
/** Allow the PHY to communicate with an eventual partner. */
#define ETHERNET_CONTROLLER_DEVICE_CONTROL_REGISTER_SET_LINK_UP_BIT 6

/** Tell if the link is full-duplex or half-duplex. */
#define ETHERNET_CONTROLLER_DEVICE_STATUS_REGISTER_LINK_FULL_DUPLEX_CONFIGURATION_INDICATION_BIT 0
/** Tell if the link is up or down. */
#define ETHERNET_CONTROLLER_DEVICE_STATUS_REGISTER_LINK_UP_INDICATION_BIT 1

/** Enable reception when set to 1, disable reception when set to 0. */
#define ETHERNET_CONTROLLER_RECEIVE_CONTROL_REGISTER_RECEIVER_ENABLE_BIT 1
/** Set to 1 to receive broadcast packets, set to 0 to disable broadcast packets reception. */
#define ETHERNET_CONTROLLER_RECEIVE_CONTROL_REGISTER_BROADCAST_ACCEPT_MODE_BIT 15

/** Enable transmission when set to 1, disable transmission when set to 0. */
#define ETHERNET_CONTROLLER_TRANSMIT_CONTROL_REGISTER_TRANSMIT_ENABLE_BIT 1
/** Enable or disable short packets padding. */
#define ETHERNET_CONTROLLER_TRANSMIT_CONTROL_REGISTER_PAD_SHORT_PACKETS_BIT 3

/** Tell that the packet contained in the buffer pointed by the transmit descriptor can be transmitted. */
#define ETHERNET_CONTROLLER_TRANSMIT_DESCRIPTOR_COMMAND_REGISTER_END_OF_PACKET_BIT (0 + 8) // Add 8 because the command byte is accessed as a 16-bit word with a little-endian processor
/** Tell the ethernet controller to set the Descriptor Done bit in the transmit descriptor when a packet has been successfully transmitted. */
#define ETHERNET_CONTROLLER_TRANSMIT_DESCRIPTOR_COMMAND_REGISTER_REPORT_STATUS_BIT (3 + 8) // Add 8 because the command byte is accessed as a 16-bit word with a little-endian processor

/** Tell that the packet has been successfully transmitted. */
#define ETHERNET_CONTROLLER_TRANSMIT_DESCRIPTOR_STATUS_REGISTER_DESCRIPTOR_DONE 0

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** Ethernet controller registers map. */
typedef struct __attribute__((packed))
{
	// Offset 0x0
	unsigned int Device_Control;
	unsigned int Device_Control_High; // Not used in 32 bits mode
	// Offset 0x08
	unsigned int Device_Status;
	unsigned int Device_Status_High; // Not used in 32 bits mode
	unsigned int Reserved_1[2];
	// Offset 0x18
	unsigned int Extended_Device_Control;
	unsigned int Extended_Device_Control_High; // Not used in 32 bits mode
	// Offset 0x20
	unsigned int MDI_Control;
	unsigned int MDI_Control_High; // Not used in 32 bits mode
	
	unsigned char Reserved_2[0xC0 - 0x28];
	
	// Offset 0xC0
	unsigned int Interrupt_Cause_Read;
	// Offset 0xC4
	unsigned int Interrupt_Throttling;
	// Offset 0xC8
	unsigned int Interrupt_Cause_Set;
	unsigned int Reserved_3;
	// Offset 0xD0
	unsigned int Interrupt_Mask_Set_Read;
	unsigned int Interrupt_Mask_Set_Read_High;
	// Offset 0xD8
	unsigned int Interrupt_Mask_Clear;
	unsigned int Interrupt_Mask_Clear_High;
	unsigned int Reserved_4[8];
	// Offset 0x100
	unsigned int Receive_Control;
	
	unsigned char Reserved_5[0x400 - 0x104];
	
	// Offset 0x400
	unsigned int Transmit_Control;
	unsigned int Reserved_6[3];
	// Offset 0x410
	unsigned int Transmit_Inter_Packet_Gap;
	
	unsigned char Reserved_7[0x2800 - 0x414];
	
	// Offset 0x2800
	volatile void *Pointer_Receive_Descriptor_Base_Address_Low;
	// Offset 0x2804
	void *Pointer_Receive_Descriptor_Base_Address_High;
	// Offset 0x2808
	unsigned int Receive_Descriptor_Length;
	unsigned int Receive_Descriptor_Length_High;
	// Offset 0x2810
	unsigned int Receive_Descriptor_Head;
	unsigned int Receive_Descriptor_Head_High;
	// Offset 0x2818
	unsigned int Receive_Descriptor_Tail;
	unsigned int Receive_Descriptor_Tail_High;
	// Offset 0x2820
	unsigned int Receive_Delay_Timer; //<! Set to zero to generate an interrupt each time a packet is received.
	
	unsigned char Reserved_8[0x3800 - 0x2824];
	
	// Offset 0x3800
	volatile void *Pointer_Transmit_Descriptor_Base_Address_Low;
	// Offset 0x3804
	void *Pointer_Transmit_Descriptor_Base_Address_High;
	// Offset 0x3808
	unsigned int Transmit_Descriptor_Length;
	unsigned int Transmit_Descriptor_Length_High;
	// Offset 0x3810
	unsigned int Transmit_Descriptor_Head;
	unsigned int Transmit_Descriptor_Head_High;
	// Offset 0x3818
	unsigned int Transmit_Descriptor_Tail;
	unsigned int Transmit_Descriptor_Tail_High;
	
	unsigned char Reserved_9[0x4000 - 0x3820];
	
	// Statistic registers
	// Offset 0x4000
	unsigned int CRC_Error_Count;
	// Offset 0x4004
	unsigned int Alignment_Error_Count;
	// Offset 0x4008
	unsigned int Symbol_Error_Count;
	// Offset 0x400C
	unsigned int RX_Error_Count;
	// Offset 0x4010
	unsigned int Missed_Packets_Count;
	// Offset 0x4014
	unsigned int Single_Collision_Count;
	// Offset 0x4018
	unsigned int Excessive_Collisions_Count;
	// Offset 0x401C
	unsigned int Multiple_Collision_Count;
	// Offset 0x4020
	unsigned int Late_Collisions_Count;
	unsigned int Reserved_10;
	// Offset 0x4028
	unsigned int Collision_Count;
	unsigned int Reserved_11;
	// Offset 0x4030
	unsigned int Defer_Count;
	// Offset 0x4034
	unsigned int Transmit_With_No_CRS;
	// Offset 0x4038
	unsigned int Sequence_Error_Count;
	// Offset 0x403C
	unsigned int Carrier_Extension_Error_Count;
	// Offset 0x4040
	unsigned int Receive_Length_Error_Count;
	unsigned int Reserved_12;
	// Offset 0x4048
	unsigned int XON_Received_Count;
	// Offset 0x404C
	unsigned int XON_Transmitted_Count;
	// Offset 0x4050
	unsigned int XOFF_Received_Count;
	// Offset 0x4054
	unsigned int XOFF_Transmitted_Count;
	// Offset 0x4058
	unsigned int FC_Received_Unsupported_Count;
	// Offset 0x405C
	unsigned int Packets_Received_64_Bytes_Count;
	// Offset 0x4060
	unsigned int Packets_Received_127_Bytes_Count;
	// Offset 0x4064
	unsigned int Packets_Received_255_Bytes_Count;
	// Offset 0x4068
	unsigned int Packets_Received_511_Bytes_Count;
	// Offset 0x406C
	unsigned int Packets_Received_1023_Bytes_Count;
	// Offset 0x4070
	unsigned int Packets_Received_Max_Bytes_Count;
	// Offset 0x4074
	unsigned int Good_Packets_Received_Count;
	// Offset 0x4078
	unsigned int Broadcast_Packets_Received_Count;
	// Offset 0x407C
	unsigned int Multicast_Packets_Received_Count;
	// Offset 0x4080
	unsigned int Good_Packets_Transmitted_Count;
	unsigned int Reserved_13;
	// Offset 0x4088
	unsigned int Good_Octets_Received_Count_Low;
	// Offset 0x408C
	unsigned int Good_Octets_Received_Count_High;
	// Offset 0x4090
	unsigned int Good_Octets_Transmitted_Count_Low;
	// Offset 0x4094
	unsigned int Good_Octets_Transmitted_Count_High;
	unsigned int Reserved_14[2];
	// Offset 0x40A0
	unsigned int Receive_No_Buffers_Count;
	// Offset 0x40A4
	unsigned int Receive_Undersize_Count;
	// Offset 0x40A8
	unsigned int Receive_Fragment_Count;
	// Offset 0x40AC
	unsigned int Receive_Oversize_Count;
	// Offset 0x40B0
	unsigned int Receive_Jabber_Count;
	// Offset 0x40B4
	unsigned int Management_Packets_Received_Count;
	// Offset 0x40B8
	unsigned int Management_Packets_Dropped_Count;
	// Offset 0x40BC
	unsigned int Management_Packets_Transmitted_Count;
	// Offset 0x40C0
	unsigned int Total_Octets_Received_Low;
	// Offset 0x40C4
	unsigned int Total_Octets_Received_High;
	// Offset 0x40C8
	unsigned int Total_Octets_Transmitted_Low;
	// Offset 0x40CC
	unsigned int Total_Octets_Transmitted_High;
	// Offset 0x40D0
	unsigned int Total_Packets_Received;
	// Offset 0x40D4
	unsigned int Total_Packets_Transmitted;
	// Offset 0x40D8
	unsigned int Packets_Transmitted_64_Bytes_Count;
	// Offset 0x40DC
	unsigned int Packets_Transmitted_127_Bytes_Count;
	// Offset 0x40E0
	unsigned int Packets_Transmitted_255_Bytes_Count;
	// Offset 0x40E4
	unsigned int Packets_Transmitted_511_Bytes_Count;
	// Offset 0x40E8
	unsigned int Packets_Transmitted_1023_Bytes_Count;
	// Offset 0x40EC
	unsigned int Packets_Transmitted_Max_Bytes_Count;
	// Offset 0x40F0
	unsigned int Multicast_Packets_Transmitted_Count;
	// Offset 0x40F4
	unsigned int Broadcast_Packets_Transmitted_Count;
	// Offset 0x40F8
	unsigned int TCP_Segmentation_Context_Transmitted_Count;
	// Offset 0x40FC
	unsigned int TCP_Segmentation_Context_Transmit_Fail_Count;
	
	unsigned char Reserved_15[0x5200 - 0x4100];
	
	// Offset 0x5200
	unsigned int Multicast_Table_Array_Entries[128];
	// Offset 0x5400
	unsigned int Receive_Addresses[32]; //<! There are 16 slots available. First slot is board MAC address, other slots are used as filters. All addresses are stored consecutively.
} TEthernetControllerRegisters;

/** A receive buffer descriptor. */
typedef struct __attribute__((packed))
{
	void *Pointer_Buffer_Address;
	void *Pointer_Buffer_Address_High;
	unsigned short Length;
	unsigned short Packet_Checksum;
	unsigned char Status;
	unsigned char Errors;
	unsigned short Special;
} TEthernetControllerReceiveDescriptor;

/** A transmit buffer descriptor. */
typedef struct __attribute__((packed))
{
	void *Pointer_Buffer_Address;
	void *Pointer_Buffer_Address_High;
	unsigned short Length;
	unsigned short Command_And_Checksum_Offset;
	unsigned char Status;
	unsigned char Checksum_Start_Field;
	unsigned short Special;
} TEthernetControllerTransmitDescriptor;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The controller registers. */
static TEthernetControllerRegisters *Pointer_Ethernet_Controller_Registers;

/** The receive buffer descriptors list (currently limited to only one descriptor). */
static volatile TEthernetControllerReceiveDescriptor __attribute__((aligned(16))) Ethernet_Controller_Receive_Descriptor; // Must be aligned on an Intel paragraph
/** The transmit buffer descriptors list (currently limited to only one descriptor). */
static volatile TEthernetControllerTransmitDescriptor __attribute__((aligned(16))) Ethernet_Controller_Transmit_Descriptor; // Must be aligned on an Intel paragraph

/** The buffer dedicated to packets reception. */
static unsigned char Ethernet_Controller_Reception_Buffer[CONFIGURATION_ETHERNET_BUFFER_SIZE];
/** The buffer dedicated to packets transmission. */
static unsigned char Ethernet_Controller_Transmission_Buffer[CONFIGURATION_ETHERNET_BUFFER_SIZE];

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
unsigned char Ethernet_Controller_MAC_Address[ETHERNET_CONTROLLER_MAC_ADDRESS_SIZE];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int EthernetInitialize(void)
{
	TPCIDeviceID Device_ID;
	TPCIConfigurationSpaceHeader Configuration_Space_Header;
	unsigned int i, MAC_Address_Double_Words[2];
	
	// Try to find an ethernet controller (can't directly use the device ID to find the device as the device ID is programmed by the board manufacturer with an unknown value)
	if (PCIFindDeviceFromClass(PCI_CLASS_CODE_BASE_NETWORK_CONTROLLER, PCI_CLASS_CODE_SUB_CLASS_ETHERNET, &Device_ID) != 0) return 1;
	PCIGetConfigurationSpaceHeader(&Device_ID, &Configuration_Space_Header);
	
	// Is it an Intel controller ?
	if (Configuration_Space_Header.Vendor_ID != PCI_VENDOR_ID_INTEL) return 2;
	
	DEBUG_SECTION_START
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
		ScreenWriteString("Ethernet controller found.\nVendor ID : 0x");
		DebugWriteHexadecimalInteger(Configuration_Space_Header.Vendor_ID);
		ScreenWriteString(", device ID : 0x");
		DebugWriteHexadecimalInteger(Configuration_Space_Header.Device_ID);
		ScreenWriteString(", BAR[0] : 0x");
		DebugWriteHexadecimalInteger(PCI_GET_BASE_ADDRESS(Configuration_Space_Header.Base_Address_Registers[0]));
		ScreenWriteCharacter('\n');
		KeyboardReadCharacter();
	DEBUG_SECTION_END
	
	// Give access to registers
	Pointer_Ethernet_Controller_Registers = (TEthernetControllerRegisters *) PCI_GET_BASE_ADDRESS(Configuration_Space_Header.Base_Address_Registers[0]);
	
	//===============================================
	// General configuration section of the datasheet
	//===============================================
	Pointer_Ethernet_Controller_Registers->Device_Control = (1 << ETHERNET_CONTROLLER_DEVICE_CONTROL_REGISTER_SET_LINK_UP_BIT) | (1 << ETHERNET_CONTROLLER_DEVICE_CONTROL_REGISTER_AUTO_SPEED_DETECTION_ENABLE_BIT);
	Pointer_Ethernet_Controller_Registers->Extended_Device_Control = 0; // Use GMII internal PHY mode, as recommended for 82540EM chip
	
	//===============================================
	// Receive initialization section of the datasheet
	//===============================================
	// Set all multicast entries to zero
	for (i = 0; i < sizeof(Pointer_Ethernet_Controller_Registers->Multicast_Table_Array_Entries) / sizeof(Pointer_Ethernet_Controller_Registers->Multicast_Table_Array_Entries[0]); i++) Pointer_Ethernet_Controller_Registers->Multicast_Table_Array_Entries[i] = 0;
	
	// Clear interrupts mask
	Pointer_Ethernet_Controller_Registers->Interrupt_Mask_Set_Read = 0;
	// Disable all interrupts (the system does not need to be told whether a packet arrived or not)
	Pointer_Ethernet_Controller_Registers->Interrupt_Mask_Clear = 0xFFFFFFFF;
	
	// Set the receive descriptors list starting address
	Pointer_Ethernet_Controller_Registers->Pointer_Receive_Descriptor_Base_Address_High = NULL;
	Pointer_Ethernet_Controller_Registers->Pointer_Receive_Descriptor_Base_Address_Low = &Ethernet_Controller_Receive_Descriptor;
	// Set the receive descriptors list length
	Pointer_Ethernet_Controller_Registers->Receive_Descriptor_Length = sizeof(Ethernet_Controller_Receive_Descriptor) << 6; // The value must be 128-bit aligned
	// Select the only available descriptor
	Pointer_Ethernet_Controller_Registers->Receive_Descriptor_Head = 0;
	Pointer_Ethernet_Controller_Registers->Receive_Descriptor_Tail = 1;
	
	// Configure the receive descriptor
	Ethernet_Controller_Receive_Descriptor.Status = 0;
	Ethernet_Controller_Receive_Descriptor.Errors = 0;
	Ethernet_Controller_Receive_Descriptor.Pointer_Buffer_Address = Ethernet_Controller_Reception_Buffer;
	
	// Configure the reception behaviour
	// TODO switch with available buffer values or always use 2048 ?
	Pointer_Ethernet_Controller_Registers->Receive_Control = 1 << ETHERNET_CONTROLLER_RECEIVE_CONTROL_REGISTER_BROADCAST_ACCEPT_MODE_BIT;
	Pointer_Ethernet_Controller_Registers->Receive_Delay_Timer = 0; // Generate an interrupt each time a packet is received
	
	//===============================================
	// Transmission initialization section of the datasheet
	//===============================================
	// Set the transmit descriptors list starting address
	Pointer_Ethernet_Controller_Registers->Pointer_Transmit_Descriptor_Base_Address_High = NULL;
	Pointer_Ethernet_Controller_Registers->Pointer_Transmit_Descriptor_Base_Address_Low = &Ethernet_Controller_Transmit_Descriptor;
	// Set the receive descriptors list length
	Pointer_Ethernet_Controller_Registers->Transmit_Descriptor_Length = sizeof(Ethernet_Controller_Transmit_Descriptor) << 6; // The value must be 128-bit aligned
	// Select the only available descriptor
	Pointer_Ethernet_Controller_Registers->Transmit_Descriptor_Head = 0;
	Pointer_Ethernet_Controller_Registers->Transmit_Descriptor_Tail = 0;
	
	// Configure the transmit descriptor
	Ethernet_Controller_Transmit_Descriptor.Pointer_Buffer_Address = Ethernet_Controller_Transmission_Buffer;
	
	// Configure the transmission behaviour
	Pointer_Ethernet_Controller_Registers->Transmit_Control = (0x40 << 12) | (0x0F << 4) | (1 << ETHERNET_CONTROLLER_TRANSMIT_CONTROL_REGISTER_PAD_SHORT_PACKETS_BIT);
	// Set the Inter Packet Gap value
	Pointer_Ethernet_Controller_Registers->Transmit_Inter_Packet_Gap = (10 << 20) | (10 << 10) | 10;
	
	// The controller MAC address is loaded from the board EEPROM, no need to set it. Get it at the initialization end to make sure the field was not overwritten by previous initialization
	// Get the MAC address (this PCI registers must be read using 32-bit accesses)
	MAC_Address_Double_Words[0] = Pointer_Ethernet_Controller_Registers->Receive_Addresses[0];
	MAC_Address_Double_Words[1] = Pointer_Ethernet_Controller_Registers->Receive_Addresses[1];
	
	// Put bytes in displayable order
	Ethernet_Controller_MAC_Address[0] = (unsigned char) MAC_Address_Double_Words[0];
	Ethernet_Controller_MAC_Address[1] = MAC_Address_Double_Words[0] >> 8;
	Ethernet_Controller_MAC_Address[2] = MAC_Address_Double_Words[0] >> 16;
	Ethernet_Controller_MAC_Address[3] = MAC_Address_Double_Words[0] >> 24;
	Ethernet_Controller_MAC_Address[4] = (unsigned char) MAC_Address_Double_Words[1];
	Ethernet_Controller_MAC_Address[5] = MAC_Address_Double_Words[1] >> 8;
	
	// Enable transmission
	Pointer_Ethernet_Controller_Registers->Transmit_Control |= 1 << ETHERNET_CONTROLLER_TRANSMIT_CONTROL_REGISTER_TRANSMIT_ENABLE_BIT;
	// Enable reception
	Pointer_Ethernet_Controller_Registers->Receive_Control |= 1 << ETHERNET_CONTROLLER_RECEIVE_CONTROL_REGISTER_RECEIVER_ENABLE_BIT;
	
	DEBUG_SECTION_START
	DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
	{
		unsigned int Device_Status;
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
		Device_Status = Pointer_Ethernet_Controller_Registers->Device_Status;
		
		ScreenWriteString("Link state : ");
		if (Device_Status & (1 << ETHERNET_CONTROLLER_DEVICE_STATUS_REGISTER_LINK_UP_INDICATION_BIT)) String_Text = "up";
		else String_Text = "down";
		ScreenWriteString(String_Text);
		
		ScreenWriteString("\nDuplex mode : ");
		if (Device_Status & (1 << ETHERNET_CONTROLLER_DEVICE_STATUS_REGISTER_LINK_FULL_DUPLEX_CONFIGURATION_INDICATION_BIT)) String_Text = "full";
		else String_Text = "half";
		ScreenWriteString(String_Text);
		
		ScreenWriteString("\nLink speed : ");
		switch ((Device_Status >> 6) & 0x00000003)
		{
			case 0:
				String_Text = "10";
				break;
				
			case 1:
				String_Text = "100";
				break;
				
			default:
				String_Text = "1000";
				break;
		}
		ScreenWriteString(String_Text);
		ScreenWriteString(" Mb/s\n");
		
		KeyboardReadCharacter();
	}
	DEBUG_SECTION_END
	
	return 0;
}

void EthernetReceivePacket(unsigned int *Pointer_Buffer_Size, void *Pointer_Buffer)
{
	// Wait for a packet to be received
	while (!Ethernet_Controller_Receive_Descriptor.Status);
	
	// TODO check for max packet size ?
	
	// Send packet to userspace
	*Pointer_Buffer_Size = Ethernet_Controller_Receive_Descriptor.Length; // Get packet size
	memcpy(Pointer_Buffer, Ethernet_Controller_Reception_Buffer, *Pointer_Buffer_Size); // Copy the packet content to userspace
	
	// Re-enable packet reception
	Ethernet_Controller_Receive_Descriptor.Status = 0; // Reset status bits as suggested in datasheet
	Pointer_Ethernet_Controller_Registers->Receive_Descriptor_Head = 0; // Reset reception buffer descriptors queue
	Pointer_Ethernet_Controller_Registers->Receive_Descriptor_Tail = 1;
}

void EthernetSendPacket(unsigned int Buffer_Size, void *Pointer_Buffer)
{
	// Set the source MAC address
	memcpy(Pointer_Buffer + ETHERNET_CONTROLLER_MAC_ADDRESS_SIZE, Ethernet_Controller_MAC_Address, ETHERNET_CONTROLLER_MAC_ADDRESS_SIZE);
	
	// Copy the packet content to the ethernet controller transmission buffer
	if (Buffer_Size > CONFIGURATION_ETHERNET_BUFFER_SIZE) Buffer_Size = CONFIGURATION_ETHERNET_BUFFER_SIZE; // Make sure the packet is not too big for the destination buffer
	memcpy(Ethernet_Controller_Transmission_Buffer, Pointer_Buffer, Buffer_Size);
	
	// Configure the transmission descriptor
	Ethernet_Controller_Transmit_Descriptor.Status = 0;
	Ethernet_Controller_Transmit_Descriptor.Length = Buffer_Size;
	Ethernet_Controller_Transmit_Descriptor.Command_And_Checksum_Offset = (1 << ETHERNET_CONTROLLER_TRANSMIT_DESCRIPTOR_COMMAND_REGISTER_END_OF_PACKET_BIT) | (1 << ETHERNET_CONTROLLER_TRANSMIT_DESCRIPTOR_COMMAND_REGISTER_REPORT_STATUS_BIT); // Tell that the packet is fully contained in the descriptor so it can be sent; make the "descriptor done" status bit be set when the packet has been transmitted
	
	// Send the packet
	Pointer_Ethernet_Controller_Registers->Transmit_Descriptor_Head = 0; // Reset transmission buffer descriptors queue
	Pointer_Ethernet_Controller_Registers->Transmit_Descriptor_Tail = 1;
	
	// Wait for the packet to be transmitted
	while (!(Ethernet_Controller_Transmit_Descriptor.Status & (1 << ETHERNET_CONTROLLER_TRANSMIT_DESCRIPTOR_STATUS_REGISTER_DESCRIPTOR_DONE)));
}

int EthernetIsPacketReceived(void)
{
	if (Ethernet_Controller_Receive_Descriptor.Status) return 1;
	return 0;
}

// TODO
//int EthernetGetStatistics()