/** @file File.c
 * @see File.h for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Error_Codes.h>
#include <File_System/File.h>
#include <File_System/File_System.h>
#include <Kernel.h> // Needed to know program entry point, ...
#include <Standard_Functions.h> // Needed by strncpy(), ...

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
typedef struct __attribute__((packed))
{
	TFilesListEntry *Pointer_Files_List_Entry; //! Pointer on the corresponding Files List entry.
	unsigned int File_Descriptor_Index; //! The index into the file descriptors table.
	unsigned int Current_Block_Index; //! Hold the current ready to process block.
	unsigned int Offset_File; //! Offset into the file (needed to know how many bytes were processed yet).
	unsigned int Offset_Buffer; //! Offset into the buffer (must be kept across calls).
	char Opening_Mode; //! Tell if the file was open in read ('r') or write ('w') mode.
	char Is_Entry_Free; //! Indicate if the entry can be used to identify a new open file or not.
	char Is_Write_Possible; //! For a file opened in write mode, indicate if it is possible to write data or if there is no more space on the file system.
	unsigned char Buffer[CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES];
} TFileDescriptor;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Used by the file listing state machine to store the index of the last listed file. */
static unsigned int File_System_Last_Listed_File;

/** All the file descriptors. */
static TFileDescriptor File_Descriptors[CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT];

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int FileExists(char *String_File_Name)
{
	// Check if file name is valid
	if (String_File_Name[0] == 0) return 0;
	
	if (FileSystemReadFilesListEntry(String_File_Name) == NULL) return 0; // File not found
	return 1;
}

void FileListInitialize(void)
{
	File_System_Last_Listed_File = 0;
}

void FileListNext(char *String_File_Name)
{
	// Avoid blank entries
	while ((File_System_Last_Listed_File < File_System.File_System_Informations.Total_Files_Count) && (File_System.Files_List[File_System_Last_Listed_File].String_Name[0] == 0)) File_System_Last_Listed_File++;

	// Check if all files were listed
	if (File_System_Last_Listed_File >= File_System.File_System_Informations.Total_Files_Count) *String_File_Name = 0; // Signal end of listing
	else
	{
		// Copy name
		strncpy(String_File_Name, File_System.Files_List[File_System_Last_Listed_File].String_Name, CONFIGURATION_FILE_NAME_LENGTH);
		File_System_Last_Listed_File++;
	}
}

int FileRename(char *String_Current_File_Name, char *String_New_File_Name)
{
	TFilesListEntry *Pointer_Files_List_Entry;
	
	// Check if file names are valid
	if (String_New_File_Name[0] == 0) return ERROR_CODE_BAD_FILE_NAME;
	
	// Check if the new name is not assigned yet
	Pointer_Files_List_Entry = FileSystemReadFilesListEntry(String_New_File_Name);
	if (Pointer_Files_List_Entry != NULL) return ERROR_CODE_FILE_ALREADY_EXISTS;
	
	// Retrieve Current_File_Name file entry
	Pointer_Files_List_Entry = FileSystemReadFilesListEntry(String_Current_File_Name);
	if (Pointer_Files_List_Entry == NULL) return ERROR_CODE_FILE_NOT_FOUND;
	
	// Write new file name
	strncpy(Pointer_Files_List_Entry->String_Name, String_New_File_Name, CONFIGURATION_FILE_NAME_LENGTH);
	FileSystemSave();
	
	return ERROR_CODE_NO_ERROR;
}

int FileDelete(char *String_File_Name)
{
	TFilesListEntry *Pointer_Files_List_Entry;
	unsigned int First_Block, Last_Block, Next_Block;
	int i;
	
	// Check if file name is valid
	if (String_File_Name[0] == 0) return ERROR_CODE_FILE_NOT_FOUND;
	
	// Retrieve corresponding file entry
	Pointer_Files_List_Entry = FileSystemReadFilesListEntry(String_File_Name);
	if (Pointer_Files_List_Entry == NULL) return ERROR_CODE_FILE_NOT_FOUND;
	
	// Close the file if it was opened
	for (i = 0; i < CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT; i++)
	{
		if (strcmp(File_Descriptors[i].Pointer_Files_List_Entry->String_Name, String_File_Name) == 0)
		{
			File_Descriptors[i].Is_Entry_Free = 1;
			break; // A file can be opened only once at a time, no need to check other file descriptors
		}
	}
	
	// Free allocated blocks by appending the file blocks list to the free blocks list beginning (the free blocks list head will become the file first block)
	First_Block = Pointer_Files_List_Entry->Start_Block;
	// Find the file last block
	Last_Block = First_Block;
	Next_Block = First_Block;
	while (Next_Block != FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF)
	{
		// Get next block
		Next_Block = File_System.Blocks_List[Last_Block];
		if (Next_Block == FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF) break; // Keep the block that contains FILE_SYSTEM_BLOCKS_LIST_BLOCK_EOF value
		
		// Go to next block
		Last_Block = Next_Block;
	}
	// The file blocks list end will point to the free blocks list head
	File_System.Blocks_List[Last_Block] = File_System.File_System_Informations.Free_Blocks_List_Head;
	// The free blocks list head will point to the file blocks list beginning
	File_System.File_System_Informations.Free_Blocks_List_Head = First_Block;
	
	// Free the file entry
	Pointer_Files_List_Entry->String_Name[0] = 0;
	
	FileSystemSave();
	
	return ERROR_CODE_NO_ERROR;
}

unsigned int FileSize(char *String_File_Name)
{
	TFilesListEntry *Pointer_Files_List_Entry;
	
	// Check if file name is valid
	if (String_File_Name[0] == 0) return 0;
	
	// Retrieve corresponding file entry
	Pointer_Files_List_Entry = FileSystemReadFilesListEntry(String_File_Name);
	if (Pointer_Files_List_Entry == NULL) return 0;
	
	return Pointer_Files_List_Entry->Size_Bytes;
}

void FileResetFileDescriptors(void)
{
	int i;
	
	for (i = 0; i < CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT; i++) File_Descriptors[i].Is_Entry_Free = 1;
}

int FileOpen(char *String_File_Name, char Opening_Mode, unsigned int *Pointer_File_Descriptor_Index)
{
	TFilesListEntry *Pointer_Files_List_Entry;
	unsigned int i, Free_File_Descriptor_Index;
	TFileDescriptor *Pointer_File_Descriptor;
	
	// Check if file name is valid
	if (String_File_Name[0] == 0) return ERROR_CODE_BAD_FILE_NAME; // There is no need to check for NULL as none userspace pointer can be NULL in the kernel
	
	// Check if the file is not opened yet (as there is only one application running, the same application must not open the same file several times)
	for (i = 0; i < CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT; i++)
	{
		if (!File_Descriptors[i].Is_Entry_Free)
		{
			if (strcmp(File_Descriptors[i].Pointer_Files_List_Entry->String_Name, String_File_Name) == 0) return ERROR_CODE_FILE_OPENED_YET;
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
			Pointer_Files_List_Entry = FileSystemReadFilesListEntry(String_File_Name);
			if (Pointer_Files_List_Entry == NULL) return ERROR_CODE_FILE_NOT_FOUND;
			
			Pointer_File_Descriptor->Is_Write_Possible = 0;
			
			// Fetch the first block if the file is not empty
			if (Pointer_Files_List_Entry->Size_Bytes > 0) Pointer_File_Descriptor->Current_Block_Index = FileSystemReadBlocks(Pointer_Files_List_Entry->Start_Block, 1, Pointer_File_Descriptor->Buffer);
			break;
			
		// Write only
		case 'w':
			// Delete the file if it exists
			FileDelete(String_File_Name);
			
			// Allocate the file entry
			if (FileSystemWriteFilesListEntry(String_File_Name, &Pointer_Files_List_Entry) != ERROR_CODE_NO_ERROR) return ERROR_CODE_FILES_LIST_FULL;
			
			// Allocate the first block
			Pointer_File_Descriptor->Current_Block_Index = FileSystemAllocateBlock();
			if (Pointer_File_Descriptor->Current_Block_Index == FILE_SYSTEM_BLOCKS_LIST_FULL_CODE) return ERROR_CODE_BLOCKS_LIST_FULL;
			
			// For now consider the file as empty
			Pointer_Files_List_Entry->Start_Block = Pointer_File_Descriptor->Current_Block_Index;
			Pointer_Files_List_Entry->Size_Bytes = 0;
			
			Pointer_File_Descriptor->Is_Write_Possible = 1;
			break;
			
		// All other modes are not supported
		default:
			return ERROR_CODE_UNKNOWN_OPENING_MODE;
	}
	
	// Fill descriptor entry
	Pointer_File_Descriptor->Pointer_Files_List_Entry = Pointer_Files_List_Entry;
	Pointer_File_Descriptor->Opening_Mode = Opening_Mode;
	Pointer_File_Descriptor->Offset_File = 0;
	Pointer_File_Descriptor->Offset_Buffer = 0;
	Pointer_File_Descriptor->Is_Entry_Free = 0;
	
	*Pointer_File_Descriptor_Index = Free_File_Descriptor_Index;
	return ERROR_CODE_NO_ERROR;
}

int FileRead(unsigned int File_Descriptor_Index, void *Pointer_Buffer, unsigned int Bytes_Count, unsigned int *Pointer_Bytes_Read)
{
	TFileDescriptor *Pointer_File_Descriptor;
	unsigned int Bytes_To_Read;
	unsigned char *Pointer_Buffer_Byte = Pointer_Buffer;
	
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
	Bytes_To_Read = Pointer_File_Descriptor->Pointer_Files_List_Entry->Size_Bytes - Pointer_File_Descriptor->Offset_File;
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
		if (Pointer_File_Descriptor->Offset_Buffer >= CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES)
		{
			Pointer_File_Descriptor->Current_Block_Index = FileSystemReadBlocks(Pointer_File_Descriptor->Current_Block_Index, 1, Pointer_File_Descriptor->Buffer);
			Pointer_File_Descriptor->Offset_Buffer = 0;
		}
	
		// Copy bytes one at a time into the destination buffer
		*Pointer_Buffer_Byte = Pointer_File_Descriptor->Buffer[Pointer_File_Descriptor->Offset_Buffer];
		Pointer_Buffer_Byte++;
		Pointer_File_Descriptor->Offset_Buffer++;
		Bytes_Count--;
	}
	
	Pointer_File_Descriptor->Offset_File += Bytes_To_Read;
	*Pointer_Bytes_Read = Bytes_To_Read;

	return ERROR_CODE_NO_ERROR;
}

int FileWrite(unsigned int File_Descriptor_Index, void *Pointer_Buffer, unsigned int Bytes_Count)
{
	TFileDescriptor *Pointer_File_Descriptor;
	unsigned int New_Block, Written_Bytes_Count;
	unsigned char *Pointer_Buffer_Byte = Pointer_Buffer;

	// Is the file opened ?
	if (File_Descriptor_Index >= CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT) return ERROR_CODE_BAD_FILE_DESCRIPTOR;
	Pointer_File_Descriptor = &File_Descriptors[File_Descriptor_Index];
	if (Pointer_File_Descriptor->Is_Entry_Free) return ERROR_CODE_FILE_NOT_OPENED;
	// Is the file in write mode ?
	if (Pointer_File_Descriptor->Opening_Mode != 'w') return ERROR_CODE_BAD_OPENING_MODE;
	// Is there enough room on the file system to write to ?
	if (!Pointer_File_Descriptor->Is_Write_Possible) return ERROR_CODE_BLOCKS_LIST_FULL;
	
	Written_Bytes_Count = Bytes_Count;
	while (Bytes_Count > 0)
	{
		// Check if the cache is not full
		if (Pointer_File_Descriptor->Offset_Buffer >= CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES)
		{
			// Flush current block to disk
			FileSystemWriteBlocks(Pointer_File_Descriptor->Current_Block_Index, 1, Pointer_File_Descriptor->Buffer);
			Pointer_File_Descriptor->Offset_Buffer = 0;
			
			// Try to allocate a new block
			New_Block = FileSystemAllocateBlock();
			// Has the block been allocated or the Blocks List is full ?
			if (New_Block == FILE_SYSTEM_BLOCKS_LIST_FULL_CODE)
			{
				Pointer_File_Descriptor->Is_Write_Possible = 0;
				return ERROR_CODE_BLOCKS_LIST_FULL;
			}
			
			// Link the new block index to the flushed block Blocks List entry
			File_System.Blocks_List[Pointer_File_Descriptor->Current_Block_Index] = New_Block;
			Pointer_File_Descriptor->Current_Block_Index = New_Block;
		}
		
		// Copy the byte to the cache
		Pointer_File_Descriptor->Buffer[Pointer_File_Descriptor->Offset_Buffer] = *Pointer_Buffer_Byte;
		Pointer_File_Descriptor->Offset_Buffer++;
		Pointer_Buffer_Byte++;
		Bytes_Count--;
	}
	
	// Update the file size
	Pointer_File_Descriptor->Pointer_Files_List_Entry->Size_Bytes += Written_Bytes_Count;
	
	return ERROR_CODE_NO_ERROR;
}

void FileClose(unsigned int File_Descriptor_Index)
{
	TFileDescriptor *Pointer_File_Descriptor;
	
	// Is the file descriptor index valid ?
	if (File_Descriptor_Index >= CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT) return;
	Pointer_File_Descriptor = &File_Descriptors[File_Descriptor_Index];
	
	// Is the file opened ?
	if (Pointer_File_Descriptor->Is_Entry_Free) return;
	
	// Save file system if the file was opened in write mode to backup newly allocated Blocks List blocks
	if (Pointer_File_Descriptor->Opening_Mode == 'w')
	{
		// Flush the last block if the file is not empty (the last block is never flushed by FileWrite() as it would need one more call, but FileClose() is called instead)
		if (Pointer_File_Descriptor->Pointer_Files_List_Entry->Size_Bytes > 0) FileSystemWriteBlocks(Pointer_File_Descriptor->Current_Block_Index, 1, Pointer_File_Descriptor->Buffer);
		FileSystemSave();
	}
	
	// Free descriptor entry
	Pointer_File_Descriptor->Is_Entry_Free = 1;
}
