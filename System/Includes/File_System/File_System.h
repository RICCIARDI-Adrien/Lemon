/** @file File_System.h
 * File system low level routines.
 * @author Adrien RICCIARDI
 * @version 1.0 : 01/08/2012
 * @version 1.1 : 12/09/2012
 * @version 1.2 : 27/12/2012, added cache to file operations.
 * @version 1.3 : 04/04/2013, moved file functions to File module.
 * @version 1.4 : 05/05/2014, used normal arrays to represent the file system data.
 * @version 1.5 : 25/02/2015, renamed the BAT in Blocks List for more consistency.
 */
#ifndef H_FILE_SYSTEM_H
#define H_FILE_SYSTEM_H

#include <Configuration.h>

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Hard disk physical sector size in bytes. */
#define FILE_SYSTEM_SECTOR_SIZE_BYTES 512
/** Tell that a block located in the Blocks List is free. */
#define FILE_SYSTEM_BLOCKS_LIST_BLOCK_FREE 0xFFFFFFFF // This allows only 2^32 - 2 blocks in the Blocks List (taking into account the EOF code too)
/** Tell that the current block is the last of the file. */
#define FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF 0xFFFFFFFE
/** Tell that the Blocks List has no more free block. */
#define FILE_SYSTEM_BLOCKS_LIST_FULL_CODE 0xFFFFFFFF

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** This structure is stored at the end of the MBR sector. It is initialized by system installer and never modified. */
typedef struct __attribute__((packed))
{
	unsigned int Magic_Number; //! Magic number to be sure that a file system is present on the device.
	unsigned int Total_Blocks_Count; //! Total number of blocks in the Blocks List.
	unsigned int Total_Files_Count; //! Total number of files in the Files List (ie maximum number of files allowed).
} TFileSystemInformations;

/** The Files List is an array of this structure. */
typedef struct __attribute__((packed))
{
	char Name[CONFIGURATION_FILE_NAME_LENGTH]; //! The ASCIIZ string storing the file name.
	unsigned int Start_Block; //! ID of the first block of the file. This is the beginning of the chained list into the Blocks List.
	unsigned int Size_Bytes; //! Size of the file in bytes. Yes, maximum file size is limited to 4 GB...
} TFilesListEntry;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
/** Informations about the currently mounted file system. */
extern TFileSystemInformations File_System_Informations;
/** Linked-list of all the blocks holding the files data. */
extern unsigned int Blocks_List[];
/** Contain each existing file name and properties. */
extern TFilesListEntry Files_List[];

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Load file system from the hard disk and initialize it.
 * @return 1 if all operations were done successfully,
 * @return 0 if there were a problem.
 */
int FileSystemInit(void);

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

/** Get the first free block found. 
 * @return The block index value if a free block was found,
 * @return FILE_SYSTEM_BLOCKS_LIST_FULL_CODE if the Blocks List is full.
 */
unsigned int FileSystemGetFirstFreeBlock(void);

/** Read logically chained blocks from the hard disk.
 * @param Start_Block The block to start reading from.
 * @param Blocks_Count How many blocks to read.
 * @return The next block to read (it can be FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF if the end of the file is reached).
 */
unsigned int FileSystemReadBlocks(unsigned int Start_Block, unsigned int Blocks_Count, unsigned char *Pointer_Buffer);

/** Write the buffer content to logical blocks.
 * @param Start_Block The block to start writing to.
 * @param Blocks_Count How many blocks to write.
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
 * @return The allocated block index or ERROR_CODE_BLOCKS_LIST_FULL if there is no more free block.
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