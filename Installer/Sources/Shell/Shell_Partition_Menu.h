/** @file Shell_Partition_Menu.h
 * Display all existing MBR partitions and allow to pick one of them to install the system on it.
 * @author Adrien RICCIARDI
 * @version 1.0 : 01/06/2015
 */
#ifndef H_SHELL_PARTITION_MENU_H
#define H_SHELL_PARTITION_MENU_H

//-------------------------------------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------------------------------------
/** A partition table contains 4 entries. */
#define SHELL_PARTITION_MENU_PARTITION_TABLE_ENTRIES_COUNT 4
/** Where to locate the partition table in the MBR sector. */
#define SHELL_PARTITION_MENU_PARTITION_TABLE_OFFSET 446
/** The partition table total size in bytes. */
#define SHELL_PARTITION_MENU_PARTITION_TABLE_SIZE (SHELL_PARTITION_MENU_PARTITION_TABLE_ENTRIES_COUNT * sizeof(TShellPartitionMenuPartitionTableEntry))

/** The Lemon partition type. */
#define SHELL_PARTITION_MENU_LEMON_PARTITION_TYPE 0x60 // Same as system interrupt calls

//-------------------------------------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------------------------------------
/** A partition table entry. */
typedef struct __attribute__((packed))
{
	unsigned char Status; //! Bit 7 tells if the partition is active (i.e. bootable) or not.
	unsigned char First_Sector_Head; //! CHS head of the partition first sector.
	unsigned char First_Sector_Sector_And_Cylinder_MSB; //! CHS sector and cylinder bits 9..8 of the partition first sector.
	unsigned char First_Sector_Cylinder; //! CHS cylinder bits 7..0 of the partition first sector.
	unsigned char Type; //! The partition type.
	unsigned char Last_Sector_Head; //! CHS head of the partition last sector.
	unsigned char Last_Sector_Sector_And_Cylinder_MSB; //! CHS sector and cylinder bits 9..8 of the partition last sector.
	unsigned char Last_Sector_Cylinder; //! CHS cylinder bits 7..0 of the partition last sector.
	unsigned int First_Sector_LBA; //! The partition first sector in LBA addressing.
	unsigned int Sectors_Count; //! How many sectors in the partition.
} TShellPartitionMenuPartitionTableEntry;

//-------------------------------------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Allow the user to choose one of the available partitions.
 * @return A pointer on the corresponding partition table to write to the Lemon MBR.
 */
TShellPartitionMenuPartitionTableEntry *ShellPartitionMenu(void);

#endif