/** @file Debug.h
 * Compile the debug informations only if the debug mode is enabled.
 * @author Adrien RICCIARDI
 */
#ifndef H_DEBUG_H
#define H_DEBUG_H

#include "Configuration.h"

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
#if CONFIGURATION_IS_DEBUG_ENABLED == 1
	/** Begin debugging informations. */
	#define DEBUG_SECTION_START
	/** Terminate debugging informations. */
	#define DEBUG_SECTION_END
#else
	/** Begin debugging informations. */
	#define DEBUG_SECTION_START if (0) { // Gcc will automatically discard this section, even in non-optimization mode
	/** Terminate debugging informations. */
	#define DEBUG_SECTION_END }
#endif

#endif