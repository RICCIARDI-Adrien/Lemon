/** @file System.h
 * @author Adrien RICCIARDI
 * @version 1.0 : 27/04/2012
 * @version 1.1 : 13/12/2013, use kernel system calls definition to ease maintenance.
 */
#ifndef H_SYSTEM_H
#define H_SYSTEM_H

// Gather all the includes the system provides, so there is no need to include them in a program
#include <Error_Codes.h> // Error codes imported directly from the system
#include <File.h>
#include <Keyboard.h>
#include <Math.h>
#include <Memory.h>
#include <Random.h>
#include <Screen.h>
#include <String.h>
#include <System_Calls.h> // System calls codes imported directly from the system
#include <UART.h>

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The NULL pointer. */
#ifndef NULL
	#define NULL ((void *) 0)
#endif

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Request a system service.
 * @param Request_Code The service identifier code.
 * @param Integer_1 First integer parameter if needed.
 * @param Integer_2 Second integer parameter if needed.
 * @param Pointer_1 First pointer parameter if needed.
 * @param Pointer_2 Second pointer parameter if needed.
 * @return The service-specific return value.
 * @see System_Calls.h for all services description.
 */
int SystemCall(TSystemCall Request_Code, int Integer_1, int Integer_2, void *Pointer_1, void *Pointer_2);

/** Wait for a multiple of 50 ms.
 * @param Periods_Count How many period of 50 ms to wait.
 */
void SystemWait(unsigned int Periods_Count);

/** Abort the current program and return to the system. */
void SystemExitProgram(void);

/** Get the total file system size.
 * The file system can store up to Pointer_Files_Count files, and all files size must not exceed Pointer_Blocks_Count * Pointer_Block_Size bytes.
 * @param Pointer_Block_Size The size of a block in bytes. A block is the smaller storage unit to store file data.
 * @param Pointer_Blocks_Count How many blocks in the file system.
 * @param Pointer_Files_Count How many different files can be handled by the file system.
 */
void SystemGetFileSystemTotalSize(unsigned int *Pointer_Block_Size, unsigned int *Pointer_Blocks_Count, unsigned int *Pointer_Files_Count);

/** Get the timer current value. This timer is incremented by one every 50 ms and starts from zero on system boot.
 * @return The timer value (the unit is 50 ms).
 */
unsigned int SystemGetTimerValue(void);

#endif