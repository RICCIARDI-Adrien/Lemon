/** @file File_System.c
 * @see File_System.h for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Drivers/Driver_Hard_Disk.h>
#include <File_System/File_System.h>
#include <Kernel.h> // For error codes
#include <Standard_Functions.h>

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Tell if a correct file system is stored on the disk or not. */
#define FILE_SYSTEM_MAGIC_NUMBER 0x12345678
/** Offset into the MBR sector where file system informations are stored. */
#define FILE_SYSTEM_INFORMATIONS_OFFSET (510 - sizeof(TFileSystemInformations)) // The "-2" is to bypass MBR magic number

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The Block Allocation Table size in hard disk sectors. */
static unsigned int Block_Allocation_Table_Size_Sectors;
/** The ID of the first hard disk sector containing the Files List. */
static unsigned int Block_Allocation_Table_First_Sector_Number;

/** The Files List size in hard disk sectors. */
static unsigned int Files_List_Size_Sectors;
/** The ID of the first hard disk sector containing the Files List. */
static unsigned int Files_List_First_Sector_Number;

/** First sector dedicated to data, located right after the file system. */
static unsigned int File_System_First_Data_Sector;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
TFileSystemInformations File_System_Informations;
unsigned int Block_Allocation_Table[CONFIGURATION_FILE_SYSTEM_MAXIMUM_BLOCK_ALLOCATION_TABLE_ENTRIES];
TFileListEntry Files_List[CONFIGURATION_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES];

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
int FileSystemInit(void)
{
	unsigned char Sector_Temp[FILE_SYSTEM_SECTOR_SIZE_BYTES];
	unsigned int Temp;
	
	// Retrieve file system informations
	// Load the hard disk MBR to prevent errors if we are booting from another drive
	HardDiskReadSector(0, Sector_Temp);
	memcpy((unsigned char *) &File_System_Informations, Sector_Temp + FILE_SYSTEM_INFORMATIONS_OFFSET, sizeof(TFileSystemInformations)); 
	
	// Check if there is a valid file system on the device
	if (File_System_Informations.Magic_Number != FILE_SYSTEM_MAGIC_NUMBER) return 0;
	// Check if the file system is small enough to fit into the kernel reserved memory space
	if ((File_System_Informations.Total_Blocks_Count > CONFIGURATION_FILE_SYSTEM_MAXIMUM_BLOCK_ALLOCATION_TABLE_ENTRIES) || (File_System_Informations.Total_Files_Count > CONFIGURATION_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES)) return 0;
	
	// Compute Block Allocation Table size in sectors
	Temp = File_System_Informations.Total_Blocks_Count * sizeof(unsigned int);
	Block_Allocation_Table_Size_Sectors = Temp / FILE_SYSTEM_SECTOR_SIZE_BYTES;
	if (Temp % FILE_SYSTEM_SECTOR_SIZE_BYTES) Block_Allocation_Table_Size_Sectors++;
	
	// Compute Files List size in sectors
	Temp = File_System_Informations.Total_Files_Count * sizeof(TFileListEntry);
	Files_List_Size_Sectors = Temp / FILE_SYSTEM_SECTOR_SIZE_BYTES;
	if (Temp % FILE_SYSTEM_SECTOR_SIZE_BYTES != 0) Files_List_Size_Sectors++;
	
	// Determine starting sectors for each file system structures
	Block_Allocation_Table_First_Sector_Number = CONFIGURATION_FILE_SYSTEM_STARTING_SECTOR;
	Files_List_First_Sector_Number = Block_Allocation_Table_First_Sector_Number + Block_Allocation_Table_Size_Sectors;
	File_System_First_Data_Sector = Files_List_First_Sector_Number + Files_List_Size_Sectors;
	
	// Load Block Allocation Table and Files List
	FileSystemReadSectors(Block_Allocation_Table_First_Sector_Number, Block_Allocation_Table_Size_Sectors, Block_Allocation_Table);
	FileSystemReadSectors(Files_List_First_Sector_Number, Files_List_Size_Sectors, Files_List);

	// No error
	return 1;
}

void FileSystemSave(void)
{
	FileSystemWriteSectors(Block_Allocation_Table_First_Sector_Number, Block_Allocation_Table_Size_Sectors, Block_Allocation_Table);
	FileSystemWriteSectors(Files_List_First_Sector_Number, Files_List_Size_Sectors, Files_List);
}

unsigned int FileSystemGetFreeBlocksCount(void)
{
	unsigned int i, Free_Blocks_Count = 0;
	
	for (i = 0; i < File_System_Informations.Total_Blocks_Count; i++)
	{
		if (Block_Allocation_Table[i] == FILE_SYSTEM_BAT_BLOCK_FREE) Free_Blocks_Count++;
	}
	return Free_Blocks_Count;
}

unsigned int FileSystemGetFreeFileListEntriesCount(void)
{
	unsigned int i, File_List_Entries_Count = 0;
	
	for (i = 0; i < File_System_Informations.Total_Files_Count; i++)
	{
		if (Files_List[i].Name[0] == 0) File_List_Entries_Count++;
	}
	return File_List_Entries_Count;
}

unsigned int FileSystemGetFirstFreeBlock(void)
{
	unsigned int i;
	
	for (i = 0; i < File_System_Informations.Total_Blocks_Count; i++)
	{
		if (Block_Allocation_Table[i] == FILE_SYSTEM_BAT_BLOCK_FREE) return i;
	}
	// No more room in BAT
	return FILE_SYSTEM_BAT_FULL_CODE;
}

unsigned int FileSystemReadBlocks(unsigned int Start_Block, unsigned int Blocks_Count, unsigned char *Buffer)
{
	unsigned int i, j, Block, Sector;
	
	// Is end of file reached ?
	if (Start_Block == FILE_SYSTEM_BAT_BLOCK_EOF) return FILE_SYSTEM_BAT_BLOCK_EOF;
		
	Block = Start_Block;
	for (i = 0; i < Blocks_Count; i++)
	{
		// Read block
		Sector = (Block * (FILE_SYSTEM_BLOCK_SIZE_BYTES / FILE_SYSTEM_SECTOR_SIZE_BYTES)) + File_System_First_Data_Sector;
		for (j = 0; j < FILE_SYSTEM_BLOCK_SIZE_BYTES / FILE_SYSTEM_SECTOR_SIZE_BYTES; j++)
		{
			HardDiskReadSector(Sector, Buffer);
			Sector++;
			Buffer += FILE_SYSTEM_SECTOR_SIZE_BYTES;
		}

		// Next block
		Block = Block_Allocation_Table[Block];
		// Return FILE_SYSTEM_BAT_BLOCK_EOF if end of file is reached
		if (Block == FILE_SYSTEM_BAT_BLOCK_EOF) break;
	}
	return Block;
}

unsigned int FileSystemWriteBlocks(unsigned int Start_Block, unsigned int Blocks_Count, unsigned char *Buffer)
{
	unsigned int i, j, Block, Next_Block, Sector;
	
	Block = Start_Block;
	
	for (i = 0; i < Blocks_Count; i++)
	{
		// Write block
		Sector = (Block * (FILE_SYSTEM_BLOCK_SIZE_BYTES / FILE_SYSTEM_SECTOR_SIZE_BYTES)) + File_System_First_Data_Sector;
		for (j = 0; j < FILE_SYSTEM_BLOCK_SIZE_BYTES / FILE_SYSTEM_SECTOR_SIZE_BYTES; j++)
		{
			HardDiskWriteSector(Sector, Buffer);
			Sector++;
			Buffer += FILE_SYSTEM_SECTOR_SIZE_BYTES;
		}
		
		// Write end-of-file in the last block
		if (i == Blocks_Count - 1) Block_Allocation_Table[Block] = FILE_SYSTEM_BAT_BLOCK_EOF;
		else
		{
			// Find next block
			Block_Allocation_Table[Block] = 12; // Set a value to the previously written block in order to avoid getting this block when calling FileSystemGetFirstFreeBlock()
			Next_Block = FileSystemGetFirstFreeBlock();
			// Update BAT with next block
			Block_Allocation_Table[Block] = Next_Block;
			Block = Next_Block;
		}
	}
	
	// Last written block
	return Block;
}

TFileListEntry *FileSystemReadFLEntry(char *File_Name)
{
	unsigned int i;
	
	// Search for matching file name in FL
	for (i = 0; i < File_System_Informations.Total_Files_Count; i++)
	{
		if (strncmp(File_Name, Files_List[i].Name, CONFIGURATION_FILE_NAME_LENGTH) == 0) return &Files_List[i];
	}
	return NULL;
}

int FileSystemWriteFLEntry(char *File_Name, TFileListEntry **New_Entry)
{
	unsigned int i;
	
	for (i = 0; i < File_System_Informations.Total_Files_Count; i++)
	{
		if (Files_List[i].Name[0] == 0)
		{
			strncpy(Files_List[i].Name, File_Name, CONFIGURATION_FILE_NAME_LENGTH);
			*New_Entry = &Files_List[i];
			return ERROR_CODE_NO_ERROR;
		}
	}
	// No room left in FL
	return ERROR_CODE_FL_FULL;
}

unsigned int FileSystemAllocateBlock(void)
{
	unsigned int New_Block;
	
	// Try to get a free block
	New_Block = FileSystemGetFirstFreeBlock();
	if (New_Block == FILE_SYSTEM_BAT_FULL_CODE) return ERROR_CODE_BAT_FULL;
	
	// "Reserve" the block
	Block_Allocation_Table[New_Block] = FILE_SYSTEM_BAT_BLOCK_EOF;
	return New_Block;
}

// Only the Installer program needs to create a new file system
#ifdef INSTALLER
	int FileSystemCreate(unsigned int Blocks_Count, unsigned int Files_Count, unsigned char *Pointer_MBR_Code)
	{
		unsigned long long Required_Size;
		unsigned int i;
				
		// The number of blocks must be greater or equal to the number of files or all files can't be stored on disk
		if (Blocks_Count < Files_Count) return 1;
		
		// Check is the file system is not too big to be mounted by the kernel
		if ((Blocks_Count > CONFIGURATION_FILE_SYSTEM_MAXIMUM_BLOCK_ALLOCATION_TABLE_ENTRIES) || (Files_Count > CONFIGURATION_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES)) return 1;
		
		// Check if the file system can fit on the hard disk
		Required_Size = (Blocks_Count * sizeof(unsigned int)) + (Files_Count * sizeof(TFileListEntry)) + (Blocks_Count * FILE_SYSTEM_BLOCK_SIZE_BYTES) + (CONFIGURATION_FILE_SYSTEM_STARTING_SECTOR * FILE_SYSTEM_SECTOR_SIZE_BYTES); // Size of the file system + size of data area + (size of MBR + size of kernel)
		if (Required_Size > HardDiskGetDriveSizeBytes()) return 2;
		
		// Create empty file system
		// Create information record
		File_System_Informations.Magic_Number = FILE_SYSTEM_MAGIC_NUMBER;
		File_System_Informations.Total_Blocks_Count = Blocks_Count;
		File_System_Informations.Total_Files_Count = Files_Count;
		
		// Copy to MBR
		memcpy(Pointer_MBR_Code + FILE_SYSTEM_INFORMATIONS_OFFSET, &File_System_Informations, sizeof(TFileSystemInformations));
		// Copy MBR to disk
		HardDiskWriteSector(0, Pointer_MBR_Code);
		
		// Set BAT area to free block value
		for (i = 0; i < Blocks_Count; i++) Block_Allocation_Table[i] = FILE_SYSTEM_BAT_BLOCK_FREE;
		// Set FL area to 0 to indicate it's free
		memset(Files_List, 0, Files_Count * sizeof(TFileListEntry));
		
		// Compute the variables needed to safely call FileSystemSave()
		// Compute Block Allocation Table size in sectors
		i = Blocks_Count * sizeof(unsigned int);
		Block_Allocation_Table_Size_Sectors = i / FILE_SYSTEM_SECTOR_SIZE_BYTES;
		if (i % FILE_SYSTEM_SECTOR_SIZE_BYTES) Block_Allocation_Table_Size_Sectors++;
		
		// Compute Files List size in sectors
		i = Files_Count * sizeof(TFileListEntry);
		Files_List_Size_Sectors = i / FILE_SYSTEM_SECTOR_SIZE_BYTES;
		if (i % FILE_SYSTEM_SECTOR_SIZE_BYTES != 0) Files_List_Size_Sectors++;
	
		// Determine starting sectors for each file system structures
		Block_Allocation_Table_First_Sector_Number = CONFIGURATION_FILE_SYSTEM_STARTING_SECTOR;
		Files_List_First_Sector_Number = Block_Allocation_Table_First_Sector_Number + Block_Allocation_Table_Size_Sectors;
		
		// Save new generated file system
		FileSystemSave();

		// No error
		return 0;
	}
#endif