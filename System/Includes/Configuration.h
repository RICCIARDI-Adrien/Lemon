/** @file Configuration.h
 * Gather all system configurable parameters.
 * @author Adrien RICCIARDI
 * @version 1.0 : 05/05/2014
 */
#ifndef H_CONFIGURATION_H
#define H_CONFIGURATION_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How many file names can be stored in the File List. */
#define CONFIGURATION_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES 128
/** How many blocks the file system can address. */
#define CONFIGURATION_FILE_SYSTEM_MAXIMUM_BLOCK_ALLOCATION_TABLE_ENTRIES 2048
/** The size of a file system block in bytes (must be a multiple of 512). */
#define CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES 4096
/** The file system is stored just behind the kernel on the hard disk. This value is in LBA addressing mode. */
#define CONFIGURATION_FILE_SYSTEM_STARTING_SECTOR 130 // 1 sector for the MBR + 128 sectors (= 64 KB) for the kernel
/** The system can't open more files than specified here simultaneously. */
#define CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT 8

/** Maximum length of a file name in characters. */
#define CONFIGURATION_FILE_NAME_LENGTH 12

/** Size of the whole RAM that the system can use (in MB). */
#define CONFIGURATION_SYSTEM_TOTAL_RAM_SIZE_MEGA_BYTES 16

#endif