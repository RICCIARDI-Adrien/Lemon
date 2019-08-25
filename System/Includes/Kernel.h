/** @file Kernel.h
 * Lemon 32-bit kernel.
 * @author Adrien RICCIARDI
 * @version 1.0 : 15/06/2011
 * @version 1.1 : 16/11/2011
 * @version 1.2 : 27/04/2012
 * @version 2.0 : 27/07/2012
 * @version 2.0.1 : 12/09/2012 (protection enabled 06/10/2012).
 * @version 2.0.2 : 22/12/2012, started shell implementation.
 * @version 2.0.3 : 29/12/2012, added shell to kernel in order to keep system simple.
 * @version 2.0.4 : 13/07/2013, global code optimization.
 * @version 2.1.0 : 28/09/2013, started Minimal Implementation Architecture improvements on the whole system.
 * @version 2.1.1 : 12/12/2013, removed Console module and created Screen module where drawing functions are now located, keyboard input functions previously located in Console are now in Keyboard.
 * @version 2.1.2 : 26/04/2014, added Includes directory, global source code reorganization and improvements.
 * @version 2.1.3 : 13/05/2014, cleaned and optimized memory protection code and interrupt code.
 * @version 2.1.4 : 07/06/2014, the system is now able to compile on 64-bit development system, file system is terminated.
 * @version 2.1.5 : 26/10/2014, removed 'help' command and added 'version' command.
 * @version 2.1.6 : 07/12/2014, fixed the memory protection limit bug.
 * @version 2.1.7 : 11/01/2015, moved the system error codes in a dedicated header file that can be shared with the libraries.
 * @version 2.1.8 : 26/02/2015, improved and fixed a bug in the file system code.
 * @version 2.1.9 : 10/05/2015, fixed a bug preventing the extended memory (above 1MB+64KB) to be enabled on old CPUs.
 * @version 2.1.10 : 29/05/2015, made the Installer boot from USB stick.
 * @version 2.2.0 : 06/06/2015, the system is now MBR partitions aware and can install to and boot from a partition. All hard disk drivers (MBR and system) are 48-LBA.
 * @version 2.2.1 : 03/11/2015, the hard disk driver automatically chooses between LBA28 and LBA48 at boot time.
 * @version 2.2.2 : 06/03/2016, better handling of slave PIC.
 * @version 2.3.0 : 12/04/2016, added PCI, AHCI and SATA support (but working only on virtual machine).
 * @version 2.3.1 : 06/05/2016, made the SATA driver works with real hardware.
 * @version 2.3.2 : 02/07/2016, added a header containing a magic number at the beginning of the executable files.
 * @version 2.3.3 : 19/08/2016, hard disk LBA mode must be configured manually (to give user more possibilities). Build configuration variables are gathered in the main makefile.
 * @version 2.4.0 : 21/09/2016, added networking support.
 * @version 2.4.1 : 03/10/2016, improved file system by using a list of free blocks.
 * @version 2.5.0 : 02/12/2016, added RAM disk support.
 * @version 2.5.1 : 19/12/2016, gathered Installer code into System code to avoid code and makefiles duplication.
 * @version 2.5.2 : 21/04/2017, improved "download" command communication protocol to allow files of any size to be downloaded.
 * @version 2.5.3 : 17/03/2018, added automatic computation of file system needed RAM and remaining user space RAM.
 * @version 2.5.4 : 25/08/2019, added support for "Autostart" automatic program launching on system boot.
 */
#ifndef H_KERNEL_H
#define H_KERNEL_H

#include <Error_Codes.h>

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Clear the kernel stack and jump to shell. */
void KernelStartShell(void);

/** Switch to protection ring 3 and execute user program's entry point.
 * @return ERROR_CODE_FILE_NOT_EXECUTABLE if the file in memory is not an executable program,
 * @return ERROR_CODE_NO_ERROR if the file was successfully launched (should never happen as the function will jump to protection ring 3 and never return).
 */
TErrorCode KernelStartProgram(void);

/** Called when an invalid system call is requested by the user. This function does not return. */
void KernelUnknownSystemCallErrorHandler(void);

#endif