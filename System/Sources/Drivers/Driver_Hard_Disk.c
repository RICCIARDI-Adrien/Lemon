/** @file Driver_Hard_Disk.c
 * @see Driver_Hard_Disk.h for description.
 * @author Adrien RICCIARDI
 */
#include <Drivers/Driver_Hard_Disk.h>
#include <Hardware_Functions.h> // To have inb() and outb()

//-------------------------------------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------------------------------------
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

/** Wait until the controller signals it is ready. */
#define WAIT_BUSY_CONTROLLER() while (inb(HARD_DISK_PORT_STATUS) & 0x80)

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
void HardDiskReadSector(unsigned int Logical_Sector_Number, unsigned char *Pointer_Buffer)
{
	// Wait for the controller to be ready
	asm("cli");
	WAIT_BUSY_CONTROLLER();
	
	// Select master device and send high LBA address nibble
	outb(HARD_DISK_PORT_DEVICE_HEAD, 0xE0 | ((Logical_Sector_Number >> 24) & 0x0F));
	
	// Send LBA address remaining bytes
	outb(HARD_DISK_PORT_LBA_ADDRESS_HIGH, Logical_Sector_Number >> 16);
	outb(HARD_DISK_PORT_LBA_ADDRESS_MIDDLE, Logical_Sector_Number >> 8);
	outb(HARD_DISK_PORT_LBA_ADDRESS_LOW, Logical_Sector_Number);
	
	// Send the sectors count to read (always 1 to avoid issues with the 400ns delay between sectors)
	outb(HARD_DISK_PORT_SECTOR_COUNT, 1);
	
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
		"mov ecx, 256\n" // Sector size is divided by two because we read words from the bus
		"mov edi, %0\n"
		"mov edx, %1\n"
		"rep insw\n"
		"pop edx\n"
		"pop ecx\n"
		"pop edi\n"
		"sti"
		: // No output
		: "m" (Pointer_Buffer), "i" (HARD_DISK_PORT_DATA)
		: "edi", "ecx", "edx"
	);
}

void HardDiskWriteSector(unsigned int Logical_Sector_Number, unsigned char *Pointer_Buffer)
{
	// Wait for the controller to be ready
	asm("cli");
	WAIT_BUSY_CONTROLLER();
	
	// Select master device and send high LBA address nibble
	outb(HARD_DISK_PORT_DEVICE_HEAD, 0xE0 | ((Logical_Sector_Number >> 24) & 0x0F));
	
	// Send LBA address remaining bytes
	outb(HARD_DISK_PORT_LBA_ADDRESS_HIGH, Logical_Sector_Number >> 16);
	outb(HARD_DISK_PORT_LBA_ADDRESS_MIDDLE, Logical_Sector_Number >> 8);
	outb(HARD_DISK_PORT_LBA_ADDRESS_LOW, Logical_Sector_Number);
	
	// Send the sectors count to write (always 1 to avoid issues with the 400ns delay between sectors)
	outb(HARD_DISK_PORT_SECTOR_COUNT, 1);
	
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
		"mov ecx, 256\n" // Sector size is divided by two because we write words to the bus
		"mov esi, %0\n"
		"mov edx, %1\n"
		"rep outsw\n"
		"pop edx\n"
		"pop ecx\n"
		"pop esi\n"
		"sti"
		: // No output
		: "m" (Pointer_Buffer), "i" (HARD_DISK_PORT_DATA)
		: "esi", "ecx", "edx"
	);
}

unsigned long long HardDiskGetDriveSizeBytes(void)
{
	// TODO
	return 64 * 1024 * 1024;
}