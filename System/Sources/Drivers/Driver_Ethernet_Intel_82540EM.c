/** @file Driver_Ethernet_Controller_Intel_82540EM.c
 * Intel 82540EM Gigabit ethernet controller (also known as Intel PRO/1000) driver.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Debug.h>
#include <Drivers/Driver_Ethernet.h>
#include <Drivers/Driver_PCI.h>

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
	
	unsigned int Reserved_2[38];
	
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
	
	unsigned char Reserved_5[0x2800 - 0x104];
	
	// Offset 0x2800
	void *Pointer_Receive_Descriptor_Base_Address_Low;
	// Offset 0x2804
	void *Pointer_Receive_Descriptor_Base_Address_High;
	// Offset 0x2808
	unsigned int Receive_Descriptors_Length;
	unsigned int Receive_Descriptors_Length_High;
	// Offset 0x2810
	unsigned int Receive_Descriptor_Head;
	unsigned int Receive_Descriptor_Head_High;
	// Offset 0x2818
	unsigned int Receive_Descriptor_Tail;
	unsigned int Receive_Descriptor_Tail_High;
	// Offset 0x2820
	unsigned int Receive_Delay_Timer; //<! Set to zero to generate an interrupt each time a packet is received.
	
	unsigned char Reserved_6[0x5200 - 0x2824];
	
	// Offset 0x5200
	unsigned int Multicast_Table_Array_Entries[128];
	// Offset 0x5400
	unsigned int Receive_Addresses[32]; //<! There are 16 slots available. First slot is board MAC address, other slots are used as filters. All addresses are stored consecutively.
	// TODO
} TEthernetControllerRegisters;

/** A receive buffer descriptor. */
typedef struct __attribute__((packed))
{
	void *Pointer_Buffer_Address;
	void *Pointer_Buffer_Address_High;
	unsigned short Length;
	unsigned short Packet_Checksum;
	unsigned short Status_And_Errors;
	unsigned short Special;
} TEthernetControllerReceiveDescriptor;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The controller registers. */
static TEthernetControllerRegisters *Pointer_Ethernet_Controller_Registers;

/** The receive buffer descriptors list (currently limited to only one descriptor). */
static TEthernetControllerReceiveDescriptor __attribute__((aligned(16))) Ethernet_Controller_Receive_Descriptor; // Must be aligned on an Intel paragraph

/** The buffer dedicated to packets reception. */
static unsigned char Ethernet_Controller_Reception_Buffer[CONFIGURATION_ETHERNET_BUFFER_SIZE];
/** The buffer dedicated to packets transmission. */
//static unsigned char Ethernet_Controller_Transmission_Buffer[CONFIGURATION_ETHERNET_BUFFER_SIZE];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int EthernetInitialize(void)
{
	TPCIDeviceID Device_ID;
	TPCIConfigurationSpaceHeader Configuration_Space_Header;
	unsigned int i;
	
	// Try to find an ethernet controller (can't directly use the device ID to find the device as the device ID is programmed by the board manufacturer with an unknown value)
	if (PCIFindDeviceFromClass(PCI_CLASS_CODE_BASE_NETWORK_CONTROLLER, PCI_CLASS_CODE_SUB_CLASS_ETHERNET, &Device_ID) != 0) return 1;
	PCIGetConfigurationSpaceHeader(&Device_ID, &Configuration_Space_Header);
	
	// Is it an Intel controller ?
	if (Configuration_Space_Header.Vendor_ID != PCI_VENDOR_ID_INTEL) return 2;
	
	// Give access to registers
	Pointer_Ethernet_Controller_Registers = (TEthernetControllerRegisters *) Configuration_Space_Header.Base_Address_Registers[0];
	
	// General configuration section of the datasheet
	Pointer_Ethernet_Controller_Registers->Device_Control = (1 << ETHERNET_CONTROLLER_DEVICE_CONTROL_REGISTER_SET_LINK_UP_BIT) | (1 << ETHERNET_CONTROLLER_DEVICE_CONTROL_REGISTER_AUTO_SPEED_DETECTION_ENABLE_BIT);
	
	// Receive initialization section of the datasheet
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
	Pointer_Ethernet_Controller_Registers->Receive_Descriptors_Length = sizeof(Ethernet_Controller_Receive_Descriptor) << 6; // The value must be 128-bit aligned
	// Select the only available descriptor
	Pointer_Ethernet_Controller_Registers->Receive_Descriptor_Head = 0;
	Pointer_Ethernet_Controller_Registers->Receive_Descriptor_Tail = 0;
	
	// Configure the receive descriptor
	Ethernet_Controller_Receive_Descriptor.Pointer_Buffer_Address = Ethernet_Controller_Reception_Buffer;
	
	// Configure the reception behaviour
	// TODO switch with available buffer values or always use 2048 ?
	Pointer_Ethernet_Controller_Registers->Receive_Control = 1 << ETHERNET_CONTROLLER_RECEIVE_CONTROL_REGISTER_BROADCAST_ACCEPT_MODE_BIT;
	Pointer_Ethernet_Controller_Registers->Receive_Delay_Timer = 0; // Generate an interrupt each time a packet is received
	
	// The controller MAC address is loaded from the board EEPROM, no need to set it. Display it at the initialization end to make sure the field was not overwritten by previous initialization
	DEBUG_SECTION_START
	DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
	{
		unsigned int MAC_Address_Double_Words[2];
		unsigned char MAC_Address_Bytes[6];
		
		// Get the MAC address (this PCI registers must be read using 32-bit accesses)
		MAC_Address_Double_Words[0] = Pointer_Ethernet_Controller_Registers->Receive_Addresses[0];
		MAC_Address_Double_Words[1] = Pointer_Ethernet_Controller_Registers->Receive_Addresses[1];
		
		// Put bytes in displayable order
		MAC_Address_Bytes[0] = (unsigned char) MAC_Address_Double_Words[0];
		MAC_Address_Bytes[1] = MAC_Address_Double_Words[0] >> 8;
		MAC_Address_Bytes[2] = MAC_Address_Double_Words[0] >> 16;
		MAC_Address_Bytes[3] = MAC_Address_Double_Words[0] >> 24;
		MAC_Address_Bytes[4] = (unsigned char) MAC_Address_Double_Words[1];
		MAC_Address_Bytes[5] = MAC_Address_Double_Words[1] >> 8;
		
		// Display MAC address
		ScreenWriteString("MAC address : ");
		for (i = 0; i < sizeof(MAC_Address_Bytes); i++)
		{
			DebugWriteHexadecimalByte(MAC_Address_Bytes[i]);
			if (i < sizeof(MAC_Address_Bytes) - 1) ScreenWriteCharacter(':');
		}
		ScreenWriteCharacter('\n');
	}
	DEBUG_SECTION_END
	
	// Enable reception
	Pointer_Ethernet_Controller_Registers->Receive_Control |= 1 << ETHERNET_CONTROLLER_RECEIVE_CONTROL_REGISTER_RECEIVER_ENABLE_BIT;
	
	DEBUG_SECTION_START
	DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
	{
		unsigned int Device_Status;
		char *String_Text;
		
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
	
#if 0
	while (1)
	{
		if (Pointer_Ethernet_Controller_Registers->Interrupt_Cause_Read /*& (1 << 7)*/) ScreenWriteString("INT\n");
	}
#endif
	
	return 0;
}

/*int EthernetReceivePacket(unsigned int Buffer_Size, void *Pointer_Buffer)
{
}

int EthernetSendPacket(unsigned int Buffer_Size, void *Pointer_Buffer)
{
}*/

// TODO
//int EthernetGetStatistics()