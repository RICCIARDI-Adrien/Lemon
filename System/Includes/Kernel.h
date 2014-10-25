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
 */
#ifndef H_KERNEL_H
#define H_KERNEL_H

#include <Configuration.h>

//-------------------------------------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------------------------------------
#define NULL ((void *) 0)

//-------------------------------------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------------------------------------
// Kernel memory areas
/** The MBR stores file system informations. */
#define KERNEL_MBR_ADDRESS 0x7C00
/** Kernel stack address. */
#define KERNEL_STACK_ADDRESS 0x10000

// Userspace and user programs stuff
/** Amount of RAM allowed to user space. */
#define KERNEL_USER_SPACE_SIZE ((CONFIGURATION_SYSTEM_TOTAL_RAM_SIZE_MEGA_BYTES - 1) * 1024 * 1024)
/** User space base address. This reserves the first MB of RAM for the kernel and the second for the shell. */
#define KERNEL_USER_SPACE_ADDRESS 0x100000
/** Instruction pointer start address for user program. */
#define KERNEL_PROGRAM_ENTRY_POINT 0x100
/** User program will be loaded at this address. 256 bytes are reserved to store command line arguments. */
#define KERNEL_PROGRAM_LOAD_ADDRESS (KERNEL_USER_SPACE_ADDRESS + KERNEL_PROGRAM_ENTRY_POINT)
/** Executable program magic number. */
#define KERNEL_PROGRAM_MAGIC_NUMBER 0x83E58955 // This is the hexadecimal representation of the main() stack frame (push ebp...)

// Functions return codes
/** No error happened. */
#define ERROR_CODE_NO_ERROR 0
/** The File List is full. */
#define ERROR_CODE_FL_FULL 1
/** There are no more free block in the Blocks Allocation Table. */
#define ERROR_CODE_BAT_FULL 2
/** The provided file name is not valid. */
#define ERROR_CODE_BAD_FILE_NAME 3
/** The requested file doesn't exist in the file system. */
#define ERROR_CODE_FILE_NOT_FOUND 4
/** The file is already existing. */
#define ERROR_CODE_FILE_ALREADY_EXISTS 5
/** The maximum opened files count is reached. */
#define ERROR_CODE_CANT_OPEN_MORE_FILES 6
/** The file is opened yet. */
#define ERROR_CODE_FILE_OPENED_YET 7
/** A file was opened with an invalid mode. */
#define ERROR_CODE_UNKNOWN_OPENING_MODE 8
/** An attempt to read or write a file which was not previously opened was done. */
#define ERROR_CODE_FILE_NOT_OPENED 9
/** A file opened in read mode is accessed in write mode or a file opened in write mode is accessed in read mode. */
#define ERROR_CODE_BAD_OPENING_MODE 10
/** A bad file descriptor (value >= CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT) was provided to a file function. */
#define ERROR_CODE_BAD_FILE_DESCRIPTOR 11
/** Bad parameters were provided to UART during initialization. */
#define ERROR_CODE_BAD_UART_PARAMETERS 12
/** There is not enough room in RAM to load the file. */
#define ERROR_CODE_FILE_LARGER_THAN_RAM 13
/** The file is not an executable program. */
#define ERROR_CODE_FILE_NOT_EXECUTABLE 14

//-------------------------------------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Clear the kernel stack and jump to shell. */
void KernelStartShell(void);

/** Switch to protection ring 3 and execute user program's entry point. */
void KernelStartProgram(void);

#endif