/** @file System.h
 * Gather all header files. Only this header should be included in an application.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_H
#define H_SYSTEM_H

// Gather all the includes the system provides, so there is no need to include them in a program
#include <Error_Codes.h> // Error codes imported directly from the system
#include <Math.h>
#include <Menu.h>
#include <Network.h>
#include <System_Calls.h> // System calls codes imported directly from the system
#include <System_File.h>
#include <System_Keyboard.h>
#include <System_Memory.h>
#include <System_Random.h>
#include <System_Screen.h>
#include <System_String.h>
#include <System_Timer.h>
#include <System_UART.h>

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
/** The NULL pointer. */
#ifndef NULL
	#define NULL ((void *) 0)
#endif

/** Give the number of elements in an array. */
#define SYSTEM_GET_ARRAY_ELEMENTS_COUNT(Array) (sizeof(Array) / sizeof(Array[0]))

/** Convert the macro identifier to a C string. */
#define SYSTEM_CONVERT_MACRO_NAME_TO_STRING(X) #X
/** Convert the macro value to a C string. The preprocessor needs two passes to do the conversion, so the SYSTEM_CONVERT_MACRO_NAME_TO_STRING() is needed. */
#define SYSTEM_CONVERT_MACRO_VALUE_TO_STRING(X) SYSTEM_CONVERT_MACRO_NAME_TO_STRING(X)

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

/** Abort the current program and return to the system. */
void SystemExitProgram(void);

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