/** @file Screen.h
 * @author Adrien RICCIARDI
 * @version 1.0 : 13/12/2013
 * @version 1.1 : 15/05/2014, added SCREEN_MAKE_COLOR().
 */
#ifndef H_SCREEN_H
#define H_SCREEN_H

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
/** Screen rows number. */
#define SCREEN_ROWS_COUNT 25
/** Screen columns number. */
#define SCREEN_COLUMNS_COUNT 80

/** Black color. */
#define SCREEN_COLOR_BLACK 0x00
/** Blue color. */
#define SCREEN_COLOR_BLUE 0x01
/** Green color. */
#define SCREEN_COLOR_GREEN 0x02
/** Cyan color. */
#define SCREEN_COLOR_CYAN 0x03
/** Red color. */
#define SCREEN_COLOR_RED 0x04
/** Magenta color. */
#define SCREEN_COLOR_MAGENTA 0x05
/** Brown color. */
#define SCREEN_COLOR_BROWN 0x06
/** Light gray color. */
#define SCREEN_COLOR_LIGHT_GRAY 0x07
/** Gray color. */
#define SCREEN_COLOR_GRAY 0x08
/** Light blue color. */
#define SCREEN_COLOR_LIGHT_BLUE 0x09
/** Light green color. */
#define SCREEN_COLOR_LIGHT_GREEN 0x0A
/** Light cyan color. */
#define SCREEN_COLOR_LIGHT_CYAN 0x0B
/** Light red color. */
#define SCREEN_COLOR_LIGHT_RED 0x0C
/** Light magenta color. */
#define SCREEN_COLOR_LIGHT_MAGENTA 0x0D
/** Yellow color. */
#define SCREEN_COLOR_YELLOW 0x0E
/** White color. */
#define SCREEN_COLOR_WHITE 0x0F

/** Create a complete color using a foreground and background colors.
 * @param Foreground_Color The text color.
 * @param Background_Color The background color.
 */
#define SCREEN_MAKE_COLOR(Foreground_Color, Background_Color) ((Background_Color << 4) | Foreground_Color)

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
// TODO enum avec les couleurs, changer paramètre couleur dans fonctions screen

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Clear the whole screen using the current background color. */
void ScreenClear(void);

/** Set the screen font color.
 * @param Color_Code Must be a constant from the SCREEN_COLOR_X set.
 */
void ScreenSetFontColor(unsigned char Color_Code);

/** Set the screen background color.
 * @param Color_Code Must be a constant from the SCREEN_COLOR_X set.
 */
void ScreenSetBackgroundColor(unsigned char Color_Code);

/** Get the screen cursor position.
 * @param Pointer_Row On output will contain the row coordinate.
 * @param Pointer_Column On output will contain the column coordinate.
 */
void ScreenGetCursorPosition(unsigned int *Pointer_Row, unsigned int *Pointer_Column);

/** Set the screen cursor position.
 * @param Row Row coordinate in range [0, SCREEN_ROWS_COUNT - 1].
 * @param Column Column coordinate in range [0, SCREEN_COLUMNS_COUNT - 1].
 * @note The function does nothing if the specified coordinates are out of the screen.
 */
void ScreenSetCursorPosition(unsigned int Row, unsigned int Column);

/** Display a buffer representing the raw video memory data on the screen.
 * @param Pointer_Buffer The buffer.
 * @note The buffer must be (SCREEN_ROWS_COUNT * SCREEN_COLUMNS_COUNT * 2) bytes large. Even byte is character ASCII code and odd byte is character color attributes.
 * @see IBM BIOS text mode 3 for more informations.
 */
void ScreenDisplayBuffer(unsigned char *Pointer_Buffer);

/** Display a single character.
 * @param Character The character to display.
 */
void ScreenWriteCharacter(char Character);

/** Display an ASCIIZ string.
 * @param String The string to display.
 */
void ScreenWriteString(char *String);

/** Display a centered ASCIIZ string.
 * @param String The string to display.
 */
void ScreenWriteCenteredString(char *String);

/** Display the string representation of a signed integer.
 * @param Integer The integer number to display.
 */
void ScreenWriteInteger(int Integer);

/** Display the string representation of an unsigned integer.
 * @param Unsigned_Integer The unsigned integer number to display.
 */
void ScreenWriteUnsignedInteger(unsigned int Unsigned_Integer);

#endif