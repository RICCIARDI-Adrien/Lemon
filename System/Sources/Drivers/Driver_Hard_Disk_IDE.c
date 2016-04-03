/** @file Driver_Hard_Disk_IDE.c
 * Raw access to IDE hard disk.
 * @author Adrien RICCIARDI
 */
#include <Architecture.h>
#include <Configuration.h>
#include <Debug.h>
#include <Drivers/Driver_Hard_Disk.h>
#include <Hardware_Functions.h> // To have inb() and outb()

#if CONFIGURATION_HARD_DISK_IS_IDE_DRIVER_ENABLED == 1

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** A standard hard disk sector size in bytes. */
#define HARD_DISK_SECTOR_SIZE 512

/** The port used to read or to write data to the hard disk controller. */
#define HARD_DISK_PORT_DATA 0x01F0
/** Hold the number of sectors to read or to write. */
#define HARD_DISK_PORT_SECTOR_COUNT 0x01F2
/** Low byte of the LBA sector address. */
#define HARD_DISK_PORT_LBA_ADDRESS_LOW 0x01F3
/** Middle byte of the LBA sector address. */
#define HARD_DISK_PORT_LBA_ADDRESS_MIDDLE 0x01F4
/** High byte of the LBA sector address. */
#define HARD_DISK_PORT_LBA_ADDRESS_HIGH 0x01F5
/** Control head and device IDs, and also 4 upper bits of the LBA address. */
#define HARD_DISK_PORT_DEVICE_HEAD 0x01F6
/** Send a command to the hard disk controller. */
#define HARD_DISK_PORT_COMMAND 0x01F7
/** Read the hard disk controller status without sending an interrupt acknowledge. */
#define HARD_DISK_PORT_STATUS 0x03F6

/** Command to start reading with automatic retries in case of failure. */
#define HARD_DISK_COMMAND_READ_WITH_RETRIES 0x20
/** Command to start writing with automatic retries in case of failure. */
#define HARD_DISK_COMMAND_WRITE_WITH_RETRIES 0x30
/** Command to identify the device. */
#define HARD_DISK_COMMAND_IDENTIFY_DEVICE 0xEC

/** Wait until the controller signals it is ready. */
#define WAIT_BUSY_CONTROLLER() while (inb(HARD_DISK_PORT_STATUS) & 0x80)

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Tell which LBA addressing to use. */
static volatile int Hard_Disk_Is_LBA48_Addressing_Used;

/** Keep the hard disk total LBA sectors count. */
static unsigned long long Hard_Disk_LBA_Sectors_Count;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Send the IDENTIFY DEVICE command to the hard disk.
 * @param Pointer_Buffer On output, contain the device parameters. The buffer must be HARD_DISK_SECTOR_SIZE large.
 */
static inline __attribute__((always_inline)) void HardDiskSendIdentifyDeviceCommand(void *Pointer_Buffer)
{
	// Wait for the controller to be ready
	ARCHITECTURE_INTERRUPTS_DISABLE();
	WAIT_BUSY_CONTROLLER();
	
	// Select the master device
	outb(HARD_DISK_PORT_DEVICE_HEAD, 0);
	
	// Send the Identify command
	outb(HARD_DISK_PORT_COMMAND, HARD_DISK_COMMAND_IDENTIFY_DEVICE);
	
	// Wait for read clearance
	WAIT_BUSY_CONTROLLER();
	// Read data
	asm
	(
		"push edi\n"
		"push ecx\n"
		"push edx\n"
		"mov ecx, %0\n" // Sector size is divided by two because we read words from the bus
		"mov edi, %1\n"
		"mov edx, %2\n"
		"rep insw\n"
		"pop edx\n"
		"pop ecx\n"
		"pop edi"
		: // No output
		: "g" (HARD_DISK_SECTOR_SIZE / 2), "g" (Pointer_Buffer), "g" (HARD_DISK_PORT_DATA)
		: "ecx", "edx", "edi"
	);
	
	ARCHITECTURE_INTERRUPTS_ENABLE();
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
// This function is based on the ATA-ATAPI 6 specs
int HardDiskInitialize(void)
{
	unsigned int Buffer[HARD_DISK_SECTOR_SIZE / sizeof(unsigned int)]; // Most parameters are accessed as DWORDs
	
	// Get the device parameters
	HardDiskSendIdentifyDeviceCommand(Buffer);
	
	// Does the device handle LBA ?
	if (!(Buffer[24] & 0x02000000)) return 1;
	
	// Use LBA28 or LBA48 ?
	if (Buffer[30] < 0x0FFFFFFF)
	{
		Hard_Disk_Is_LBA48_Addressing_Used = 0;
		Hard_Disk_LBA_Sectors_Count = Buffer[30];
	}
	else
	{
		// TODO : test for LBA48 support even if it seems overkill
		Hard_Disk_Is_LBA48_Addressing_Used = 1;
		Hard_Disk_LBA_Sectors_Count = ((unsigned long long) Buffer[51] << 32) | Buffer[50];
	}
	
	DEBUG_SECTION_START
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
		ScreenWriteString("Total sectors count : ");
		ScreenWriteString(itoa((unsigned int) Hard_Disk_LBA_Sectors_Count)); // TODO : display the whole sectors count
		ScreenWriteString(", using ");
		if (Hard_Disk_Is_LBA48_Addressing_Used) ScreenWriteString("LBA48");
		else ScreenWriteString("LBA28");
		ScreenWriteString(".\n");
		KeyboardReadCharacter();
	DEBUG_SECTION_END
	
	return 0;
}

void HardDiskReadSector(unsigned int Logical_Sector_Number, void *Pointer_Buffer)
{
	unsigned char *Pointer_Buffer_Bytes = Pointer_Buffer;
	
	// Wait for the controller to be ready
	ARCHITECTURE_INTERRUPTS_DISABLE();
	WAIT_BUSY_CONTROLLER();
	
	if (Hard_Disk_Is_LBA48_Addressing_Used)
	{
		// Select master device and configure for 48-LBA
		outb(HARD_DISK_PORT_DEVICE_HEAD, 0x40);
		
		// Send the sectors to read count (always 1 to avoid issues with the 400 ns delay between sectors)
		outb(HARD_DISK_PORT_SECTOR_COUNT, 0); // High byte
		outb(HARD_DISK_PORT_SECTOR_COUNT, 1); // Low byte
		
		// Send the sector address
		outb(HARD_DISK_PORT_LBA_ADDRESS_LOW, Logical_Sector_Number >> 24); // Byte 3
		outb(HARD_DISK_PORT_LBA_ADDRESS_MIDDLE, 0); // Byte 4 (always 0 as the addresses are 32-bit long)
		outb(HARD_DISK_PORT_LBA_ADDRESS_HIGH, 0); // Byte 5 (always 0 as the addresses are 32-bit long)
		outb(HARD_DISK_PORT_LBA_ADDRESS_LOW, Logical_Sector_Number); // Byte 0
		outb(HARD_DISK_PORT_LBA_ADDRESS_MIDDLE, Logical_Sector_Number >> 8); // Byte 1
		outb(HARD_DISK_PORT_LBA_ADDRESS_HIGH, Logical_Sector_Number >> 16); // Byte 2
	}
	else
	{
		// Select master device and send high LBA address nibble
		outb(HARD_DISK_PORT_DEVICE_HEAD, 0xE0 | ((Logical_Sector_Number >> 24) & 0x0F));
		
		// Send LBA address remaining bytes
		outb(HARD_DISK_PORT_LBA_ADDRESS_HIGH, Logical_Sector_Number >> 16);
		outb(HARD_DISK_PORT_LBA_ADDRESS_MIDDLE, Logical_Sector_Number >> 8);
		outb(HARD_DISK_PORT_LBA_ADDRESS_LOW, Logical_Sector_Number);
		
		// Send the sectors count to read (always 1 to avoid issues with the 400 ns delay between sectors)
		outb(HARD_DISK_PORT_SECTOR_COUNT, 1);
	}
	
	// Send read command with automatic retries
	outb(HARD_DISK_PORT_COMMAND, HARD_DISK_COMMAND_READ_WITH_RETRIES);
	
	// Wait for read clearance
	WAIT_BUSY_CONTROLLER();
	// Read data
	asm
	(
		"push edi\n"
		"push ecx\n"
		"push edx\n"
		"mov ecx, %0\n" // Sector size is divided by two because we read words from the bus
		"mov edi, %1\n"
		"mov edx, %2\n"
		"rep insw\n"
		"pop edx\n"
		"pop ecx\n"
		"pop edi"
		: // No output
		: "g" (HARD_DISK_SECTOR_SIZE / 2), "g" (Pointer_Buffer_Bytes), "g" (HARD_DISK_PORT_DATA)
		: "ecx", "edx", "edi"
	);
	
	ARCHITECTURE_INTERRUPTS_ENABLE();
}

void HardDiskWriteSector(unsigned int Logical_Sector_Number, void *Pointer_Buffer)
{
	unsigned char *Pointer_Buffer_Bytes = Pointer_Buffer;
	
	// Wait for the controller to be ready
	ARCHITECTURE_INTERRUPTS_DISABLE();
	WAIT_BUSY_CONTROLLER();
	
	if (Hard_Disk_Is_LBA48_Addressing_Used)
	{
		// Select master device and configure for 48-LBA
		outb(HARD_DISK_PORT_DEVICE_HEAD, 0x40);
		
		// Send the sectors to write count (always 1 to avoid issues with the 400 ns delay between sectors)
		outb(HARD_DISK_PORT_SECTOR_COUNT, 0); // High byte
		outb(HARD_DISK_PORT_SECTOR_COUNT, 1); // Low byte
		
		// Send the sector address
		outb(HARD_DISK_PORT_LBA_ADDRESS_LOW, Logical_Sector_Number >> 24); // Byte 3
		outb(HARD_DISK_PORT_LBA_ADDRESS_MIDDLE, 0); // Byte 4 (always 0 as the addresses are 32-bit long)
		outb(HARD_DISK_PORT_LBA_ADDRESS_HIGH, 0); // Byte 5 (always 0 as the addresses are 32-bit long)
		outb(HARD_DISK_PORT_LBA_ADDRESS_LOW, Logical_Sector_Number); // Byte 0
		outb(HARD_DISK_PORT_LBA_ADDRESS_MIDDLE, Logical_Sector_Number >> 8); // Byte 1
		outb(HARD_DISK_PORT_LBA_ADDRESS_HIGH, Logical_Sector_Number >> 16); // Byte 2
	}
	else
	{
		// Select master device and send high LBA address nibble
		outb(HARD_DISK_PORT_DEVICE_HEAD, 0xE0 | ((Logical_Sector_Number >> 24) & 0x0F));
		
		// Send LBA address remaining bytes
		outb(HARD_DISK_PORT_LBA_ADDRESS_HIGH, Logical_Sector_Number >> 16);
		outb(HARD_DISK_PORT_LBA_ADDRESS_MIDDLE, Logical_Sector_Number >> 8);
		outb(HARD_DISK_PORT_LBA_ADDRESS_LOW, Logical_Sector_Number);
		
		// Send the sectors count to write (always 1 to avoid issues with the 400 ns delay between sectors)
		outb(HARD_DISK_PORT_SECTOR_COUNT, 1);
	}
	
	// Send write command with automatic retries
	outb(HARD_DISK_PORT_COMMAND, HARD_DISK_COMMAND_WRITE_WITH_RETRIES);
	
	// Wait for write clearance
	WAIT_BUSY_CONTROLLER();
	// Write data
	asm
	(
		"push esi\n"
		"push ecx\n"
		"push edx\n"
		"mov ecx, %0\n" // Sector size is divided by two because we write words to the bus
		"mov esi, %1\n"
		"mov edx, %2\n"
		"rep outsw\n"
		"pop edx\n"
		"pop ecx\n"
		"pop esi"
		: // No output
		: "g" (HARD_DISK_SECTOR_SIZE / 2), "g" (Pointer_Buffer_Bytes), "g" (HARD_DISK_PORT_DATA)
		: "ecx", "edx", "esi"
	);
	
	ARCHITECTURE_INTERRUPTS_ENABLE();
}

unsigned int HardDiskGetDriveSizeSectors(void)
{
	// TODO : handle the LBA48 48-bit value
	return (unsigned int) Hard_Disk_LBA_Sectors_Count;
}

#endif