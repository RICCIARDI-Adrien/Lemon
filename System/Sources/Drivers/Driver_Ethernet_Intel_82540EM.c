/** @file Driver_Ethernet_Controller_Intel_82540EM.c
 * Intel 82540EM Gigabit ethernet controller (also known as Intel PRO/1000) driver.
 * @author Adrien RICCIARDI
 */
#include <Debug.h>
#include <Drivers/Driver_Ethernet.h>
#include <Drivers/Driver_PCI.h>

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** Ethernet controller registers map. */
typedef struct __attribute__((packed))
{
	unsigned int Device_Control; // 0x0
	unsigned int Device_Control_High; // Not used in 32 bits mode
	unsigned int Status;
	unsigned int Status_High; // Not used in 32 bits mode
	unsigned int EEPROM_Control_Data; //<! EEPROM or Flash control and data.
	unsigned int EEPROM_Control_Data_High; // Not used in 32 bits mode
	unsigned int Reserved_1[2]; //<! Stands for Flash Access register on 82541xx and 82547GI/EI only.
	unsigned int Extended_Device_Control;
	unsigned int Extended_Device_Control_High; // Not used in 32 bits mode
	unsigned int MDI_Control;
	unsigned int MDI_Control_High; // Not used in 32 bits mode
	unsigned int Reserved_2[6]; //<! Flow control registers (not used by this driver).
	
	
	unsigned int Reserved_3[(0x5400 - (18 * 4)) / sizeof(unsigned int)];
	// Offset 0x5400
	unsigned int Receive_Addresses[32]; //<! There are 16 slots available. First slot is board MAC address, other slots are used as filters. All addresses are stored consecutively.
	// TODO
} TEthernetControllerRegisters;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The controller registers. */
static TEthernetControllerRegisters *Pointer_Ethernet_Controller_Registers;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int EthernetInitialize(void)
{
	TPCIDeviceID Device_ID;
	TPCIConfigurationSpaceHeader Configuration_Space_Header;
	
	// Try to find an ethernet controller (can't directly use the device ID to find the device as the device ID is programmed by the board manufacturer with an unknown value)
	if (PCIFindDeviceFromClass(PCI_CLASS_CODE_BASE_NETWORK_CONTROLLER, PCI_CLASS_CODE_SUB_CLASS_ETHERNET, &Device_ID) != 0) return 1;
	PCIGetConfigurationSpaceHeader(&Device_ID, &Configuration_Space_Header);
	
	// Is it an Intel controller ?
	if (Configuration_Space_Header.Vendor_ID != PCI_VENDOR_ID_INTEL) return 2;
	
	// Give access to registers
	Pointer_Ethernet_Controller_Registers = (TEthernetControllerRegisters *) Configuration_Space_Header.Base_Address_Registers[0];
	
	DEBUG_SECTION_START
	DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
	{
		unsigned int MAC_Address_Double_Words[2], i;
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
		KeyboardReadCharacter();
	}
	DEBUG_SECTION_END
	
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