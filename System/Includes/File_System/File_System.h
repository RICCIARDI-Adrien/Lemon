/** @file File_System.h
 * File system low level routines.
 * @author Adrien RICCIARDI
 * @version 1.0 : 01/08/2012
 * @version 1.1 : 12/09/2012
 * @version 1.2 : 27/12/2012, added cache to file operations.
 * @version 1.3 : 04/04/2013, moved file functions to File module.
 * @version 1.4 : 05/05/2014, used normal arrays to represent the file system data.
 */
#ifndef H_FILE_SYSTEM_H
#define H_FILE_SYSTEM_H

#include <Configuration.h>

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Hard disk physical sector size in bytes. */
#define FILE_SYSTEM_SECTOR_SIZE_BYTES 512
/** Hard disk logical block size in bytes. */
#define FILE_SYSTEM_BLOCK_SIZE_BYTES 4096
/** Tell that a block located by the Block Allocation Table is free. */
#define FILE_SYSTEM_BAT_BLOCK_FREE 0xFFFFFFFF // This allows only 2^32 - 2 blocks in the Blocks Allocation Table
/** Tell that a block located by the Block Allocation Table is the end of the current. */
#define FILE_SYSTEM_BAT_BLOCK_EOF 0xFFFFFFFE
/** Tell that the BAT has no more free block. */
#define FILE_SYSTEM_BAT_FULL_CODE 0xFFFFFFFF

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** This structure is stored at the end of the MBR sector. It is initialized by system installer and never modified. */
typedef struct __attribute__((packed))
{
	unsigned int Magic_Number; //! Magic number to be sure that a file system is present on the device.
	unsigned int Total_Blocks_Count; //! Total number of blocks in the Blocks Allocation Table.
	unsigned int Total_Files_Count; //! Total number of files in the Files List (ie maximum number of files allowed).
} TFileSystemInformations;

/** The File List is an array of this structure. */
typedef struct __attribute__((packed))
{
	char Name[CONFIGURATION_FILE_NAME_LENGTH]; //! File name with up to 12 ASCII characters.
	unsigned int Start_Block; //! ID of the first block of the file. This is the beginning of the chained list into the BAT.
	unsigned int Size_Bytes; //! Size of the file in bytes. Yes, maximum file size is limited to 4 GB...
} TFileListEntry;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
/** Informations about the currently mounted file system. */
extern TFileSystemInformations File_System_Informations;
/** Linked-list of all the blocks holding the files data. */
extern unsigned int Block_Allocation_Table[];
/** Contain each existing file name and properties. */
extern TFileListEntry Files_List[];

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Load file system from the hard disk and initialize it.
 * @return 1 if all operations were done successfully,
 * @return 0 if there were a problem.
 */
int FileSystemInit(void);

/** Copy BAT and FL to disk. */
void FileSystemSave(void);

/** Get the free blocks count.
 * @return The free blocks count.
 */
unsigned int FileSystemGetFreeBlocksCount(void);

/** Get the free FL entries count.
 * @return The free FL entries count.
 */
unsigned int FileSystemGetFreeFileListEntriesCount(void);

/** Get the first free block found. 
 * @return The block index value if a free block was found,
 * @return FILE_SYSTEM_BAT_FULL_CODE if the BAT is full.
 */
unsigned int FileSystemGetFirstFreeBlock(void);

/** Read Blocks_Count blocks from Starting_Block and store data to Buffer.
 * @param Start_Block The block to start reading from.
 * @param Blocks_Count How many blocks to read.
 * @return The next block to read (it can be FILE_SYSTEM_BAT_BLOCK_EOF if end of file is reached).
 */
unsigned int FileSystemReadBlocks(unsigned int Start_Block, unsigned int Blocks_Count, unsigned char *Pointer_Buffer);

/** Write Blocks_Count blocks from Starting_Block and store data from Buffer.
 * @param Start_Block The block to start writing to.
 * @param Blocks_Count How many blocks to write.
 * @return The last block written.
 * @warning This function does not check if the BAT is full or not, you must be sure that there are enough free blocks to write the data before calling this function.
 * @note This function automatically adds the EOF code to the last BAT written block.
 */
unsigned int FileSystemWriteBlocks(unsigned int Start_Block, unsigned int Blocks_Count, unsigned char *Pointer_Buffer);

/** Create a new entry in the FL.
 * @param String_File_Name The name of the file to be created.
 * @param Pointer_Pointer_New_Entry On output, a pointer on the newly created FL entry.
 * @return ERROR_CODE_NO_ERROR if no error happened,
 * @return ERROR_CODE_FL_FULL if there is no room left in FL.
 */
int FileSystemWriteFLEntry(char *String_File_Name, TFileListEntry **Pointer_Pointer_New_Entry);

/** Try to find the FL entry corresponding to the provided file name.
 * @param String_File_Name The file to search.
 * @return A pointer on the FL entry if it was found,
 * @return NULL if the requested file was not found.
 */
TFileListEntry *FileSystemReadFLEntry(char *String_File_Name);

/** "Reserve" a free block by writing a false value into it.
 * @return The allocated block index or ERROR_CODE_BAT_FULL if there is no more free block.
 */
unsigned int FileSystemAllocateBlock(void);

/** Create a new file system on the hard disk.
 * @param Blocks_Count Number of blocks on the new file system.
 * @param Files_Count Number of files on the new file system.
 * @param Pointer_MBR_Code The 512-byte MBR code to copy into the first hard disk sector
 * @return 0 if the new file system was successfully created,
 * @return 1 if Blocks_Count and/or Files_Count variables are incoherent values,
 * @return 2 if hard disk size is less than requested file system size.
 * @warning This function overwrites any previously created file system. When this function terminates, the new file system is in use.
 */
int FileSystemCreate(unsigned int Blocks_Count, unsigned int Files_Count, unsigned char *Pointer_MBR_Code);

#endif