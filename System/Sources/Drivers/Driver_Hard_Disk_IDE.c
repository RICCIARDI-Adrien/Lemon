/** @file Driver_Hard_Disk_IDE.c
 * Raw access to IDE hard disk.
 * @author Adrien RICCIARDI
 */
#include <Architecture.h>
#include <Debug.h>
#include <Drivers/Driver_Hard_Disk.h>
#include <Hardware_Functions.h> // To have inb() and outb()

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Select requested drive. */
#ifdef CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_IDE_DRIVE_MASTER
	#define HARD_DISK_IDE_DRIVE_INDEX 0
#else
	#define HARD_DISK_IDE_DRIVE_INDEX 1
#endif

/** The port used to read or to write data to the hard disk controller. */
#ifdef SYSTEM_HARD_DISK_DRIVER_IDE_BUS_PRIMARY
	#define HARD_DISK_PORT_DATA 0x01F0
#else
	#define HARD_DISK_PORT_DATA 0x0170
#endif
/** Hold the number of sectors to read or to write. */
#ifdef SYSTEM_HARD_DISK_DRIVER_IDE_BUS_PRIMARY
	#define HARD_DISK_PORT_SECTOR_COUNT 0x01F2
#else
	#define HARD_DISK_PORT_SECTOR_COUNT 0x0172
#endif
/** Low byte of the LBA sector address. */
#ifdef SYSTEM_HARD_DISK_DRIVER_IDE_BUS_PRIMARY
	#define HARD_DISK_PORT_LBA_ADDRESS_LOW 0x01F3
#else
	#define HARD_DISK_PORT_LBA_ADDRESS_LOW 0x0173
#endif
/** Middle byte of the LBA sector address. */
#ifdef SYSTEM_HARD_DISK_DRIVER_IDE_BUS_PRIMARY
	#define HARD_DISK_PORT_LBA_ADDRESS_MIDDLE 0x01F4
#else
	#define HARD_DISK_PORT_LBA_ADDRESS_MIDDLE 0x0174
#endif
/** High byte of the LBA sector address. */
#ifdef SYSTEM_HARD_DISK_DRIVER_IDE_BUS_PRIMARY
	#define HARD_DISK_PORT_LBA_ADDRESS_HIGH 0x01F5
#else
	#define HARD_DISK_PORT_LBA_ADDRESS_HIGH 0x0175
#endif
/** Control head and device IDs, and also 4 upper bits of the LBA address. */
#ifdef SYSTEM_HARD_DISK_DRIVER_IDE_BUS_PRIMARY
	#define HARD_DISK_PORT_DEVICE_HEAD 0x01F6
#else
	#define HARD_DISK_PORT_DEVICE_HEAD 0x0176
#endif
/** Send a command to the hard disk controller. */
#ifdef SYSTEM_HARD_DISK_DRIVER_IDE_BUS_PRIMARY
	#define HARD_DISK_PORT_COMMAND 0x01F7
#else
	#define HARD_DISK_PORT_COMMAND 0x0177
#endif
/** Read the hard disk controller status without sending an interrupt acknowledge. */
#ifdef SYSTEM_HARD_DISK_DRIVER_IDE_BUS_PRIMARY
	#define HARD_DISK_PORT_STATUS 0x03F6
#else
	#define HARD_DISK_PORT_STATUS 0x0376
#endif

/** Command to start reading with automatic retries in case of failure. */
#define HARD_DISK_COMMAND_READ_WITH_RETRIES 0x20
/** Command to start writing with automatic retries in case of failure. */
#define HARD_DISK_COMMAND_WRITE_WITH_RETRIES 0x30
/** Command to identify the device. */
#define HARD_DISK_COMMAND_IDENTIFY_DEVICE 0xEC

/** Wait until the controller signals it is ready. */
#define WAIT_BUSY_CONTROLLER() \
{ \
	while (inb(HARD_DISK_PORT_STATUS) & 0x80); \
}

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** Useful fields of ATA "IDENTIFY DEVICE" command answer. */
typedef struct __attribute__((packed))
{
	unsigned short Padding_0[27];
	char String_Model_Number[40]; //!< Words 27..46 in ATA specification. Warning : this string is padded with spaces but not zero-terminated.
	unsigned short Padding_1[2];
	unsigned int Capabilities; //!< Words 49..50 in ATA specification.
	unsigned short Padding_2[9];
	unsigned int LBA_28_Maximum_Addressable_Logical_Sectors_Count; //!< Words 60..61 in ATA specification.
	unsigned short Padding_3[38];
	unsigned long long LBA_48_Maximum_Addressable_Logical_Sectors_Count; //!< Words 100..103 in ATA specification, available only when drive supports LBA-48 mode.
	unsigned short Padding_4[152];
} THardDiskIdentifyDeviceAnswer;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Keep the hard disk total LBA sectors count. */
static unsigned long long Hard_Disk_LBA_Sectors_Count;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Send the IDENTIFY DEVICE command to the hard disk.
 * @param Pointer_Buffer On output, contain the device parameters. The buffer must be HARD_DISK_SECTOR_SIZE large.
 */
static inline __attribute__((always_inline)) void HardDiskSendIdentifyDeviceCommand(THardDiskIdentifyDeviceAnswer *Pointer_Identify_Device_Answer)
{
	// Wait for the controller to be ready
	ARCHITECTURE_INTERRUPTS_DISABLE();
	WAIT_BUSY_CONTROLLER();
	
	// Select the master device
	outb(HARD_DISK_PORT_DEVICE_HEAD, HARD_DISK_IDE_DRIVE_INDEX << 4);
	
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
		: "g" (HARD_DISK_SECTOR_SIZE / 2), "g" (Pointer_Identify_Device_Answer), "g" (HARD_DISK_PORT_DATA)
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
	THardDiskIdentifyDeviceAnswer Identify_Device_Answer;
	
	// Get the device parameters
	HardDiskSendIdentifyDeviceCommand(&Identify_Device_Answer);
	
	// Does the device handle LBA ?
	if (!(Identify_Device_Answer.Capabilities & 0x0000200)) return 1;
	
	#ifdef CONFIGURATION_SYSTEM_HARD_DISK_LOGICAL_BLOCK_ADDRESSING_MODE_28
		Hard_Disk_LBA_Sectors_Count = Identify_Device_Answer.LBA_28_Maximum_Addressable_Logical_Sectors_Count;
	#else
		Hard_Disk_LBA_Sectors_Count = Identify_Device_Answer.LBA_48_Maximum_Addressable_Logical_Sectors_Count;
	#endif
	
	DEBUG_SECTION_START
	{
		unsigned int i;
		
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
		
		// Display model number
		ScreenWriteString("Hard disk model : ");
		for (i = 0; i < sizeof(Identify_Device_Answer.String_Model_Number); i += 2) // String characters are swapped due to little-endian words storage
		{
			ScreenWriteCharacter(Identify_Device_Answer.String_Model_Number[i + 1]);
			ScreenWriteCharacter(Identify_Device_Answer.String_Model_Number[i]);
		}
		
		// Display sectors count
		ScreenWriteString("\nTotal sectors count : ");
		ScreenWriteString(itoa((unsigned int) Hard_Disk_LBA_Sectors_Count)); // TODO : display the whole sectors count
		ScreenWriteString(".\n");
		
		KeyboardReadCharacter();
	}
	DEBUG_SECTION_END
	
	return 0;
}

void HardDiskReadSector(unsigned int Logical_Sector_Number, void *Pointer_Buffer)
{
	unsigned char *Pointer_Buffer_Bytes = Pointer_Buffer;
	
	// Wait for the controller to be ready
	ARCHITECTURE_INTERRUPTS_DISABLE();
	WAIT_BUSY_CONTROLLER();
	
	#ifdef CONFIGURATION_SYSTEM_HARD_DISK_LOGICAL_BLOCK_ADDRESSING_MODE_28
		// Select master device and send high LBA address nibble
		outb(HARD_DISK_PORT_DEVICE_HEAD, 0xE0 | (HARD_DISK_IDE_DRIVE_INDEX << 4) | ((Logical_Sector_Number >> 24) & 0x0F));
		
		// Send LBA address remaining bytes
		outb(HARD_DISK_PORT_LBA_ADDRESS_HIGH, Logical_Sector_Number >> 16);
		outb(HARD_DISK_PORT_LBA_ADDRESS_MIDDLE, Logical_Sector_Number >> 8);
		outb(HARD_DISK_PORT_LBA_ADDRESS_LOW, Logical_Sector_Number);
		
		// Send the sectors count to read (always 1 to avoid issues with the 400 ns delay between sectors)
		outb(HARD_DISK_PORT_SECTOR_COUNT, 1);
	#else
		// Select master device and configure for 48-LBA
		outb(HARD_DISK_PORT_DEVICE_HEAD, 0x40 | (HARD_DISK_IDE_DRIVE_INDEX << 4));
		
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
	
	#ifdef CONFIGURATION_SYSTEM_HARD_DISK_LOGICAL_BLOCK_ADDRESSING_MODE_28
		// Select master device and send high LBA address nibble
		outb(HARD_DISK_PORT_DEVICE_HEAD, 0xE0 | (HARD_DISK_IDE_DRIVE_INDEX << 4) | ((Logical_Sector_Number >> 24) & 0x0F));
		
		// Send LBA address remaining bytes
		outb(HARD_DISK_PORT_LBA_ADDRESS_HIGH, Logical_Sector_Number >> 16);
		outb(HARD_DISK_PORT_LBA_ADDRESS_MIDDLE, Logical_Sector_Number >> 8);
		outb(HARD_DISK_PORT_LBA_ADDRESS_LOW, Logical_Sector_Number);
		
		// Send the sectors count to write (always 1 to avoid issues with the 400 ns delay between sectors)
		outb(HARD_DISK_PORT_SECTOR_COUNT, 1);
	#else
		// Select master device and configure for 48-LBA
		outb(HARD_DISK_PORT_DEVICE_HEAD, 0x40 | (HARD_DISK_IDE_DRIVE_INDEX << 4));
		
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