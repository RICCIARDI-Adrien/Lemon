/** @file System.h
 * Gather all header files. Only this header should be included in an application.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_H
#define H_SYSTEM_H

// Gather all the includes the system provides, so there is no need to include them in a program
#include <Error_Codes.h> // Error codes imported directly from the system
#include <File.h>
#include <Keyboard.h>
#include <Math.h>
#include <Memory.h>
#include <Menu.h>
#include <Network.h>
#include <Random.h>
#include <Screen.h>
#include <String.h>
#include <System_Calls.h> // System calls codes imported directly from the system
#include <UART.h>

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
/** The NULL pointer. */
#ifndef NULL
	#define NULL ((void *) 0)
#endif

/** Give the number of elements in an array. */
#define SYSTEM_GET_ARRAY_ELEMENTS_COUNT(Array) (sizeof(Array) / sizeof(Array[0]))

/** The configuration file name. */
#define SYSTEM_CONFIGURATION_FILE_NAME "System.cfg"
/** The maximum length in characters of a value string. */
#define SYSTEM_CONFIGURATION_FILE_MAXIMUM_VALUE_SIZE 128

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

/** Wait for the specified number of milliseconds.
 * @param Periods_Count How many milliseconds to wait.
 */
void SystemWait(unsigned int Milliseconds_Count);

/** Abort the current program and return to the system. */
void SystemExitProgram(void);

/** Get the total file system size.
 * The file system can store up to Pointer_Files_Count files, and all files size must not exceed Pointer_Blocks_Count * Pointer_Block_Size bytes.
 * @param Pointer_Block_Size The size of a block in bytes. A block is the smaller storage unit to store file data.
 * @param Pointer_Blocks_Count How many blocks in the file system.
 * @param Pointer_Files_Count How many different files can be handled by the file system.
 */
void SystemGetFileSystemTotalSize(unsigned int *Pointer_Block_Size, unsigned int *Pointer_Blocks_Count, unsigned int *Pointer_Files_Count);

/** Get the file system free space.
 * @param Pointer_Blocks_Count On output, contain the amount of available blocks.
 * @param Pointer_Files_Count On output, contain the amount of available Files List entries.
 */
void SystemGetFileSystemFreeSize(unsigned int *Pointer_Blocks_Count, unsigned int *Pointer_Files_Count);

/** Get the timer current value. This timer is incremented by one every millisecond and starts from zero on system boot.
 * @return The timer value (the unit is 1 ms).
 */
unsigned int SystemGetTimerValue(void);

/** Read a configuration value from the configuration file.
 * @param String_Key The value key (the string that identifies the value).
 * @param String_Value On output, contain the requested value. This buffer must be large enough to hold the whole value.
 * @return 0 if the value was successfully retrieved,
 * @return 1 if the configuration file was not found,
 * @return 2 if the provided key is empty,
 * @return 3 if a read error occurred,
 * @return 4 if the key was not found.
 */
int SystemConfigurationReadValue(char *String_Key, char *String_Value);

#endif