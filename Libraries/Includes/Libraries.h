/** @file Libraries.h
 * Gather all header files. Only this header should be included in an application.
 * @author Adrien RICCIARDI
 */
#ifndef H_LIBRARIES_H
#define H_LIBRARIES_H

// Gather all the includes the system provides, so there is no need to include them in a program
#include <Error_Codes.h> // Error codes imported directly from the system
#include <Math.h>
#include <Network.h>
#include <Libraries_Configuration.h>
#include <Libraries_File.h>
#include <Libraries_Keyboard.h>
#include <Libraries_Memory.h>
#include <Libraries_Menu.h>
#include <Libraries_Random.h>
#include <Libraries_Screen.h>
#include <Libraries_String.h>
#include <Libraries_System.h>
#include <Libraries_Timer.h>
#include <Libraries_UART.h>
#include <System_Calls.h> // System calls codes imported directly from the system

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
/** The NULL pointer. */
#ifndef NULL
	#define NULL ((void *) 0)
#endif

/** Give the number of elements in an array. */
#define LIBRARIES_GET_ARRAY_ELEMENTS_COUNT(Array) (sizeof(Array) / sizeof(Array[0]))

/** Convert the macro identifier to a C string. */
#define LIBRARIES_CONVERT_MACRO_NAME_TO_STRING(X) #X
/** Convert the macro value to a C string. The preprocessor needs two passes to do the conversion, so the LIBRARIES_CONVERT_MACRO_NAME_TO_STRING() is needed. */
#define LIBRARIES_CONVERT_MACRO_VALUE_TO_STRING(X) LIBRARIES_CONVERT_MACRO_NAME_TO_STRING(X)

#endif
