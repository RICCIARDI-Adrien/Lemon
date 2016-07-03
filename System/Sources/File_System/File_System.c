/** @file File_System.c
 * @see File_System.h for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Drivers/Driver_Hard_Disk.h>
#include <Error_Codes.h>
#include <File_System/File_System.h>
#include <Kernel.h> // Needed for the NULL definition
#include <Standard_Functions.h>

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Tell if a correct file system is stored on the disk or not. */
#define FILE_SYSTEM_MAGIC_NUMBER 0x12345678

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The Blocks List size in hard disk sectors. */
static unsigned int Blocks_List_Size_Sectors;
/** The ID of the first hard disk sector containing the Blocks List. */
static unsigned int Blocks_List_First_Sector_Number;

/** The Files List size in hard disk sectors. */
static unsigned int Files_List_Size_Sectors;
/** The ID of the first hard disk sector containing the Files List. */
static unsigned int Files_List_First_Sector_Number;

/** First sector dedicated to data, located right after the file system. */
static unsigned int Data_First_Sector_Number;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
TFileSystem File_System;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Read several sectors from the hard disk.
 * @param First_Sector_Number The sector to start reading from.
 * @param Sectors_Count How many sectors to read.
 * @param Pointer_Buffer On output, will contain the read data.
 */
static void FileSystemReadSectors(unsigned int First_Sector_Number, unsigned int Sectors_Count, void *Pointer_Buffer)
{
	while (Sectors_Count > 0)
	{
		HardDiskReadSector(First_Sector_Number, Pointer_Buffer);
		First_Sector_Number++;
		Pointer_Buffer += FILE_SYSTEM_SECTOR_SIZE_BYTES;
		Sectors_Count--;
	}
}

/** Write several sectors to the hard disk.
 * @param First_Sector_Number The sector to start writing to.
 * @param Sectors_Count How many sectors to write.
 * @param Pointer_Buffer The buffer containing the data to write.
 */
static void FileSystemWriteSectors(unsigned int First_Sector_Number, unsigned int Sectors_Count, void *Pointer_Buffer)
{
	while (Sectors_Count > 0)
	{
		HardDiskWriteSector(First_Sector_Number, Pointer_Buffer);
		First_Sector_Number++;
		Pointer_Buffer += FILE_SYSTEM_SECTOR_SIZE_BYTES;
		Sectors_Count--;
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int FileSystemInitialize(unsigned int Starting_Sector)
{
	unsigned int Temp;
	
	// Retrieve file system informations, they are stored at the beginning of the file system
	HardDiskReadSector(Starting_Sector, &File_System);
	
	// Check if there is a valid file system on the device
	if (File_System.File_System_Informations.Magic_Number != FILE_SYSTEM_MAGIC_NUMBER) return 0;
	// Check if the file system is small enough to fit into the kernel reserved memory space
	if ((File_System.File_System_Informations.Total_Blocks_Count > CONFIGURATION_FILE_SYSTEM_MAXIMUM_BLOCKS_LIST_ENTRIES) || (File_System.File_System_Informations.Total_Files_Count > CONFIGURATION_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES)) return 0;
	
	// Compute Blocks List size in sectors
	Temp = File_System.File_System_Informations.Total_Blocks_Count * sizeof(unsigned int) + sizeof(TFileSystemInformations); // Take into account the file system informations to ease their handling
	Blocks_List_Size_Sectors = Temp / FILE_SYSTEM_SECTOR_SIZE_BYTES;
	if (Temp % FILE_SYSTEM_SECTOR_SIZE_BYTES) Blocks_List_Size_Sectors++;
	
	// Compute Files List size in sectors
	Temp = File_System.File_System_Informations.Total_Files_Count * sizeof(TFilesListEntry);
	Files_List_Size_Sectors = Temp / FILE_SYSTEM_SECTOR_SIZE_BYTES;
	if (Temp % FILE_SYSTEM_SECTOR_SIZE_BYTES != 0) Files_List_Size_Sectors++;
	
	// Determine starting sectors for each file system structure
	Blocks_List_First_Sector_Number = Starting_Sector;
	Files_List_First_Sector_Number = Blocks_List_First_Sector_Number + Blocks_List_Size_Sectors;
	Data_First_Sector_Number = Files_List_First_Sector_Number + Files_List_Size_Sectors;
	
	// Load Blocks List and Files List
	FileSystemReadSectors(Blocks_List_First_Sector_Number, Blocks_List_Size_Sectors, &File_System); // The file system informations are reloaded, but this is the easiest way
	FileSystemReadSectors(Files_List_First_Sector_Number, Files_List_Size_Sectors, &File_System.Files_List);

	// No error
	return 1;
}

void FileSystemSave(void)
{
	FileSystemWriteSectors(Blocks_List_First_Sector_Number, Blocks_List_Size_Sectors, &File_System);
	FileSystemWriteSectors(Files_List_First_Sector_Number, Files_List_Size_Sectors, &File_System.Files_List);
}

unsigned int FileSystemGetFreeBlocksCount(void)
{
	unsigned int i, Free_Blocks_Count = 0;
	
	for (i = 0; i < File_System.File_System_Informations.Total_Blocks_Count; i++)
	{
		if (File_System.Blocks_List[i] == FILE_SYSTEM_BLOCKS_LIST_BLOCK_FREE) Free_Blocks_Count++;
	}
	return Free_Blocks_Count;
}

unsigned int FileSystemGetFreeFilesListEntriesCount(void)
{
	unsigned int i, File_List_Entries_Count = 0;
	
	for (i = 0; i < File_System.File_System_Informations.Total_Files_Count; i++)
	{
		if (File_System.Files_List[i].String_Name[0] == 0) File_List_Entries_Count++;
	}
	return File_List_Entries_Count;
}

unsigned int FileSystemGetFirstFreeBlock(void)
{
	unsigned int i;
	
	for (i = 0; i < File_System.File_System_Informations.Total_Blocks_Count; i++)
	{
		if (File_System.Blocks_List[i] == FILE_SYSTEM_BLOCKS_LIST_BLOCK_FREE) return i;
	}
	// No more room in the Blocks List
	return FILE_SYSTEM_BLOCKS_LIST_FULL_CODE;
}

unsigned int FileSystemReadBlocks(unsigned int Start_Block, unsigned int Blocks_Count, unsigned char *Pointer_Buffer)
{
	unsigned int i, j, Block, Sector;
	
	// Is end of file reached ?
	if (Start_Block == FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF) return FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF;
		
	Block = Start_Block;
	for (i = 0; i < Blocks_Count; i++)
	{
		// Read block
		Sector = (Block * (CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES / FILE_SYSTEM_SECTOR_SIZE_BYTES)) + Data_First_Sector_Number;
		for (j = 0; j < CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES / FILE_SYSTEM_SECTOR_SIZE_BYTES; j++)
		{
			HardDiskReadSector(Sector, Pointer_Buffer);
			Sector++;
			Pointer_Buffer += FILE_SYSTEM_SECTOR_SIZE_BYTES;
		}

		// Next block
		Block = File_System.Blocks_List[Block];
		// Tell that the end of file is reached
		if (Block == FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF) break;
	}
	return Block;
}

unsigned int FileSystemWriteBlocks(unsigned int Start_Block, unsigned int Blocks_Count, unsigned char *Pointer_Buffer)
{
	unsigned int i, j, Block, Next_Block, Sector;
	
	Block = Start_Block;
	
	for (i = 0; i < Blocks_Count; i++)
	{
		// Write block
		Sector = (Block * (CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES / FILE_SYSTEM_SECTOR_SIZE_BYTES)) + Data_First_Sector_Number;
		for (j = 0; j < CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES / FILE_SYSTEM_SECTOR_SIZE_BYTES; j++)
		{
			HardDiskWriteSector(Sector, Pointer_Buffer);
			Sector++;
			Pointer_Buffer += FILE_SYSTEM_SECTOR_SIZE_BYTES;
		}
		
		// Write end-of-file in the last block
		if (i == Blocks_Count - 1) File_System.Blocks_List[Block] = FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF;
		else
		{
			// Find next block
			File_System.Blocks_List[Block] = 12; // Set a value to the previously written block in order to avoid getting this block when calling FileSystemGetFirstFreeBlock()
			Next_Block = FileSystemGetFirstFreeBlock();
			// Update Blocks List with next block
			File_System.Blocks_List[Block] = Next_Block;
			Block = Next_Block;
		}
	}
	
	// Last written block
	return Block;
}

TFilesListEntry *FileSystemReadFilesListEntry(char *String_File_Name)
{
	unsigned int i;
	
	// Search for the first matching file name in the Files List
	for (i = 0; i < File_System.File_System_Informations.Total_Files_Count; i++)
	{
		if (strncmp(String_File_Name, File_System.Files_List[i].String_Name, CONFIGURATION_FILE_NAME_LENGTH) == 0) return &File_System.Files_List[i];
	}
	return NULL;
}

int FileSystemWriteFilesListEntry(char *String_File_Name, TFilesListEntry **Pointer_Pointer_New_Entry)
{
	unsigned int i;
	
	for (i = 0; i < File_System.File_System_Informations.Total_Files_Count; i++)
	{
		if (File_System.Files_List[i].String_Name[0] == 0)
		{
			strncpy(File_System.Files_List[i].String_Name, String_File_Name, CONFIGURATION_FILE_NAME_LENGTH);
			*Pointer_Pointer_New_Entry = &File_System.Files_List[i];
			return ERROR_CODE_NO_ERROR;
		}
	}
	// No room left in Files List
	return ERROR_CODE_FILES_LIST_FULL;
}

unsigned int FileSystemAllocateBlock(void)
{
	unsigned int New_Block;
	
	// Try to get a free block
	New_Block = FileSystemGetFirstFreeBlock();
	if (New_Block != FILE_SYSTEM_BLOCKS_LIST_FULL_CODE) File_System.Blocks_List[New_Block] = FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF; // "Reserve" the block on success
	return New_Block;
}

// Only the Installer program needs to create a new file system
#if CONFIGURATION_BUILD_INSTALLER
	int FileSystemCreate(unsigned int Blocks_Count, unsigned int Files_Count, unsigned int Starting_Sector)
	{
		unsigned int i, Required_Disk_Size;
		
		// The number of blocks must be greater or equal to the number of files or all files can't be stored on disk
		if (Blocks_Count < Files_Count) return 1;
		
		// Check if the file system is not too big to be mounted by the kernel
		if ((Blocks_Count > CONFIGURATION_FILE_SYSTEM_MAXIMUM_BLOCKS_LIST_ENTRIES) || (Files_Count > CONFIGURATION_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES)) return 1;
		
		// Check if the file system can fit on the hard disk
		Required_Disk_Size = (Blocks_Count * sizeof(unsigned int)) + (Files_Count * sizeof(TFilesListEntry)) + (Blocks_Count * CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES) + (CONFIGURATION_FILE_SYSTEM_STARTING_SECTOR_OFFSET * FILE_SYSTEM_SECTOR_SIZE_BYTES); // Size of the file system + size of data area + (size of MBR + size of kernel)
		// Convert the bytes value to sectors
		i = Required_Disk_Size / FILE_SYSTEM_SECTOR_SIZE_BYTES; // Use 'i' as a temporary variable
		if (Required_Disk_Size % FILE_SYSTEM_SECTOR_SIZE_BYTES != 0) i++;
		Required_Disk_Size = i;
		// Add the partition starting offset
		Required_Disk_Size += Starting_Sector;
		if (Required_Disk_Size > HardDiskGetDriveSizeSectors()) return 2;
		
		// Create empty file system
		// Create information record
		File_System.File_System_Informations.Magic_Number = FILE_SYSTEM_MAGIC_NUMBER;
		File_System.File_System_Informations.Total_Blocks_Count = Blocks_Count;
		File_System.File_System_Informations.Total_Files_Count = Files_Count;
		
		// Fill the Blocks List area with free block value
		for (i = 0; i < Blocks_Count; i++) File_System.Blocks_List[i] = FILE_SYSTEM_BLOCKS_LIST_BLOCK_FREE;
		// Set Files List area to 0 to indicate it's free
		memset(File_System.Files_List, 0, Files_Count * sizeof(TFilesListEntry));
		
		// Compute the variables needed to safely call FileSystemSave()
		// Compute Blocks List size in sectors
		i = Blocks_Count * sizeof(unsigned int) + sizeof(TFileSystemInformations); // Take the file system informations into account to ease their handling
		Blocks_List_Size_Sectors = i / FILE_SYSTEM_SECTOR_SIZE_BYTES;
		if (i % FILE_SYSTEM_SECTOR_SIZE_BYTES) Blocks_List_Size_Sectors++;
		
		// Compute Files List size in sectors
		i = Files_Count * sizeof(TFilesListEntry);
		Files_List_Size_Sectors = i / FILE_SYSTEM_SECTOR_SIZE_BYTES;
		if (i % FILE_SYSTEM_SECTOR_SIZE_BYTES != 0) Files_List_Size_Sectors++;
	
		// Determine starting sectors for each file system structures
		Blocks_List_First_Sector_Number = Starting_Sector;
		Files_List_First_Sector_Number = Blocks_List_First_Sector_Number + Blocks_List_Size_Sectors;
		
		// Save new generated file system
		FileSystemSave();

		// No error
		return 0;
	}
#endif