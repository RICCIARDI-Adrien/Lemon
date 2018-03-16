/** @file Configuration.h
 * Gather all system configurable parameters.
 * @author Adrien RICCIARDI
 */
#ifndef H_CONFIGURATION_H
#define H_CONFIGURATION_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
// File system
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

// RAM management
/** The address the MBR is loaded to by the BIOS. */
#define CONFIGURATION_SYSTEM_MBR_LOAD_ADDRESS 0x7C00
/** Kernel stack address. */
#define CONFIGURATION_KERNEL_STACK_ADDRESS 0x10000

// User space definitions
/** Amount of RAM allowed for user space. */
#define CONFIGURATION_USER_SPACE_SIZE ((CONFIGURATION_SYSTEM_TOTAL_RAM_SIZE_MEGA_BYTES - 1) * 1024UL * 1024) // Keep the first megabyte for the kernel; force an unsigned int cast
/** User space base address. This reserves the first MB of RAM for the kernel. */
#define CONFIGURATION_USER_SPACE_ADDRESS 0x100000
/** A program header size. */
#define CONFIGURATION_USER_SPACE_PROGRAM_HEADER_SIZE 4
/** Instruction pointer start address for user program. */
#define CONFIGURATION_USER_SPACE_PROGRAM_ENTRY_POINT 0x100
/** User program will be loaded at this address. 256 bytes are reserved to store command line arguments. */
#define CONFIGURATION_USER_SPACE_PROGRAM_LOAD_ADDRESS (CONFIGURATION_USER_SPACE_ADDRESS + CONFIGURATION_USER_SPACE_PROGRAM_ENTRY_POINT)
/** Executable program magic number. */
#define CONFIGURATION_USER_SPACE_PROGRAM_MAGIC_NUMBER 0x78563412

// Drivers
/** Select which SATA hard disk (from 0 to 31) to use.
 * @note This option is available only when the SATA driver is enabled.
 */
#define CONFIGURATION_HARD_DISK_SATA_DRIVE_INDEX 0

/** Ethernet receive and transmit buffers size in byte (Intel 82540EM available values are 256 bytes, 512 bytes, 1024 bytes and 2048 bytes as jumbo frames are not enabled). As the equipements default MTU is 1500 bytes, 2048 bytes should be fine). */
#define CONFIGURATION_ETHERNET_BUFFER_SIZE 2048 // TODO this value does not change the controller value

//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
// These symbols come from the linker script. Their address must be used, not their value
/** The BSS (variables that value will default to zero) section start (4-byte aligned). */
extern unsigned int Configuration_Memory_BSS_Start_Address;
/** The BSS section end (4-byte aligned). */
extern unsigned int Configuration_Memory_BSS_End_Address;

#endif
