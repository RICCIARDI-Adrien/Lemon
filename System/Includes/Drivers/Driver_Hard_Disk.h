/** @file Driver_Hard_Disk.h
 * Raw access to IDE hard disk.
 * @author Adrien RICCIARDI
 * @version 1.0 : 22/07/2011
 * @version 1.1 : 17/11/2011
 * @version 1.2 : 01/08/2012
 * @version 1.3 : 18/11/2012, heads count and sector / track count are now stored into the file system descriptor.
 * @version 1.4 : 24/11/2012, use of the 28-LBA addressing mode.
 * @version 1.5 : 10/08/2013, code cleanup and optimization.
 */
#ifndef H_DRIVER_HARD_DISK_H
#define H_DRIVER_HARD_DISK_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
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

/** Get the total size of the hard disk 0 in bytes.
 * @return The hard disk size in bytes.
 */
unsigned long long HardDiskGetDriveSizeBytes(void);

#endif
