/** @file Configuration.h
 * Gather all program configurable parameters.
 * @author Adrien RICCIARDI
 */
#ifndef H_CONFIGURATION_H
#define H_CONFIGURATION_H

#include <System.h>

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** How many characters the buffer can contain. */
#define CONFIGURATION_BUFFER_MAXIMUM_SIZE (10 * 1024 * 1024)

/** How many characters the copy buffer can contain. */
#define CONFIGURATION_COPY_BUFFER_MAXIMUM_SIZE (SYSTEM_SCREEN_ROWS_COUNT * SYSTEM_SCREEN_COLUMNS_COUNT * 10) // Up to 10 full pages

/** A line length in characters (i.e. the display width). */
#define CONFIGURATION_DISPLAY_COLUMNS_COUNT SYSTEM_SCREEN_COLUMNS_COUNT
/** How many lines can be displayed in the same time on the display. */
#define CONFIGURATION_DISPLAY_ROWS_COUNT SYSTEM_SCREEN_ROWS_COUNT

/** The text foreground color. */
#define CONFIGURATION_TEXT_FOREGROUND_COLOR SYSTEM_SCREEN_COLOR_BLUE
/** The text background color. */
#define CONFIGURATION_TEXT_BACKGROUND_COLOR SYSTEM_SCREEN_COLOR_WHITE
/** The cursor background color. */
#define CONFIGURATION_CURSOR_BACKGROUND_COLOR SYSTEM_SCREEN_COLOR_LIGHT_GREEN

#endif