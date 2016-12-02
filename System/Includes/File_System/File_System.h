/** @file File_System.h
 * File system low level routines. All the blocks handling is done with virtual block numbers, only FileSystemReadBlocks() and FileSystemWriteBlocks() really access to the physical blocks.
 * @author Adrien RICCIARDI
 */
#ifndef H_FILE_SYSTEM_H
#define H_FILE_SYSTEM_H

#include <Configuration.h>

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Hard disk physical sector size in bytes. */
#define FILE_SYSTEM_SECTOR_SIZE_BYTES 512

/** Tell that the current block is the last of the file. */
#define FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF 0xFFFFFFFF // This allows only 2^32 - 1 blocks in the Blocks List
/** Tell that the Blocks List has no more free block. */
#define FILE_SYSTEM_BLOCKS_LIST_FULL_CODE 0xFFFFFFFF

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** This structure is stored at the file system beginning. It is initialized by system installer and never modified. */
typedef struct __attribute__((packed))
{
	unsigned int Magic_Number; //!< Magic number to be sure that a file system is present on the device.
	unsigned int Total_Blocks_Count; //!< Total number of blocks in the Blocks List.
	unsigned int Total_Files_Count; //!< Total number of files in the Files List (ie maximum number of files allowed).
	unsigned int Free_Blocks_List_Head; //!< The list of empty blocks starts here.
} TFileSystemInformations;

/** The Files List is an array of this structure. */
typedef struct __attribute__((packed))
{
	char String_Name[CONFIGURATION_FILE_NAME_LENGTH]; //!< The ASCIIZ string storing the file name.
	unsigned int Start_Block; //!< ID of the first block of the file. This is the beginning of the chained list into the Blocks List.
	unsigned int Size_Bytes; //!< Size of the file in bytes. Yes, maximum file size is limited to 4 GB...
} TFilesListEntry;

/** Represent a whole file system. */
typedef struct __attribute__((packed))
{
	TFileSystemInformations File_System_Informations; //!< Contain the file system parameters.
	unsigned int Blocks_List[CONFIGURATION_FILE_SYSTEM_MAXIMUM_BLOCKS_LIST_ENTRIES]; //!< All the possible blocks.
	unsigned char Padding_1[FILE_SYSTEM_SECTOR_SIZE_BYTES]; //!< Provide one more sector to allow the blocks list to "overflow". Indeed, if the CONFIGURATION_FILE_SYSTEM_MAXIMUM_BLOCKS_LIST_ENTRIES value is not a multiple of FILE_SYSTEM_SECTOR_SIZE_BYTES, one more sector will be required to store the data.
	TFilesListEntry Files_List[CONFIGURATION_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES]; //!< All the possible files.
	unsigned char Padding_2[FILE_SYSTEM_SECTOR_SIZE_BYTES]; //!< Provide one more sector for the same reason than the previous padding.
} TFileSystem;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
/** The currently mounted file system. */
extern TFileSystem File_System;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Load file system from the hard disk and initialize it.
 * @param Starting_Sector The first LBA sector of the file system.
 * @return 1 if all operations were done successfully,
 * @return 0 if there were a problem.
 */
int FileSystemInitialize(unsigned int Starting_Sector);

/** Copy the Blocks List and the Files List to the disk. */
void FileSystemSave(void);

/** Get the free blocks count.
 * @return The free blocks count.
 */
unsigned int FileSystemGetFreeBlocksCount(void);

/** Get the free Files List entries count.
 * @return The free Files List entries count.
 */
unsigned int FileSystemGetFreeFilesListEntriesCount(void);

/** Read logically chained blocks from the hard disk.
 * @param Start_Block The block to start reading from.
 * @param Blocks_Count How many blocks to read.
 * @param Pointer_Buffer On output, contain the read blocks content.
 * @return The next block to read (it can be FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF if the end of the file is reached).
 */
unsigned int FileSystemReadBlocks(unsigned int Start_Block, unsigned int Blocks_Count, unsigned char *Pointer_Buffer);

/** Write the buffer content to logical blocks.
 * @param Start_Block The block to start writing to.
 * @param Blocks_Count How many blocks to write.
 * @param Pointer_Buffer The data to write.
 * @return The last block written.
 * @warning This function does not check if the Blocks List is full or not, you must be sure that there are enough free blocks to write the data before calling this function.
 * @note This function automatically adds the EOF code to the last Blocks List written block.
 */
unsigned int FileSystemWriteBlocks(unsigned int Start_Block, unsigned int Blocks_Count, unsigned char *Pointer_Buffer);

/** Create a new entry in the Files List.
 * @param String_File_Name The name of the file to be created.
 * @param Pointer_Pointer_New_Entry On output, a pointer on the newly created Files List entry.
 * @return ERROR_CODE_NO_ERROR if no error happened,
 * @return ERROR_CODE_FILES_LIST_FULL if there is no room left in the Files List.
 */
int FileSystemWriteFilesListEntry(char *String_File_Name, TFilesListEntry **Pointer_Pointer_New_Entry);

/** Try to find the Files List entry corresponding to the provided file name.
 * @param String_File_Name The file to search.
 * @return A pointer on the Files List entry if it was found,
 * @return NULL if the requested file was not found.
 */
TFilesListEntry *FileSystemReadFilesListEntry(char *String_File_Name);

/** "Reserve" a free block by writing a false value into it.
 * @return The allocated block index or FILE_SYSTEM_BLOCKS_LIST_FULL_CODE if there is no more free block.
 */
unsigned int FileSystemAllocateBlock(void);

/** Create a new file system on the hard disk.
 * @param Blocks_Count Number of blocks on the new file system.
 * @param Files_Count Number of files on the new file system.
 * @param Starting_Sector The file system starting sector.
 * @return 0 if the new file system was successfully created,
 * @return 1 if Blocks_Count and/or Files_Count variables are incoherent values,
 * @return 2 if hard disk size is less than requested file system size.
 * @warning This function overwrites any previously created file system. When this function terminates, the new file system is in use.
 */
int FileSystemCreate(unsigned int Blocks_Count, unsigned int Files_Count, unsigned int Starting_Sector);

/** Compute a file system whole size (file system structures plus storage data) in sectors.
 * @param Blocks_Count How many entries in the Blocks List.
 * @param Files_Count How many entries in the Files List.
 * @return The total file system size in sectors.
 */
static inline unsigned int FileSystemComputeSizeSectors(unsigned int Blocks_Count, unsigned int Files_Count)
{
	unsigned int Size;
	
	// Compute the size in bytes
	Size = (Blocks_Count * sizeof(unsigned int)) + (Files_Count * sizeof(TFilesListEntry)) + (Blocks_Count * CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES) + (CONFIGURATION_FILE_SYSTEM_STARTING_SECTOR_OFFSET * FILE_SYSTEM_SECTOR_SIZE_BYTES); // Size of the file system + size of data area + (size of MBR + size of kernel)
	// Add one more sector if the file system size is not a multiple of the sector size
	if (Size % FILE_SYSTEM_SECTOR_SIZE_BYTES != 0) Size += FILE_SYSTEM_SECTOR_SIZE_BYTES;
	// Convert size to sectors
	Size /= FILE_SYSTEM_SECTOR_SIZE_BYTES;
	
	return Size;
}

#endif