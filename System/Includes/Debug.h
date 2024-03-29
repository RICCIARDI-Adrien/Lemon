/** @file Debug.h
 * Compile the debug informations only if the debug mode is enabled.
 * @author Adrien RICCIARDI
 */
#ifndef H_DEBUG_H
#define H_DEBUG_H

#include <Configuration.h>
// The following includes must always be included or the disabled debug code (between if (0) {}) will issue warnings due to undefined ScreenXXX() functions
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_Screen.h> // The debugging code will write informations to the screen
#include <Standard_Functions.h> // The debugging code will certainly use itoa()

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
#ifdef CONFIGURATION_SYSTEM_IS_DEBUG_ENABLED
	/** Begin debugging informations. */
	#define DEBUG_SECTION_START
	/** Terminate debugging informations. */
	#define DEBUG_SECTION_END
	
	/** Display the current function name between braces. */
	#define DEBUG_DISPLAY_CURRENT_FUNCTION_NAME() \
	{ \
		ScreenSetColor(SCREEN_COLOR_MAGENTA); \
		ScreenWriteCharacter('['); \
		ScreenWriteString((char *) __func__); \
		ScreenWriteString("] "); \
		ScreenSetColor(SCREEN_COLOR_BLUE); \
	}
#else
	/** Begin debugging informations. */
	#define DEBUG_SECTION_START if (0) { // Gcc will automatically discard this section, even in non-optimization mode
	/** Terminate debugging informations. */
	#define DEBUG_SECTION_END }
	
	/** Display the current function name between braces. */
	#define DEBUG_DISPLAY_CURRENT_FUNCTION_NAME()
#endif

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Display a byte value converted to hexadecimal.
 * @param Byte The value to display.
 */
void DebugWriteHexadecimalByte(unsigned char Byte);

/** Display a double-word value converted to hexadecimal.
 * @param Integer The value to display.
 */
void DebugWriteHexadecimalInteger(unsigned int Integer);

#endif