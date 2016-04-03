/** @file Configuration.h
 * Gather all system configurable parameters.
 * @author Adrien RICCIARDI
 */
#ifndef H_CONFIGURATION_H
#define H_CONFIGURATION_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How many file names can be stored in the Files List. */
#define CONFIGURATION_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES 128
/** How many blocks the file system can address. */
#define CONFIGURATION_FILE_SYSTEM_MAXIMUM_BLOCKS_LIST_ENTRIES 2048
/** The size of a file system block in bytes (must be a multiple of 512). */
#define CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES 4096
/** The file system is stored just behind the kernel on the hard disk. This value is in LBA addressing mode. */
#define CONFIGURATION_FILE_SYSTEM_STARTING_SECTOR_OFFSET 129 // 1 sector for the MBR + 128 sectors (= 64 KB) for the kernel
/** The system can't open more files than specified here simultaneously. */
#define CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT 8

/** Maximum length of a file name in characters. */
#define CONFIGURATION_FILE_NAME_LENGTH 12

/** Size of the whole RAM that the system can use (in MB). */
#define CONFIGURATION_SYSTEM_TOTAL_RAM_SIZE_MEGA_BYTES 16

/** Enable or not the IDE hard disk driver.
 * @note The SATA hard disk driver must be disabled when the IDE one is enabled.
 */
#define CONFIGURATION_IS_HARD_DISK_IDE_DRIVER_ENABLED 1

/** Enable or not the SATA hard disk driver (this driver contains a PCI AHCI driver too).
 * @note The PCI subsystem will be automatically enabled for this driver to work.
 * @note The IDE hard disk driver must be disabled when the SATA one is enabled.
 */
#define CONFIGURATION_IS_HARD_DISK_SATA_DRIVER_ENABLED 0

/** Enable or disable debug informations. */
#define CONFIGURATION_IS_DEBUG_ENABLED 0

// Automatic symbol definition according to needs
// Enable or not the PCI subsystem
#if CONFIGURATION_IS_HARD_DISK_SATA_DRIVER_ENABLED == 1
	#define CONFIGURATION_IS_PCI_DRIVER_ENABLED 1
#else
	#define CONFIGURATION_IS_PCI_DRIVER_ENABLED 0
#endif

#endif