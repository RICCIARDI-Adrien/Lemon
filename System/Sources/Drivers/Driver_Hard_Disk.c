/** @file Driver_Hard_Disk.c
 * @see Driver_Hard_Disk.h for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Debug.h>
#include <Drivers/Driver_Hard_Disk.h>
#include <Hardware_Functions.h> // To have inb() and outb()

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
// Public functions
//-------------------------------------------------------------------------------------------------
void HardDiskReadSector(unsigned int Logical_Sector_Number, void *Pointer_Buffer)
{
	unsigned char *Pointer_Buffer_Bytes = Pointer_Buffer;
	
	// Wait for the controller to be ready
	asm("cli");
	WAIT_BUSY_CONTROLLER();
	
	#if CONFIGURATION_HARD_DISK_ADDRESSING_IS_LBA48 == 1
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
	#else
		// Select master device and send high LBA address nibble
		outb(HARD_DISK_PORT_DEVICE_HEAD, 0xE0 | ((Logical_Sector_Number >> 24) & 0x0F));
		
		// Send LBA address remaining bytes
		outb(HARD_DISK_PORT_LBA_ADDRESS_HIGH, Logical_Sector_Number >> 16);
		outb(HARD_DISK_PORT_LBA_ADDRESS_MIDDLE, Logical_Sector_Number >> 8);
		outb(HARD_DISK_PORT_LBA_ADDRESS_LOW, Logical_Sector_Number);
		
		// Send the sectors count to read (always 1 to avoid issues with the 400 ns delay between sectors)
		outb(HARD_DISK_PORT_SECTOR_COUNT, 1);
	#endif
	
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
		"pop edi\n"
		"sti"
		: // No output
		: "g" (HARD_DISK_SECTOR_SIZE / 2), "g" (Pointer_Buffer_Bytes), "g" (HARD_DISK_PORT_DATA)
		: "ecx", "edx", "edi"
	);
}

void HardDiskWriteSector(unsigned int Logical_Sector_Number, void *Pointer_Buffer)
{
	unsigned char *Pointer_Buffer_Bytes = Pointer_Buffer;
	
	// Wait for the controller to be ready
	asm("cli");
	WAIT_BUSY_CONTROLLER();
	
	#if CONFIGURATION_HARD_DISK_ADDRESSING_IS_LBA48 == 1
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
	#else
		// Select master device and send high LBA address nibble
		outb(HARD_DISK_PORT_DEVICE_HEAD, 0xE0 | ((Logical_Sector_Number >> 24) & 0x0F));
		
		// Send LBA address remaining bytes
		outb(HARD_DISK_PORT_LBA_ADDRESS_HIGH, Logical_Sector_Number >> 16);
		outb(HARD_DISK_PORT_LBA_ADDRESS_MIDDLE, Logical_Sector_Number >> 8);
		outb(HARD_DISK_PORT_LBA_ADDRESS_LOW, Logical_Sector_Number);
		
		// Send the sectors count to write (always 1 to avoid issues with the 400 ns delay between sectors)
		outb(HARD_DISK_PORT_SECTOR_COUNT, 1);
	#endif
	
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
		"pop esi\n"
		"sti"
		: // No output
		: "g" (HARD_DISK_SECTOR_SIZE / 2), "g" (Pointer_Buffer_Bytes), "g" (HARD_DISK_PORT_DATA)
		: "ecx", "edx", "esi"
	);
}

unsigned int HardDiskGetDriveSizeSectors(void)
{
	unsigned int Buffer[HARD_DISK_SECTOR_SIZE / sizeof(unsigned int)]; // Store a whole sector
	#if CONFIGURATION_HARD_DISK_ADDRESSING_IS_LBA48 == 1
		const int LBA_Total_Sectors_Count_Index = 50; // The total number of sectors is located in the words 100 to 103 (TODO handle the 8-byte value)
	#else
		const int LBA_Total_Sectors_Count_Index = 30; // The total number of sectors is located in the words 60 and 61
	#endif
	
	// Wait for the controller to be ready
	asm("cli");
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
		"pop edi\n"
		"sti"
		: // No output
		: "g" (HARD_DISK_SECTOR_SIZE / 2), "g" (Buffer), "g" (HARD_DISK_PORT_DATA)
		: "ecx", "edx", "edi"
	);
	
	DEBUG_SECTION_START
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
		ScreenWriteString("Total sectors count : ");
		ScreenWriteString(itoa(Buffer[LBA_Total_Sectors_Count_Index]));
		ScreenWriteCharacter('\n');
	DEBUG_SECTION_END

	// TODO : handle the 48-bit return value of LBA48 (the return value is currently limited at hard disk of 2TB)
	return Buffer[LBA_Total_Sectors_Count_Index];
}