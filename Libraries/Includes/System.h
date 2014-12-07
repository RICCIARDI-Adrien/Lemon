/** @file System.h
 * @author Adrien RICCIARDI
 * @version 1.0 : 27/04/2012
 * @version 1.1 : 13/12/2013, use kernel system calls definition to ease maintenance.
 */
#ifndef H_SYSTEM_H
#define H_SYSTEM_H

// Gather all the includes the system provides, so there is no need to include them in a program
#include <File.h>
#include <Keyboard.h>
#include <Memory.h>
#include <Random.h>
#include <Screen.h>
#include <String.h>
#include <System_Calls.h> // System calls codes imported directly from kernel
#include <UART.h>

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
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

// TODO : enum avec les erreurs (ou pas, à déterminer)

//-------------------------------------------------------------------------------------------------
// Types
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

#endif