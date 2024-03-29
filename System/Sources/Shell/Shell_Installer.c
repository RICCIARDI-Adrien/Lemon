/** @file Shell.c
 * This is the custom Installer program shell.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Drivers/Driver_Hard_Disk.h> // For HardDiskWriteSector() and others
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_Screen.h>
#include <Error_Codes.h>
#include <File_System/File.h>
#include <File_System/File_System.h>
#include <Installer_Embedded_Files_Data.h>
#include <Shell.h>
#include <Standard_Functions.h> // To have atoi()
#include <Strings.h>

//-------------------------------------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------------------------------------
/** The sections title font color. */
#define SHELL_SECTION_TITLE_COLOR SCREEN_COLOR_LIGHT_BLUE

//-------------------------------------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Ask the user to remove installation media and reboot the computer. */
static void ShellReboot(void)
{
	ScreenSetColor(SCREEN_COLOR_BLUE);
	ScreenWriteString(STRING_SHELL_INSTALLER_REBOOT);
	KeyboardReadCharacter();
	KeyboardRebootSystem();
	while (1);
}

/** Copy the MBR code and the partition table at the partition starting sector.
 * @param Pointer_Lemon_Partition_Table The partition table to put on the Lemon MBR.
 */
static void ShellInstallMBR(TFileSystemMasterBootLoaderPartitionTableEntry *Pointer_Lemon_Partition_Table)
{
	unsigned char Sector_Temp[FILE_SYSTEM_SECTOR_SIZE_BYTES];
	
	// Extract the MBR code
	memcpy(Sector_Temp, Embedded_Files[0].Pointer_Data, FILE_SYSTEM_SECTOR_SIZE_BYTES);
	
	// Merge the partition table
	memcpy(&Sector_Temp[FILE_SYSTEM_MASTER_BOOT_LOADER_PARTITION_TABLE_OFFSET], Pointer_Lemon_Partition_Table, FILE_SYSTEM_MASTER_BOOT_LOADER_PARTITION_TABLE_SIZE);
	
	// Write to disk
	HardDiskWriteSector(Pointer_Lemon_Partition_Table[0].First_Sector_LBA, Sector_Temp);
}

/** Copy the kernel code just after the MBR.
 * @param Pointer_Kernel_File The file entry containing the kernel.
 * @param Starting_Sector Where to start installing the kernel (LBA addressing).
 */
static void ShellInstallKernel(TEmbeddedFile *Pointer_Kernel_File, unsigned int Starting_Sector)
{
	unsigned char *Pointer_Data;
	int i, Sectors_Count;
	
	// Compute the number of sectors the kernel needs
	Sectors_Count = Pointer_Kernel_File->Size_Bytes / FILE_SYSTEM_SECTOR_SIZE_BYTES;
	if (Pointer_Kernel_File->Size_Bytes % FILE_SYSTEM_SECTOR_SIZE_BYTES != 0) Sectors_Count++;
	
	Pointer_Data = Pointer_Kernel_File->Pointer_Data;
	for (i = 0; i < Sectors_Count; i++)
	{
		HardDiskWriteSector(Starting_Sector + i, Pointer_Data);
		Pointer_Data += FILE_SYSTEM_SECTOR_SIZE_BYTES;
	}
}

/** Install all remaining embedded files to the hard disk. */
static void ShellInstallFiles(void)
{
	int Embedded_Files_To_Install_Count, i;
	unsigned int File_Descriptor;
	
	// Compute the total number of embedded files
	Embedded_Files_To_Install_Count = sizeof(Embedded_Files) / sizeof(TEmbeddedFile);
	
	// Bypass MBR and kernel which were already installed
	for (i = 2; i < Embedded_Files_To_Install_Count; i++)
	{
		// Show file name and size
		ScreenWriteString(STRING_SHELL_INSTALLER_FILE_NAME_1);
		ScreenWriteString(Embedded_Files[i].String_Name);
		ScreenWriteString(STRING_SHELL_INSTALLER_FILE_NAME_2);
		ScreenWriteString(itoa(Embedded_Files[i].Size_Bytes));
		ScreenWriteString(STRING_SHELL_INSTALLER_FILE_NAME_3);
		
		// Try to open the file
		if (FileOpen(Embedded_Files[i].String_Name, 'w', &File_Descriptor) != ERROR_CODE_NO_ERROR)
		{
			ScreenSetColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_SHELL_INSTALLER_ERROR_CANT_OPEN_FILE);
			ShellReboot();
		}
		
		// Write the file content
		if (FileWrite(File_Descriptor, Embedded_Files[i].Pointer_Data, Embedded_Files[i].Size_Bytes) != ERROR_CODE_NO_ERROR)
		{
			ScreenSetColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_SHELL_INSTALLER_ERROR_CANT_WRITE_FILE_CONTENT);
			ShellReboot();
		}
		
		FileClose(File_Descriptor);
	}
}

/** Ask a question until the user answers yes or no. Yes and no characters are represented by two constants, so they can be customized for different languages.
 * @param String_Question The question.
 * @return 1 if the user answered "yes" or 0 if he chose "no".
 */
static int ShellAskYesNoQuestion(char *String_Question)
{
	char String_User_Answer[2];
	
	while (1)
	{
		ScreenWriteString(String_Question);
		
		KeyboardReadString(String_User_Answer, 1);
		if (String_User_Answer[0] == STRING_SHELL_INSTALLER_CHARACTER_YES) return 1;
		if (String_User_Answer[0] == STRING_SHELL_INSTALLER_CHARACTER_NO) return 0;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
void ShellInstallerDisplayTitle(char *String_Title)
{
	// Display the title in a different color
	ScreenSetColor(SHELL_SECTION_TITLE_COLOR);
	ScreenWriteString(String_Title);
	
	// Reset default installer color
	ScreenSetColor(SCREEN_COLOR_BLUE);
}

/** The installer specific code. */
void Shell(void)
{
	TFileSystemMasterBootLoaderPartitionTableEntry *Pointer_Lemon_Partition_Table, Default_Lemon_Partition_Table;
	unsigned int Partition_Starting_Sector, File_System_Starting_Sector;
	
	// Show the title
	ScreenSetColor(SCREEN_COLOR_LIGHT_BLUE);
	ScreenClear();
	ScreenWriteString(STRING_SHELL_INSTALLER_TITLE);
	
	ScreenSetColor(SCREEN_COLOR_BLUE);
	ScreenWriteString(STRING_SHELL_INSTALLER_WELCOME);
	
	// Ask the user on continuing the installation or not
	ShellInstallerDisplayTitle(STRING_SHELL_INSTALLER_SECTION_WARNING_TITLE);
	ScreenWriteString(STRING_SHELL_INSTALLER_SECTION_WARNING_MESSAGE);
	if (!ShellAskYesNoQuestion(STRING_SHELL_INSTALLER_SECTION_WARNING_QUESTION))
	{
		ScreenWriteString(STRING_SHELL_INSTALLER_SECTION_WARNING_INSTALLATION_ABORTED);
		ShellReboot();
	}
	
	// Ask the user whether he wants to use the whole disk or not
	ShellInstallerDisplayTitle(STRING_SHELL_INSTALLER_SECTION_HARD_DISK_TITLE);
	ScreenWriteString(STRING_SHELL_INSTALLER_SECTION_HARD_DISK_MESSAGE);
	if (ShellAskYesNoQuestion(STRING_SHELL_INSTALLER_SECTION_HARD_DISK_QUESTION))
	{
		// Fill the default partition table
		memset(&Default_Lemon_Partition_Table, 0, sizeof(Default_Lemon_Partition_Table)); // Partitions 1 to 3 are not used, so force them to zero
		Default_Lemon_Partition_Table.Status = 0x80; // Tell that the partition is bootable
		Default_Lemon_Partition_Table.Type = FILE_SYSTEM_MASTER_BOOT_LOADER_PARTITION_TABLE_PARTITION_TYPE_LEMON;
		Default_Lemon_Partition_Table.First_Sector_LBA = 0; // Start from the disk beginning
		Default_Lemon_Partition_Table.Sectors_Count = FileSystemComputeSizeSectors(CONFIGURATION_SYSTEM_FILE_SYSTEM_MAXIMUM_BLOCKS_LIST_ENTRIES, CONFIGURATION_SYSTEM_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES);
		Pointer_Lemon_Partition_Table = &Default_Lemon_Partition_Table;
	}
	else Pointer_Lemon_Partition_Table = ShellInstallerPartitionMenu(); // Select the installation partition
	
	// Compute the selected partition necessary offsets
	Partition_Starting_Sector = Pointer_Lemon_Partition_Table[0].First_Sector_LBA;
	File_System_Starting_Sector = Partition_Starting_Sector + CONFIGURATION_FILE_SYSTEM_STARTING_SECTOR_OFFSET;
	
	// Start the installation
	ShellInstallerDisplayTitle(STRING_SHELL_INSTALLER_INSTALLATION_BEGINNING);
	
	// Create file system
	ScreenWriteString(STRING_SHELL_INSTALLER_CREATING_FILE_SYSTEM);
	switch (FileSystemCreate(CONFIGURATION_SYSTEM_FILE_SYSTEM_MAXIMUM_BLOCKS_LIST_ENTRIES, CONFIGURATION_SYSTEM_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES, File_System_Starting_Sector))
	{
		case 1:
			ScreenSetColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_SHELL_INSTALLER_ERROR_BAD_FILE_SYSTEM_PARAMETERS);
			ShellReboot();
			break; // ShellReboot() does not return but this makes gcc 7 happy
			
		case 2:
			ScreenSetColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_SHELL_INSTALLER_ERROR_FILE_SYSTEM_TOO_BIG_FOR_HARD_DISK_1);
			ScreenWriteString(itoa(HardDiskGetDriveSizeSectors()));
			ScreenWriteString(STRING_SHELL_INSTALLER_ERROR_FILE_SYSTEM_TOO_BIG_FOR_HARD_DISK_2);
			ShellReboot();
			break;
			
		default:
			break;
	}
	FileSystemInitialize(File_System_Starting_Sector);
	
	// Install MBR
	ShellInstallMBR(Pointer_Lemon_Partition_Table);
	
	// Install kernel
	ShellInstallKernel(&Embedded_Files[1], Partition_Starting_Sector + 1);
	
	// Install remaining files
	ScreenWriteString(STRING_SHELL_INSTALLER_INSTALLING_FILES);
	ShellInstallFiles();
	
	// Installation has finished
	ScreenSetColor(SCREEN_COLOR_GREEN);
	ScreenWriteString(STRING_SHELL_INSTALLER_INSTALLATION_COMPLETED);
	
	ShellReboot();
}