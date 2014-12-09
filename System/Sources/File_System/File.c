/** @file File.c
 * @see File.h for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <File_System/File.h>
#include <File_System/File_System.h>
#include <Kernel.h>
#include <Standard_Functions.h> // Needed by strncpy(), ...

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
typedef struct __attribute__((packed))
{
	TFileListEntry *Pointer_FL_Entry; //! Pointer on the corresponding Fl entry.
	unsigned int File_Descriptor_Index; //! The index into the file descriptors table.
	unsigned int Current_Block_Index; //! Hold the current available to process block.
	unsigned int Offset_File; //! Offset into the file (needed to know how many bytes were processed yet).
	unsigned int Offset_Buffer; //! Offset into the buffer (must be kept across calls).
	char Opening_Mode; //! Tell if the file was open in read ('r') or write ('w') mode.
	char Is_Entry_Free; //! Indicate if the entry can be used to identify a new open file or not.
	char Is_Write_Possible; //! For a file opened in write mode, indicate if it is possible to write data or if there is no more space on the file system.
	unsigned char Buffer[FILE_SYSTEM_BLOCK_SIZE_BYTES];
} TFileDescriptor;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
static unsigned int File_System_Last_Listed_File;
static TFileDescriptor File_Descriptors[CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
// Algorithm :
// Check if the file exists yet
//      If true, check if (BAT_Free_Space + Found_File_Size) is enough to store the file
//            If false, exit function with error code (so we keep old file version)
//      If false, check if there is enough room in BAT to store the file
//            If false, exit function with error code.
// Store file. 
int FileCreate(char *String_File_Name, unsigned char *Buffer, unsigned int Buffer_Size_Bytes)
{
	TFileListEntry *Pointer_FL_Entry;
	unsigned int New_File_Size_Blocks, Old_File_Size_Blocks;
	
	// Check if new file name is valid
	if (String_File_Name[0] == 0) return ERROR_CODE_BAD_FILE_NAME;
	
	// Compute new file size in blocks now because it is needed further
	New_File_Size_Blocks = Buffer_Size_Bytes / FILE_SYSTEM_BLOCK_SIZE_BYTES;
	if ((Buffer_Size_Bytes % FILE_SYSTEM_BLOCK_SIZE_BYTES) != 0) New_File_Size_Blocks++;
	
	// Does the file exist yet ?
	Pointer_FL_Entry = FileSystemReadFLEntry(String_File_Name);
	// Yes
	if (Pointer_FL_Entry != NULL)
	{
		// Compute old file size in blocks
		Old_File_Size_Blocks =  Pointer_FL_Entry->Size_Bytes / FILE_SYSTEM_BLOCK_SIZE_BYTES;
		if ((Pointer_FL_Entry->Size_Bytes % FILE_SYSTEM_BLOCK_SIZE_BYTES) != 0) Old_File_Size_Blocks++;
		
		// Check if free block's space + file size is enough to store new file
		if (FileSystemGetFreeBlocksCount() + Old_File_Size_Blocks < New_File_Size_Blocks) return ERROR_CODE_BAT_FULL;
		
		// Delete file
		FileDelete(String_File_Name);
	}
	// No, check if free blocks space is enough to store files
	else if (FileSystemGetFreeBlocksCount() < New_File_Size_Blocks) return ERROR_CODE_BAT_FULL;
	
	// Create new entry in FL
	if (FileSystemWriteFLEntry(String_File_Name, &Pointer_FL_Entry) == ERROR_CODE_FL_FULL) return ERROR_CODE_FL_FULL;
	// Fill FL entry
	Pointer_FL_Entry->Start_Block = FileSystemGetFirstFreeBlock();
	Pointer_FL_Entry->Size_Bytes = Buffer_Size_Bytes;
	
	// Write data to disk
	FileSystemWriteBlocks(Pointer_FL_Entry->Start_Block, New_File_Size_Blocks, Buffer);
	FileSystemSave();
	
	// No error
	return ERROR_CODE_NO_ERROR;
}

int FileExists(char *String_File_Name)
{
	// Check if file name is valid
	if (String_File_Name[0] == 0) return 0;
	
	if (FileSystemReadFLEntry(String_File_Name) == NULL) return 0; // File not found
	return 1;
}

void FileListInitialize(void)
{
	File_System_Last_Listed_File = 0;
}

void FileListNext(char *String_File_Name)
{
	// Avoid blank entries
	while ((File_System_Last_Listed_File < File_System_Informations.Total_Files_Count) && (Files_List[File_System_Last_Listed_File].Name[0] == 0)) File_System_Last_Listed_File++;

	// Check if all files were listed
	if (File_System_Last_Listed_File >= File_System_Informations.Total_Files_Count) *String_File_Name = 0; // Signal end of listing
	else
	{
		// Copy name
		strncpy(String_File_Name, Files_List[File_System_Last_Listed_File].Name, CONFIGURATION_FILE_NAME_LENGTH);
		File_System_Last_Listed_File++;
	}
}

int FileRename(char *String_Current_File_Name, char *String_New_File_Name)
{
	TFileListEntry *Pointer_FL_Entry;
	
	// Check if file names are valid
	if (String_New_File_Name[0] == 0) return ERROR_CODE_BAD_FILE_NAME;
	
	// Check if the new name is not assigned yet
	Pointer_FL_Entry = FileSystemReadFLEntry(String_New_File_Name);
	if (Pointer_FL_Entry != NULL) return ERROR_CODE_FILE_ALREADY_EXISTS;
	
	// Retrieve Current_File_Name file entry
	Pointer_FL_Entry = FileSystemReadFLEntry(String_Current_File_Name);
	if (Pointer_FL_Entry == NULL) return ERROR_CODE_FILE_NOT_FOUND;
	
	// Write new file name
	strncpy(Pointer_FL_Entry->Name, String_New_File_Name, CONFIGURATION_FILE_NAME_LENGTH);
	FileSystemSave();
	
	return ERROR_CODE_NO_ERROR;
}

int FileDelete(char *String_File_Name)
{
	TFileListEntry *Pointer_FL_Entry;
	unsigned int Block, Next_Block;
	
	// Check if file name is valid
	if (String_File_Name[0] == 0) return ERROR_CODE_FILE_NOT_FOUND;
	
	// Retrieve corresponding file entry
	Pointer_FL_Entry = FileSystemReadFLEntry(String_File_Name);
	if (Pointer_FL_Entry == NULL) return ERROR_CODE_FILE_NOT_FOUND;
	
	// Free BAT blocks
	Block = Pointer_FL_Entry->Start_Block;
	while (Block != FILE_SYSTEM_BAT_BLOCK_EOF)
	{
		// Get next block
		Next_Block = Block_Allocation_Table[Block];
		// Erase current block
		Block_Allocation_Table[Block] = FILE_SYSTEM_BAT_BLOCK_FREE;
		// Go to next block
		Block = Next_Block;
	}
	
	// Free the file entry after having freed the allocated blocks, so if the system is powered off during the blocks freeing step the file system won't be corrupted : the file list entry is still existing
	Pointer_FL_Entry->Name[0] = 0;
	
	FileSystemSave();
	
	return ERROR_CODE_NO_ERROR;
}

unsigned int FileSize(char *String_File_Name)
{
	TFileListEntry *Pointer_FL_Entry;
	
	// Check if file name is valid
	if (String_File_Name[0] == 0) return 0;
	
	// Retrieve corresponding file entry
	Pointer_FL_Entry = FileSystemReadFLEntry(String_File_Name);
	if (Pointer_FL_Entry == NULL) return 0;
	
	return Pointer_FL_Entry->Size_Bytes;
}

int FileLoad(char *String_File_Name, unsigned char *Buffer, int Is_Executable_Check_Enabled)
{
	TFileListEntry *Pointer_FL_Entry;
	unsigned int File_Size_Bytes, File_Size_Blocks, *Pointer_Magic_Number, Next_Block;
	
	// Check if file name is valid
	if (String_File_Name[0] == 0) return ERROR_CODE_FILE_NOT_FOUND;
	
	// Get corresponding file entry
	Pointer_FL_Entry = FileSystemReadFLEntry(String_File_Name);
	if (Pointer_FL_Entry == NULL) return ERROR_CODE_FILE_NOT_FOUND;
	
	// Do program size checks
	File_Size_Bytes = Pointer_FL_Entry->Size_Bytes;
	// Is there something to load ?
	if (File_Size_Bytes == 0)
	{
		if (Is_Executable_Check_Enabled) return ERROR_CODE_FILE_NOT_EXECUTABLE; // As the file is empty, the executable magic number is not present
		return ERROR_CODE_NO_ERROR;
	}
	// Is there enough room in RAM to store the file ?
	if (File_Size_Bytes > KERNEL_USER_SPACE_SIZE - KERNEL_PROGRAM_ENTRY_POINT) return ERROR_CODE_FILE_LARGER_THAN_RAM;
	
	// Load first block to check the magic number
	Next_Block = FileSystemReadBlocks(Pointer_FL_Entry->Start_Block, 1, Buffer);
	if (Is_Executable_Check_Enabled)
	{
		Pointer_Magic_Number = (unsigned int *) Buffer;
		if (*Pointer_Magic_Number != KERNEL_PROGRAM_MAGIC_NUMBER) return ERROR_CODE_FILE_NOT_EXECUTABLE;
	}
	// Adjust buffer for next blocks
	Buffer += FILE_SYSTEM_BLOCK_SIZE_BYTES;

	// Compute file size in blocks
	File_Size_Blocks = File_Size_Bytes / FILE_SYSTEM_BLOCK_SIZE_BYTES;
	if ((File_Size_Bytes % FILE_SYSTEM_BLOCK_SIZE_BYTES) != 0) File_Size_Blocks++;
	
	// Load the remaining part of the file
	FileSystemReadBlocks(Next_Block, File_Size_Blocks - 1, Buffer); // All blocks minus the one previously loaded 
	return ERROR_CODE_NO_ERROR;
}

void FileResetFileDescriptors(void)
{
	int i;
	
	for (i = 0; i < CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT; i++) File_Descriptors[i].Is_Entry_Free = 1;
}

int FileOpen(char *String_File_Name, char Opening_Mode, unsigned int *Pointer_File_Descriptor_Index)
{
	TFileListEntry *Pointer_FL_Entry;
	unsigned int i, Free_File_Descriptor_Index;
	TFileDescriptor *Pointer_File_Descriptor;
	
	// Check if file name is valid
	if (String_File_Name[0] == 0) return ERROR_CODE_BAD_FILE_NAME;
	
	// Check if the file is not opened yet (as there is only one application running, the same application must not open the same file several times)
	for (i = 0; i < CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT; i++)
	{
		if (!File_Descriptors[i].Is_Entry_Free)
		{
			if (strcmp(File_Descriptors[i].Pointer_FL_Entry->Name, String_File_Name) == 0) return ERROR_CODE_FILE_OPENED_YET;
		}
	}
	
	// Search for the first free entry in the file descriptors table
	for (Free_File_Descriptor_Index = 0; Free_File_Descriptor_Index < CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT; Free_File_Descriptor_Index++)
	{
		if (File_Descriptors[Free_File_Descriptor_Index].Is_Entry_Free) break;
	}
	if (Free_File_Descriptor_Index == CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT) return ERROR_CODE_CANT_OPEN_MORE_FILES;
	Pointer_File_Descriptor = &File_Descriptors[Free_File_Descriptor_Index];
	
	// Open file
	switch (Opening_Mode)
	{
		// Read only
		case 'r':
			// Retrieve corresponding file entry
			Pointer_FL_Entry = FileSystemReadFLEntry(String_File_Name);
			if (Pointer_FL_Entry == NULL) return ERROR_CODE_FILE_NOT_FOUND;
			
			Pointer_File_Descriptor->Is_Write_Possible = 0;
			
			// Fetch the first block if the file is not empty
			if (Pointer_FL_Entry->Size_Bytes > 0) Pointer_File_Descriptor->Current_Block_Index = FileSystemReadBlocks(Pointer_FL_Entry->Start_Block, 1, Pointer_File_Descriptor->Buffer);
			break;
			
		// Write only
		case 'w':
			// Delete the file if it exists
			FileDelete(String_File_Name);
			
			// Allocate the file entry
			if (FileSystemWriteFLEntry(String_File_Name, &Pointer_FL_Entry) != ERROR_CODE_NO_ERROR) return ERROR_CODE_FL_FULL;
			
			// Allocate the first block
			Pointer_File_Descriptor->Current_Block_Index = FileSystemAllocateBlock();
			if (Pointer_File_Descriptor->Current_Block_Index == ERROR_CODE_BAT_FULL) return ERROR_CODE_BAT_FULL;
			
			// For now consider the file as empty
			Pointer_FL_Entry->Start_Block = Pointer_File_Descriptor->Current_Block_Index;
			Pointer_FL_Entry->Size_Bytes = 0;
			
			Pointer_File_Descriptor->Is_Write_Possible = 1;
			break;
			
		// All other modes are not supported
		default:
			return ERROR_CODE_UNKNOWN_OPENING_MODE;
	}
	
	// Fill descriptor entry
	Pointer_File_Descriptor->Pointer_FL_Entry = Pointer_FL_Entry;
	Pointer_File_Descriptor->Opening_Mode = Opening_Mode;
	Pointer_File_Descriptor->Offset_File = 0;
	Pointer_File_Descriptor->Offset_Buffer = 0;
	Pointer_File_Descriptor->Is_Entry_Free = 0;
	
	*Pointer_File_Descriptor_Index = Free_File_Descriptor_Index;
	return ERROR_CODE_NO_ERROR;
}

int FileRead(unsigned int File_Descriptor_Index, unsigned char *Pointer_Buffer, unsigned int Bytes_Count, unsigned int *Pointer_Bytes_Read)
{
	TFileDescriptor *Pointer_File_Descriptor;
	unsigned int Bytes_To_Read;
	
	// Is there something to read ?
	if (Bytes_Count == 0)
	{
		*Pointer_Bytes_Read = 0;
		return ERROR_CODE_NO_ERROR;
	}
	// Is the file opened ?
	if (File_Descriptor_Index >= CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT) return ERROR_CODE_BAD_FILE_DESCRIPTOR;
	Pointer_File_Descriptor = &File_Descriptors[File_Descriptor_Index];
	if (Pointer_File_Descriptor->Is_Entry_Free) return ERROR_CODE_FILE_NOT_OPENED;
	// Is the file in read mode ?
	if (Pointer_File_Descriptor->Opening_Mode != 'r') return ERROR_CODE_BAD_OPENING_MODE;
	
	// Check how many more bytes can be read from the file
	Bytes_To_Read = Pointer_File_Descriptor->Pointer_FL_Entry->Size_Bytes - Pointer_File_Descriptor->Offset_File;
	// If there is no more byte to read the file end is reached
	if (Bytes_To_Read == 0)
	{
		*Pointer_Bytes_Read = 0;
		return ERROR_CODE_NO_ERROR;
	}
	
	// Adjust remaining bytes to read count according to file size
	if (Bytes_Count > Bytes_To_Read) Bytes_Count = Bytes_To_Read;
	else Bytes_To_Read = Bytes_Count; // Keep bytes to read count for later
	
	// Read data from file
	while (Bytes_Count > 0)
	{
		// Load next block when needed (i.e. when a block is fully read)
		if (Pointer_File_Descriptor->Offset_Buffer >= FILE_SYSTEM_BLOCK_SIZE_BYTES)
		{
			Pointer_File_Descriptor->Current_Block_Index = FileSystemReadBlocks(Pointer_File_Descriptor->Current_Block_Index, 1, Pointer_File_Descriptor->Buffer);
			Pointer_File_Descriptor->Offset_Buffer = 0;
		}
	
		// Copy bytes one at a time into the destination buffer
		*Pointer_Buffer = Pointer_File_Descriptor->Buffer[Pointer_File_Descriptor->Offset_Buffer];
		Pointer_Buffer++;
		Pointer_File_Descriptor->Offset_Buffer++;
		Bytes_Count--;
	}
	
	Pointer_File_Descriptor->Offset_File += Bytes_To_Read;
	*Pointer_Bytes_Read = Bytes_To_Read;

	return ERROR_CODE_NO_ERROR;
}

int FileWrite(unsigned int File_Descriptor_Index, unsigned char *Pointer_Buffer, unsigned int Bytes_Count)
{
	TFileDescriptor *Pointer_File_Descriptor;
	unsigned int New_Block, Written_Bytes_Count;

	// Is the file opened ?
	if (File_Descriptor_Index >= CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT) return ERROR_CODE_BAD_FILE_DESCRIPTOR;
	Pointer_File_Descriptor = &File_Descriptors[File_Descriptor_Index];
	if (Pointer_File_Descriptor->Is_Entry_Free) return ERROR_CODE_FILE_NOT_OPENED;
	// Is the file in write mode ?
	if (Pointer_File_Descriptor->Opening_Mode != 'w') return ERROR_CODE_BAD_OPENING_MODE;
	// Is there enough room on the file system to write to ?
	if (!Pointer_File_Descriptor->Is_Write_Possible) return ERROR_CODE_BAT_FULL;
	
	Written_Bytes_Count = Bytes_Count;
	while (Bytes_Count > 0)
	{
		// Check if the cache is not full
		if (Pointer_File_Descriptor->Offset_Buffer >= FILE_SYSTEM_BLOCK_SIZE_BYTES)
		{
			// Flush current block to disk
			FileSystemWriteBlocks(Pointer_File_Descriptor->Current_Block_Index, 1, Pointer_File_Descriptor->Buffer);
			Pointer_File_Descriptor->Offset_Buffer = 0;
			
			// Try to allocate a new block
			New_Block = FileSystemAllocateBlock();
			// Has the block been allocated or the BAT is full ?
			if (New_Block == FILE_SYSTEM_BAT_FULL_CODE)
			{
				Pointer_File_Descriptor->Is_Write_Possible = 0;
				return ERROR_CODE_BAT_FULL;
			}
			
			// Link the new block index to the flushed block BAT entry
			Block_Allocation_Table[Pointer_File_Descriptor->Current_Block_Index] = New_Block;
			Pointer_File_Descriptor->Current_Block_Index = New_Block;
		}
		
		// Copy the byte to the cache
		Pointer_File_Descriptor->Buffer[Pointer_File_Descriptor->Offset_Buffer] = *Pointer_Buffer;
		Pointer_File_Descriptor->Offset_Buffer++;
		Pointer_Buffer++;
		Bytes_Count--;
	}
	
	// Update the file size
	Pointer_File_Descriptor->Pointer_FL_Entry->Size_Bytes += Written_Bytes_Count;
	
	return ERROR_CODE_NO_ERROR;
}

void FileClose(unsigned int File_Descriptor_Index)
{
	TFileDescriptor *Pointer_File_Descriptor;
	
	// Is the file descriptor index valid ?
	if (File_Descriptor_Index >= CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT) return;
	Pointer_File_Descriptor = &File_Descriptors[File_Descriptor_Index];
	
	// Save file system if the file was opened in write mode to backup newly allocated BAT blocks
	if (Pointer_File_Descriptor->Opening_Mode == 'w')
	{
		// Flush last block if it exists (even if is has been flushed yet as this case rarely happens)
		if (Pointer_File_Descriptor->Pointer_FL_Entry->Size_Bytes > 0) FileSystemWriteBlocks(Pointer_File_Descriptor->Current_Block_Index, 1, Pointer_File_Descriptor->Buffer);
		FileSystemSave();
	}
	
	// Free descriptor entry
	Pointer_File_Descriptor->Is_Entry_Free = 1;
}
