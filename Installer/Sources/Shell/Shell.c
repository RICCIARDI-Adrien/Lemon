/** @file Shell.c
 * This is the custom Installer program shell.
 * @author Adrien RICCIARDI
 * @version 1.0 : 26/04/2014
 */
#include <Drivers/Driver_Hard_Disk.h> // For HardDiskWriteSector() and others
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_Screen.h>
#include <File_System/File.h>
#include <File_System/File_System.h>
#include <Standard_Functions.h> // To have atoi()
#include "Embedded_Files_Data.h"
#include "Shell_Partition_Menu.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Ask the user to remove installation media and reboot the computer. */
static void ShellReboot(void)
{
	ScreenSetColor(SCREEN_COLOR_BLUE);
	ScreenWriteString(STRING_REBOOT);
	KeyboardReadCharacter();
	KeyboardRebootSystem();
	while (1);
}

/** Copy the MBR code and the partition table at the partition starting sector.
 * @param Pointer_Lemon_Partition_Table The partition table to put on the Lemon MBR.
 */
static void ShellInstallMBR(TShellPartitionMenuPartitionTableEntry *Pointer_Lemon_Partition_Table)
{
	unsigned char Sector_Temp[FILE_SYSTEM_SECTOR_SIZE_BYTES];
	
	// Extract the MBR code
	memcpy(Sector_Temp, Embedded_Files[0].Pointer_Data, FILE_SYSTEM_SECTOR_SIZE_BYTES);
	
	// Merge the partition table
	memcpy(&Sector_Temp[SHELL_PARTITION_MENU_PARTITION_TABLE_OFFSET], Pointer_Lemon_Partition_Table, SHELL_PARTITION_MENU_PARTITION_TABLE_SIZE);
	
	// Write on disk
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
	
	// Compute the total number of embedded files
	Embedded_Files_To_Install_Count = sizeof(Embedded_Files) / sizeof(TEmbeddedFile);
	
	// Bypass MBR and kernel which were already installed
	for (i = 2; i < Embedded_Files_To_Install_Count; i++)
	{
		// Show file name and size
		ScreenWriteString(STRING_FILE_NAME_1);
		ScreenWriteString(Embedded_Files[i].String_Name);
		ScreenWriteString(STRING_FILE_NAME_2);
		ScreenWriteString(itoa(Embedded_Files[i].Size_Bytes));
		ScreenWriteString(STRING_FILE_NAME_3);
		
		if (FileCreate(Embedded_Files[i].String_Name, Embedded_Files[i].Pointer_Data, Embedded_Files[i].Size_Bytes) != 0)
		{
			ScreenSetColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_ERROR_CANT_CREATE_FILE);
			ShellReboot();
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
/** The installer specific code. */
void Shell(void)
{
	char String_User_Answer[2];
	TShellPartitionMenuPartitionTableEntry *Pointer_Lemon_Partition_Table;
	unsigned int Partition_Starting_Sector, File_System_Starting_Sector;
	
	// Show title
	ScreenSetColor(SCREEN_COLOR_LIGHT_BLUE);
	ScreenClear();
	ScreenWriteString(STRING_TITLE);
	
	ScreenSetColor(SCREEN_COLOR_BLUE);
	ScreenWriteString(STRING_WELCOME);
	
	ScreenSetColor(SCREEN_COLOR_RED);
	ScreenWriteString(STRING_CAUTION_MESSAGE_1);
	
	// Ask the user on continuing the installation or not
	ScreenSetColor(SCREEN_COLOR_BLUE);
	while (1)
	{
		ScreenWriteString(STRING_CAUTION_MESSAGE_2);
		
		KeyboardReadString(String_User_Answer, 1);
		if (String_User_Answer[0] == STRING_CHARACTER_YES) break;
		if (String_User_Answer[0] == STRING_CHARACTER_NO)
		{
			ScreenSetColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_INSTALLATION_ABORTED);
			ShellReboot();
		}
	}
	
	// Select the installation partition
	Pointer_Lemon_Partition_Table = ShellPartitionMenu();
	Partition_Starting_Sector = Pointer_Lemon_Partition_Table[0].First_Sector_LBA;
	File_System_Starting_Sector = Partition_Starting_Sector + CONFIGURATION_FILE_SYSTEM_STARTING_SECTOR_OFFSET;
	
	// Start the installation
	ScreenSetColor(SCREEN_COLOR_LIGHT_BLUE);
	ScreenWriteString(STRING_INSTALLATION_BEGINNING);
	ScreenSetColor(SCREEN_COLOR_BLUE);
	
	// Create file system
	ScreenWriteString(STRING_CREATING_FILE_SYSTEM);
	switch (FileSystemCreate(2048, 128, File_System_Starting_Sector))
	{
		case 1:
			ScreenSetColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_ERROR_BAD_FILE_SYSTEM_PARAMETERS);
			ShellReboot();
		case 2:
			ScreenSetColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_ERROR_FILE_SYSTEM_TOO_BIG_FOR_HARD_DISK_1);
			ScreenWriteString(itoa(HardDiskGetDriveSizeBytes()));
			ScreenWriteString(STRING_ERROR_FILE_SYSTEM_TOO_BIG_FOR_HARD_DISK_2);
			ShellReboot();
		default:
			break;
	}
	FileSystemInitialize(File_System_Starting_Sector);
	
	// Install MBR
	ShellInstallMBR(Pointer_Lemon_Partition_Table);
	
	// Install kernel
	ShellInstallKernel(&Embedded_Files[1], Partition_Starting_Sector + 1);
	
	// Install remaining files
	ScreenWriteString(STRING_INSTALLING_FILES);
	ShellInstallFiles();
	
	// Installation has finished
	ScreenSetColor(SCREEN_COLOR_GREEN);
	ScreenWriteString(STRING_INSTALLATION_COMPLETED);
	
	ShellReboot();
}