/** @file Configuration.h
 * Gather all program configurable parameters.
 * @author Adrien RICCIARDI
 * @version 1.0 : 08/08/2015
 */
#ifndef H_CONFIGURATION_H
#define H_CONFIGURATION_H

#include <System.h>

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How many characters the buffer can contain. */
#define CONFIGURATION_BUFFER_MAXIMUM_SIZE (10 * 1024 * 1024)

/** A line length in characters (i.e. the screen width). */
#define CONFIGURATION_SCREEN_COLUMNS_COUNT SCREEN_COLUMNS_COUNT
/** How many lines can be displayed in the same time on the screen. */
#define CONFIGURATION_SCREEN_ROWS_COUNT SCREEN_ROWS_COUNT

/** The cursor color. */
#define CONFIGURATION_CURSOR_COLOR SCREEN_COLOR_LIGHT_GREEN

#endif