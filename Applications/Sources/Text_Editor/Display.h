/** @file Display.h
 * Use the Lemon feature to directly render on the screen, so scrolling is faster and the whole screen can be used.
 * @author Adrien RICCIARDI
 */
#ifndef H_DISPLAY_H
#define H_DISPLAY_H

#include <System.h>

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Erase the whole display with the default background color and put the cursor at the upper left corner. */
void DisplayClear(void);

/** Display a single character.
 * @param Character The character to display.
 * @warning Scrolling is not supported, do not write more characters than the display size !
 */
void DisplayWriteCharacter(char Character);

/** Set the display background color.
 * @param Color_Code The color to use.
 */
void DisplaySetBackgroundColor(TSystemScreenColor Color_Code);

/** Get the display cursor position.
 * @param Pointer_Row On output will contain the row coordinate.
 * @param Pointer_Column On output will contain the column coordinate.
 */
void DisplayGetCursorPosition(unsigned int *Pointer_Row, unsigned int *Pointer_Column);

/** Set the display cursor position.
 * @param Row Row coordinate in range [0, CONFIGURATION_DISPLAY_ROWS_COUNT - 1].
 * @param Column Column coordinate in range [0, CONFIGURATION_DISPLAY_COLUMNS_COUNT - 1].
 * @note The function does nothing if the specified coordinates are out of the display bounds.
 */
void DisplaySetCursorPosition(unsigned int Row, unsigned int Column);

/** Send the display buffer to the video memory. */
void DisplayRenderToScreen(void);

#endif