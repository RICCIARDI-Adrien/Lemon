/** @file Driver_Hard_Disk_RAM_Disk.c
 * Simulate a hard disk storage by reading and writing a RAM area.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Debug.h>
#include <Drivers/Driver_Hard_Disk.h>
#include <Error_Codes.h>
#include <File_System/File.h>
#include <File_System/File_System.h>
#include <Installer_Embedded_Files_Data.h>
#include <Standard_Functions.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Put the RAM disk area right after the user space area. */
#define HARD_DISK_RAM_DISK_MEMORY_AREA_ADDRESS (CONFIGURATION_USER_SPACE_ADDRESS + CONFIGURATION_USER_SPACE_SIZE)

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The file system total size (file system structures plus data blocks) in sectors. */
static unsigned int Hard_Disk_RAM_Disk_File_System_Total_Size_Sectors;

/** The memory area itself. */
static unsigned char *Pointer_Memory_Area = (unsigned char *) HARD_DISK_RAM_DISK_MEMORY_AREA_ADDRESS;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Install embedded files to the file system.
 * @return 0 if all files were successfully installed,
 * @return 1 if an error occurred.
 */
static int HardDiskRAMDiskInstallFiles(void)
{
	int Embedded_Files_To_Install_Count, i;
	unsigned int File_Descriptor;
	
	// Compute the total number of embedded files
	Embedded_Files_To_Install_Count = sizeof(Embedded_Files) / sizeof(TEmbeddedFile);
	
	// Bypass MBR and kernel files
	for (i = 2; i < Embedded_Files_To_Install_Count; i++)
	{
		// Try to open the file
		if (FileOpen(Embedded_Files[i].String_Name, 'w', &File_Descriptor) != ERROR_CODE_NO_ERROR) return 1;
		// Write the file content
		if (FileWrite(File_Descriptor, Embedded_Files[i].Pointer_Data, Embedded_Files[i].Size_Bytes) != ERROR_CODE_NO_ERROR) return 1;
		FileClose(File_Descriptor);
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int HardDiskInitialize(void)
{
	// Compute the file system size in blocks (Files List size + Blocks list size + data blocks size)
	Hard_Disk_RAM_Disk_File_System_Total_Size_Sectors = FileSystemComputeSizeSectors(CONFIGURATION_FILE_SYSTEM_MAXIMUM_BLOCKS_LIST_ENTRIES, CONFIGURATION_SYSTEM_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES);
	
	DEBUG_SECTION_START
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
		ScreenWriteString("RAM disk memory area : 0x");
		DebugWriteHexadecimalInteger(HARD_DISK_RAM_DISK_MEMORY_AREA_ADDRESS);
		ScreenWriteString("\nRAM disk sectors count : ");
		ScreenWriteString(itoa(Hard_Disk_RAM_Disk_File_System_Total_Size_Sectors));
		ScreenWriteCharacter('\n');
		KeyboardReadCharacter();
	DEBUG_SECTION_END
	
	// Create the file system in RAM
	if (FileSystemCreate(CONFIGURATION_FILE_SYSTEM_MAXIMUM_BLOCKS_LIST_ENTRIES, CONFIGURATION_SYSTEM_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES, 0) != ERROR_CODE_NO_ERROR) return 3;
	
	// Load file system
	if (!FileSystemInitialize(0)) return 4;
	
	// Populate file system
	if (HardDiskRAMDiskInstallFiles() != 0) return 5;
	
	return 0;
}

void HardDiskReadSector(unsigned int Logical_Sector_Number, void *Pointer_Buffer)
{
	memcpy(Pointer_Buffer, &Pointer_Memory_Area[Logical_Sector_Number * FILE_SYSTEM_SECTOR_SIZE_BYTES], FILE_SYSTEM_SECTOR_SIZE_BYTES);
}

void HardDiskWriteSector(unsigned int Logical_Sector_Number, void *Pointer_Buffer)
{
	memcpy(&Pointer_Memory_Area[Logical_Sector_Number * FILE_SYSTEM_SECTOR_SIZE_BYTES], Pointer_Buffer, FILE_SYSTEM_SECTOR_SIZE_BYTES);
}

unsigned int HardDiskGetDriveSizeSectors(void)
{
	return Hard_Disk_RAM_Disk_File_System_Total_Size_Sectors;
}