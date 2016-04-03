/** @file Driver_Hard_Disk_SATA.c
 * AHCI SATA hard disk driver.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Debug.h>
#include <Drivers/Driver_Hard_Disk.h>
#include <Drivers/Driver_PCI.h>

#if CONFIGURATION_HARD_DISK_IS_SATA_DRIVER_ENABLED == 1

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** SATA controller Generic Host Control registers.
 * @note Vendor-specific registers are not present.
 */
typedef struct __attribute__((packed))
{
	unsigned int HBA_Capabilities;
	unsigned int Global_HBA_Control;
	unsigned int Interrupt_Status;
	unsigned int Ports_Implemented; //! Bitmask.
	unsigned int AHCI_Version;
} THardDiskSATAGenericHostControlRegisters;

/** SATA controller Port Registers.
 * @note Vendor-specific registers are not present.
 */
typedef struct __attribute__((packed))
{
	unsigned int Command_List_Base_Address;
	unsigned int Command_List_Base_Address_High_Double_Word; //! Only required for 64-bit accesses.
	unsigned int Frame_Information_Structure_Base_Address;
	unsigned int Frame_Information_Structure_Base_Address_High_Double_Word; //! Only required for 64-bit accesses.
	unsigned int Interrupt_Status;
	unsigned int Interrupt_Enable;
	unsigned int Command;
	unsigned int Reserved;
	unsigned int Task_File_Data;
	unsigned int Signature;
	unsigned int SATA_Status;
	unsigned int SATA_Control;
	unsigned int SATA_Error;
	unsigned int SATA_Active;
	unsigned int Command_Issue;
} THardDiskSATAPortRegisters;

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int HardDiskInitialize(void)
{
	TPCIDeviceID SATA_Device_ID;
	TPCIConfigurationSpaceHeader Device_Configuration_Space_Header;
	THardDiskSATAGenericHostControlRegisters *Pointer_Generic_Host_Control_Registers;
	THardDiskSATAPortRegisters *Pointer_Drive_Port_Registers;
	unsigned int Temp_Double_Word;
	
	// Find the SATA controller on the PCI bus
	if (PCIFindDeviceFromClass(PCI_CLASS_CODE_BASE_MASS_STORAGE, PCI_CLASS_CODE_SUB_CLASS_SATA, &SATA_Device_ID) != 0)
	{
		DEBUG_SECTION_START
			DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
			ScreenWriteString("Error : no SATA controller found.\n");
			KeyboardReadCharacter();
		DEBUG_SECTION_END
		return 2;
	}
	
	// Get the SATA controller properties
	if (PCIGetConfigurationSpaceHeader(&SATA_Device_ID, &Device_Configuration_Space_Header) != 0)
	{
		DEBUG_SECTION_START
			DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
			ScreenWriteString("Error : failed to get the SATA controller Configuration Space Header.\n");
			KeyboardReadCharacter();
		DEBUG_SECTION_END
		return 2;
	}
	
	DEBUG_SECTION_START
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
		ScreenWriteString("SATA controller found.\nVendor ID : 0x");
		DebugWriteHexadecimalInteger(Device_Configuration_Space_Header.Vendor_ID);
		ScreenWriteString(", device ID : 0x");
		DebugWriteHexadecimalInteger(Device_Configuration_Space_Header.Device_ID);
		ScreenWriteString(", BAR[5] : 0x");
		DebugWriteHexadecimalInteger(Device_Configuration_Space_Header.Base_Address_Registers[5]);
		ScreenWriteCharacter('\n');
		KeyboardReadCharacter();
	DEBUG_SECTION_END
	
	// Allow access to the generic registers
	Pointer_Generic_Host_Control_Registers = (THardDiskSATAGenericHostControlRegisters *) Device_Configuration_Space_Header.Base_Address_Registers[5];
	DEBUG_SECTION_START
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
		ScreenWriteString("AHCI version : 0x");
		DebugWriteHexadecimalInteger(Pointer_Generic_Host_Control_Registers->AHCI_Version);
		ScreenWriteString("\nports implemented bit mask : 0x");
		DebugWriteHexadecimalInteger(Pointer_Generic_Host_Control_Registers->Ports_Implemented);
		ScreenWriteCharacter('\n');
		KeyboardReadCharacter();
	DEBUG_SECTION_END
	
	// Is the requested drive connected ?
	if (!(Pointer_Generic_Host_Control_Registers->Ports_Implemented & (1 << CONFIGURATION_HARD_DISK_SATA_DRIVE_INDEX)))
	{
		DEBUG_SECTION_START
			DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
			ScreenWriteString("Error : the requested drive is not handled by the controller.\n");
			KeyboardReadCharacter();
		DEBUG_SECTION_END
		return 2;
	}
	
	// Allow access to the drive registers
	Pointer_Drive_Port_Registers = (THardDiskSATAPortRegisters *) (Device_Configuration_Space_Header.Base_Address_Registers[5] + (0x0100 + (CONFIGURATION_HARD_DISK_SATA_DRIVE_INDEX * 128)));
	DEBUG_SECTION_START
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
		ScreenWriteString("Drive SATA status : 0x");
		DebugWriteHexadecimalInteger(Pointer_Drive_Port_Registers->SATA_Status);
		ScreenWriteCharacter('\n');
		KeyboardReadCharacter();
	DEBUG_SECTION_END
	
	// Is the device present and active ?
	Temp_Double_Word = Pointer_Drive_Port_Registers->SATA_Status; // Cache the value
	if (((Temp_Double_Word & 0x0000000F) != 3) || (((Temp_Double_Word >> 4) & 0x0000000F) == 0)) // 3 = "Device presence detected and Phy communication established", 0 = "Device not present or communication not established"
	{
		DEBUG_SECTION_START
			DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
			ScreenWriteString("Error : the requested drive is not detected.\n");
			KeyboardReadCharacter();
		DEBUG_SECTION_END
		return 2;
	}
	if (((Temp_Double_Word >> 8) & 0x0000000F) != 1) // 1 = "Interface in active state"
	{
		DEBUG_SECTION_START
			DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
			ScreenWriteString("Error : the drive interface is not in active state.\n");
			KeyboardReadCharacter();
		DEBUG_SECTION_END
		return 2;
	}
	
	// TODO
	
	return 0;
}

void HardDiskReadSector(unsigned int Logical_Sector_Number, void *Pointer_Buffer)
{
}

void HardDiskWriteSector(unsigned int Logical_Sector_Number, void *Pointer_Buffer)
{
}

unsigned int HardDiskGetDriveSizeSectors(void)
{
}

#endif