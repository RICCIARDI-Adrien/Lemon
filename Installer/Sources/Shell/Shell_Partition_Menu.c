/** @file Shell_Partition_Menu.c
 * @see Shell_Partition_Menu.h for description.
 * @author Adrien RICCIARDI
 */
#include <Drivers/Driver_Hard_Disk.h> // For HardDiskWriteSector() and others
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_Screen.h>
#include <File_System/File_System.h>
#include <Standard_Functions.h>
#include "Shell.h"
#include "Shell_Partition_Menu.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
TShellPartitionMenuPartitionTableEntry *ShellPartitionMenu(void)
{
	int i, Partition_Number;
	unsigned char MBR_Sector_Buffer[FILE_SYSTEM_SECTOR_SIZE_BYTES];
	TShellPartitionMenuPartitionTableEntry Partition_Table[SHELL_PARTITION_MENU_PARTITION_TABLE_ENTRIES_COUNT];
	char String_User_Answer[2];
	static TShellPartitionMenuPartitionTableEntry Lemon_Partition_Table[SHELL_PARTITION_MENU_PARTITION_TABLE_ENTRIES_COUNT];
	
	ShellDisplayTitle(STRING_PARTITION_MENU_SHOW_PARTITION_TABLE_1);
	ScreenWriteString(STRING_PARTITION_MENU_SHOW_PARTITION_TABLE_2);
	
	// Load the partition table from the MBR
	HardDiskReadSector(0, MBR_Sector_Buffer);
	memcpy(Partition_Table, MBR_Sector_Buffer + SHELL_PARTITION_MENU_PARTITION_TABLE_OFFSET, sizeof(Partition_Table));
	
	// Display it
	for (i = 0; i < SHELL_PARTITION_MENU_PARTITION_TABLE_ENTRIES_COUNT; i++)
	{
		// Partition number
		ScreenWriteString(STRING_PARTITION_MENU_PARTITIONS_DETAILS_1);
		ScreenWriteString(itoa(i + 1)); // Partitions start from 1
		ScreenWriteString(STRING_PARTITION_MENU_PARTITIONS_DETAILS_2);
		ScreenWriteString(itoa(Partition_Table[i].Type));
		if (Partition_Table[i].Type != 0)
		{
			ScreenWriteString(STRING_PARTITION_MENU_PARTITIONS_DETAILS_3);
			ScreenWriteString(itoa(Partition_Table[i].First_Sector_LBA));
			ScreenWriteString(STRING_PARTITION_MENU_PARTITIONS_DETAILS_4);
			ScreenWriteString(itoa(Partition_Table[i].Sectors_Count * FILE_SYSTEM_SECTOR_SIZE_BYTES));
		}
		else ScreenWriteString(STRING_PARTITION_MENU_PARTITIONS_DETAILS_EMPTY_PARTITION);
		ScreenWriteCharacter('\n');
	}
	
	// Ask the partition number
	do
	{
		ScreenWriteString(STRING_PARTITION_MENU_CHOOSE_PARTITION_NUMBER);
		KeyboardReadString(String_User_Answer, 1);
	} while ((String_User_Answer[0] < '1') || (String_User_Answer[0] > '4'));
	Partition_Number = String_User_Answer[0] - '0' - 1; // Convert the number to an index in the partition table (-1 is because partitions start from 1)

	// Fill the first Lemon MBR partition table with the selected entry
	memcpy(&Lemon_Partition_Table[0], &Partition_Table[Partition_Number], sizeof(TShellPartitionMenuPartitionTableEntry));
	// Set the Lemon partition type
	Lemon_Partition_Table[0].Type = SHELL_PARTITION_MENU_LEMON_PARTITION_TYPE;
	// Clear all other entries
	memset(&Lemon_Partition_Table[1], 0, sizeof(TShellPartitionMenuPartitionTableEntry) * (SHELL_PARTITION_MENU_PARTITION_TABLE_ENTRIES_COUNT - 1));
	
	return Lemon_Partition_Table;
}