/** @file System.h
 * Gather all header files. Only this header should be included in an application.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_H
#define H_SYSTEM_H

// Gather all the includes the system provides, so there is no need to include them in a program
#include <Error_Codes.h> // Error codes imported directly from the system
#include <Math.h>
#include <Network.h>
#include <System_Calls.h> // System calls codes imported directly from the system
#include <System_Configuration.h>
#include <System_File.h>
#include <System_Keyboard.h>
#include <System_Memory.h>
#include <System_Menu.h>
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
int LibrariesSystemCall(TSystemCall Request_Code, int Integer_1, int Integer_2, void *Pointer_1, void *Pointer_2);

/** Abort the current program and return to the system. */
void LibrariesSystemExitProgram(void);

#endif