/** @file Driver_Hard_Disk_SATA.c
 * AHCI SATA hard disk driver. This driver is based on Serial ATA AHCI 1.0 Specification and Serial ATA Revision 3.0.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Debug.h>
#include <Drivers/Driver_Hard_Disk.h>
#include <Drivers/Driver_PCI.h>
#include <Standard_Functions.h>

#if CONFIGURATION_HARD_DISK_IS_SATA_DRIVER_ENABLED == 1

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Register - Host to Device FIS type. */
#define HARD_DISK_SATA_FRAME_INFORMATION_STRUCTURE_TYPE_REGISTER_HOST_TO_DEVICE 0x27

/** The FIS command to get device properties. */
#define HARD_DISK_SATA_COMMAND_IDENTIFY_DEVICE 0xEC

// Port Registers Command useful bits
/** ST bit. */
#define HARD_DISK_SATA_BIT_PORT_REGISTERS_COMMAND_START 1
/** FRE bit. */
#define HARD_DISK_SATA_BIT_PORT_REGISTERS_COMMAND_FRAME_INFORMATION_STRUCTURE_RECEIVE_ENABLE (1 << 4)
/** FR bit. */
#define HARD_DISK_SATA_BIT_PORT_REGISTERS_COMMAND_FRAME_INFORMATION_STRUCTURE_RECEIVE_RUNNING (1 << 14)
/** CR bit. */
#define HARD_DISK_SATA_BIT_PORT_REGISTERS_COMMAND_COMMAND_LIST_RUNNING (1 << 15)
// Port Registers Task File Data useful bits
/** BSY bit. */
#define HARD_DISK_SATA_BIT_PORT_REGISTERS_TASK_FILE_DATA_BUSY (1 << 7)

// Command List Structure Description Information useful bits
/** W bit. */
#define HARD_DISK_SATA_BIT_COMMAND_LIST_STRUCTURE_DESCRIPTION_INFORMATION_WRITE (1 << 6)
/** P bit. */
#define HARD_DISK_SATA_BIT_COMMAND_LIST_STRUCTURE_DESCRIPTION_INFORMATION_PREFETCHABLE (1 << 7)
/** C bit. */
#define HARD_DISK_SATA_BIT_COMMAND_LIST_STRUCTURE_DESCRIPTION_INFORMATION_CLEAR_BUSY_ON_ACKNOWLEDGE (1 << 10)

// FIS Information Structure Host to Device useful bits
/** C bit. */
#define HARD_DISK_SATA_FRAME_INFORMATION_STRUCTURE_REGISTER_HOST_TO_DEVICE_TRANSFER_WHEN_COMMAND_ISSUE_SET_BIT (1 << 7)

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
	unsigned int Command_Issue; //! Bitmask. Set a bit to '1' to indicate that the corresponding command in the Command List is ready for execution.
} THardDiskSATAPortRegisters;

/** Register - Host to Device FIS. */
typedef struct __attribute__((packed))
{
	unsigned char Frame_Information_Structure_Type; //! Set to 0x27.
	unsigned char Port_Multiplier_Port_And_Transfer_When_Command_Issue_Set_Bit;
	unsigned char Command;
	unsigned char Features_Low_Byte;
	unsigned char LBA_Address_Low_Bytes[3];
	unsigned char Device;
	unsigned char LBA_Address_High_Bytes[3];
	unsigned char Features_High_Byte;
	unsigned short Sectors_Count;
	unsigned char Isochronous_Command_Completion; //! Command execution timeout.
	unsigned char Control;
	unsigned char Reserved_2[4];
} THardDiskSATAFrameInformationStructureRegisterHostToDevice;

/** Register - Device to Host FIS. */
typedef struct __attribute__((packed))
{
	unsigned char Frame_Information_Structure_Type; //! Set to 0x34.
	unsigned char Port_Multiplier_Address: 4;
	unsigned char Reserved_1: 2;
	unsigned char Interrupt_Bit: 1;
	unsigned char Reserved_2: 1;
	unsigned char Status;
	unsigned char Error;
	unsigned char LBA_Address_Low_Bytes[3];
	unsigned char Device;
	unsigned char LBA_Address_High_Bytes[3];
	unsigned char Reserved_3;
	unsigned short Sectors_Count;
	unsigned char Reserved_4[6];
} THardDiskSATAFrameInformationStructureRegisterDeviceToHost;

/** A Command List Structure, which compose a device Command List. */
typedef struct __attribute__((packed))
{
	unsigned int Description_Information;
	unsigned int Command_Status;
	unsigned int Command_Table_Descriptor_Base_Address;
	unsigned int Command_Table_Descriptor_Base_Address_High_Double_Word;
	unsigned int Reserved[4];
} THardDiskSATACommandListStructure;

/** A Physical Region Descriptor Table, containing the payload buffer address and size. */
typedef struct __attribute__((packed))
{
	unsigned int Data_Base_Address;
	unsigned int Data_Base_Address_High_Double_Word;
	unsigned int Reserved_1;
	unsigned int Data_Byte_Count: 22;
	unsigned int Reserved_2: 9;
	unsigned int Interrupt_On_Completion: 1;
} THardDiskSATAPhysicalRegionDescriptorTable;

/** A Command Table, containing a command properties. */
typedef struct __attribute__((packed))
{
	THardDiskSATAFrameInformationStructureRegisterHostToDevice Command_Frame_Information_Structure;
	unsigned char Padding[0x40 - sizeof(THardDiskSATAFrameInformationStructureRegisterHostToDevice)];
	unsigned char ATAPI_Command[32]; //! Not used here.
	unsigned char Reserved[32];
	THardDiskSATAPhysicalRegionDescriptorTable Physical_Region_Descriptor_Table; //! The only PRDT used.
} THardDiskSATACommandTable;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The desired drive registers. */
static volatile THardDiskSATAPortRegisters *Pointer_Hard_Disk_SATA_Drive_Port_Registers;

/** The device Command List. Only the first entry is used because all commands are executed sequentially. */
static volatile THardDiskSATACommandListStructure __attribute__((aligned(1024))) Hard_Disk_SATA_Command_List;

/** The first Command List slot's Command Table. */
static volatile THardDiskSATACommandTable __attribute__((aligned(128))) Hard_Disk_SATA_Command_Table;

// TODO
static volatile unsigned char __attribute__((aligned(256))) FIS[2048]; // sz 256

/** The buffer used for all data transfer. */
static volatile unsigned char __attribute__((aligned(2))) Hard_Disk_SATA_Buffer[HARD_DISK_SECTOR_SIZE];

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Execute the command located in the Command List 0 and wait for its completion. */
static void HardDiskSATAControllerExecuteCommand(void)
{
	// Start executing the command
	Pointer_Hard_Disk_SATA_Drive_Port_Registers->Command_Issue |= 1; // Always execute the first command as there is only one slot in the command list
	
	// Wait for command completion
	while (Pointer_Hard_Disk_SATA_Drive_Port_Registers->Task_File_Data & HARD_DISK_SATA_BIT_PORT_REGISTERS_TASK_FILE_DATA_BUSY);
}

/** Fill the Command List slot 0 needed fields.
 * @param Is_Write_Operation Set to 1 if it is a write operation, set to 0 if it is a read operation.
 * @param Is_Prefetchable Set to 1 if the data is prefetchable (the HBA can optimize the data transfer) or set to 0 if not.
 */
static void HardDiskSATAPrepareCommandListStructure(int Is_Write_Operation, int Is_Prefetchable)
{
	Hard_Disk_SATA_Command_List.Description_Information = (sizeof(Hard_Disk_SATA_Command_Table.Command_Frame_Information_Structure) / 4) & 0x1F; // // Set the Frame Information Structure Length field (bits 4 to 0), the value unit is double-word
	if (Is_Write_Operation) Hard_Disk_SATA_Command_List.Description_Information |= HARD_DISK_SATA_BIT_COMMAND_LIST_STRUCTURE_DESCRIPTION_INFORMATION_WRITE; // Is it a write operation ?
	if (Is_Prefetchable) Hard_Disk_SATA_Command_List.Description_Information |= HARD_DISK_SATA_BIT_COMMAND_LIST_STRUCTURE_DESCRIPTION_INFORMATION_PREFETCHABLE; // Can the data transfer be optimized without looses ?
	Hard_Disk_SATA_Command_List.Description_Information |= HARD_DISK_SATA_BIT_COMMAND_LIST_STRUCTURE_DESCRIPTION_INFORMATION_CLEAR_BUSY_ON_ACKNOWLEDGE; // Clear PxTFD.STS.BSY and PxCI.CI(z) when command is completed
	Hard_Disk_SATA_Command_List.Description_Information |= 1 << 16; // There is always a single entry in the PRDT in this implementation
	
	Hard_Disk_SATA_Command_List.Command_Status = 0; // This field will be incremented with the real transfered bytes amount
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int HardDiskInitialize(void)
{
	TPCIDeviceID SATA_Device_ID;
	TPCIConfigurationSpaceHeader Device_Configuration_Space_Header;
	volatile THardDiskSATAGenericHostControlRegisters *Pointer_Generic_Host_Control_Registers;
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
	Pointer_Generic_Host_Control_Registers->Global_HBA_Control |= 1 << 31; // Enable use of AHCI communication mechanism only
	
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
	Pointer_Hard_Disk_SATA_Drive_Port_Registers = (THardDiskSATAPortRegisters *) (Device_Configuration_Space_Header.Base_Address_Registers[5] + (0x0100 + (CONFIGURATION_HARD_DISK_SATA_DRIVE_INDEX * 128)));
	
	// Initialize the drive
	// Reset PxCMD.ST
	Pointer_Hard_Disk_SATA_Drive_Port_Registers->Command &= ~HARD_DISK_SATA_BIT_PORT_REGISTERS_COMMAND_START;
	while (Pointer_Hard_Disk_SATA_Drive_Port_Registers->Command & HARD_DISK_SATA_BIT_PORT_REGISTERS_COMMAND_COMMAND_LIST_RUNNING);
	// Reset PxCMD.FRE
	if (Pointer_Hard_Disk_SATA_Drive_Port_Registers->Command & HARD_DISK_SATA_BIT_PORT_REGISTERS_COMMAND_FRAME_INFORMATION_STRUCTURE_RECEIVE_ENABLE)
	{
		Pointer_Hard_Disk_SATA_Drive_Port_Registers->Command &= ~HARD_DISK_SATA_BIT_PORT_REGISTERS_COMMAND_FRAME_INFORMATION_STRUCTURE_RECEIVE_ENABLE;
		while (Pointer_Hard_Disk_SATA_Drive_Port_Registers->Command & HARD_DISK_SATA_BIT_PORT_REGISTERS_COMMAND_FRAME_INFORMATION_STRUCTURE_RECEIVE_RUNNING);
	}
	
	DEBUG_SECTION_START
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
		ScreenWriteString("Drive SATA status : 0x");
		DebugWriteHexadecimalInteger(Pointer_Hard_Disk_SATA_Drive_Port_Registers->SATA_Status);
		ScreenWriteCharacter('\n');
		KeyboardReadCharacter();
	DEBUG_SECTION_END
	
	// Is the device present and active ?
	Temp_Double_Word = Pointer_Hard_Disk_SATA_Drive_Port_Registers->SATA_Status; // Cache the value
	if (((Temp_Double_Word & 0x0000000F) != 3) || (((Temp_Double_Word >> 4) & 0x0000000F) == 0)) // 3 = "Device presence detected and Phy communication established", 0 = "Device not present or communication not established"
	{
		DEBUG_SECTION_START
			DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
			ScreenWriteString("Error : the requested drive was not detected.\n");
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
	
	// Configure the Command List
	// Set the Command List address
	Pointer_Hard_Disk_SATA_Drive_Port_Registers->Command_List_Base_Address = (unsigned int) &Hard_Disk_SATA_Command_List;
	Pointer_Hard_Disk_SATA_Drive_Port_Registers->Command_List_Base_Address_High_Double_Word = 0;
	
	// Set the first Command List slot's Command Table address
	Hard_Disk_SATA_Command_List.Command_Table_Descriptor_Base_Address = (unsigned int) &Hard_Disk_SATA_Command_Table;
	Hard_Disk_SATA_Command_List.Command_Table_Descriptor_Base_Address_High_Double_Word = 0;
	
	// Set the command table data address once and for all
	Hard_Disk_SATA_Command_Table.Physical_Region_Descriptor_Table.Data_Base_Address = (unsigned int) Hard_Disk_SATA_Buffer;
	Hard_Disk_SATA_Command_Table.Physical_Region_Descriptor_Table.Data_Base_Address_High_Double_Word = 0;
	Hard_Disk_SATA_Command_Table.Physical_Region_Descriptor_Table.Data_Byte_Count = HARD_DISK_SECTOR_SIZE - 1; // The size is zero-based, so a value of 0 = a size of 1 byte
	
	// TODO
	
	// Set the received FIS address TODO
	Pointer_Hard_Disk_SATA_Drive_Port_Registers->Frame_Information_Structure_Base_Address = (unsigned int) FIS;
	Pointer_Hard_Disk_SATA_Drive_Port_Registers->Frame_Information_Structure_Base_Address_High_Double_Word = 0;
	
	// Clear PxSERR
	Pointer_Hard_Disk_SATA_Drive_Port_Registers->SATA_Error = 0x07FF0F03; // Put ones in all implemented bits (as asked by the specification)
	
	// Start Command Engine (allow the commands to be processed)
	Pointer_Hard_Disk_SATA_Drive_Port_Registers->Command |= HARD_DISK_SATA_BIT_PORT_REGISTERS_COMMAND_START;
	
	DEBUG_SECTION_START
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
		ScreenWriteString("Driver successfully initialized.\n");
		KeyboardReadCharacter();
	DEBUG_SECTION_END
	
	// The first command will fail, so execute a dumb command to make the following work TODO : fix this
	HardDiskGetDriveSizeSectors();
	
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
	unsigned int Sectors_Count;
	
	// Send an IDENTIFY DEVICE ATA command
	
	// Configure the Command List slot
	HardDiskSATAPrepareCommandListStructure(0, 0); // Read operation, do not prefetch data (TODO is data prefetchable ?)
	
	// Create the H2D FIS content
	memset((void *) &Hard_Disk_SATA_Command_Table.Command_Frame_Information_Structure, 0, sizeof(Hard_Disk_SATA_Command_Table.Command_Frame_Information_Structure));
	Hard_Disk_SATA_Command_Table.Command_Frame_Information_Structure.Frame_Information_Structure_Type = HARD_DISK_SATA_FRAME_INFORMATION_STRUCTURE_TYPE_REGISTER_HOST_TO_DEVICE; // Set the FIS type
	Hard_Disk_SATA_Command_Table.Command_Frame_Information_Structure.Command = HARD_DISK_SATA_COMMAND_IDENTIFY_DEVICE; // Set the command
	Hard_Disk_SATA_Command_Table.Command_Frame_Information_Structure.Port_Multiplier_Port_And_Transfer_When_Command_Issue_Set_Bit = HARD_DISK_SATA_FRAME_INFORMATION_STRUCTURE_REGISTER_HOST_TO_DEVICE_TRANSFER_WHEN_COMMAND_ISSUE_SET_BIT; // The command will be executed when the corresponding bit in CI register will be set
	
	// Execute the command and wait for its completion
	HardDiskSATAControllerExecuteCommand();
	
	// Retrieve the sectors count value
	Sectors_Count = (Hard_Disk_SATA_Buffer[203] << 24) | (Hard_Disk_SATA_Buffer[202] << 16) | (Hard_Disk_SATA_Buffer[201] << 8) | Hard_Disk_SATA_Buffer[200];
	DEBUG_SECTION_START
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
		ScreenWriteString("Total sectors count : ");
		ScreenWriteString(itoa(Sectors_Count));
		ScreenWriteCharacter('\n');
		KeyboardReadCharacter();
	DEBUG_SECTION_END
	
	// TODO handle 48-bit value
	return Sectors_Count;
}

#endif