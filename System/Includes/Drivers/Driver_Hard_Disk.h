/** @file Driver_Hard_Disk.h
 * Generic hard disk access functions.
 * @author Adrien RICCIARDI
 */
#ifndef H_DRIVER_HARD_DISK_H
#define H_DRIVER_HARD_DISK_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** A standard hard disk sector size in bytes. */
#define HARD_DISK_SECTOR_SIZE 512

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Probe the hard disk to determine the LBA addressing to use.
 * @return 0 if the LBA addressing was successfully found,
 * @return 1 if the disk does not support LBA addressing,
 * @return 2 if the SATA disk was not found.
 */
int HardDiskInitialize(void);

/** Read a logical sector (512 bytes width) from the first IDE hard drive.
 * @param Logical_Sector_Number The LBA sector to read.
 * @param Pointer_Buffer A pointer on a 512 bytes buffer where the data will be stored.
 */
void HardDiskReadSector(unsigned int Logical_Sector_Number, void *Pointer_Buffer);

/** Write the data contained in Buffer to a hard disk logical sector.
 * @param Logical_Sector_Number The LBA sector to write.
 * @param Pointer_Buffer A pointer on a 512 bytes buffer containing data to write.
 */
void HardDiskWriteSector(unsigned int Logical_Sector_Number, void *Pointer_Buffer);

/** Get the total size of the hard disk 0 in sectors.
 * @return The hard disk size in sectors.
 */
unsigned int HardDiskGetDriveSizeSectors(void);

#endif
