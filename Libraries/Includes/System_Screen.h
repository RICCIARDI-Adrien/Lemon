/** @file System_Screen.h
 * Display data on the screen, modify cursor location and provide a direct access to the video memory for games.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_SCREEN_H
#define H_SYSTEM_SCREEN_H

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
/** Screen rows number. */
#define SCREEN_ROWS_COUNT 25
/** Screen columns number. */
#define SCREEN_COLUMNS_COUNT 80

/** Create a complete color using a foreground and a background colors.
 * @param Foreground_Color The text color.
 * @param Background_Color The background color.
 */
#define SCREEN_MAKE_COLOR(Foreground_Color, Background_Color) ((Background_Color << 4) | Foreground_Color)

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** All available colors. */
typedef enum
{
	SCREEN_COLOR_BLACK,
	SCREEN_COLOR_BLUE,
	SCREEN_COLOR_GREEN,
	SCREEN_COLOR_CYAN,
	SCREEN_COLOR_RED,
	SCREEN_COLOR_MAGENTA,
	SCREEN_COLOR_BROWN,
	SCREEN_COLOR_LIGHT_GRAY,
	SCREEN_COLOR_GRAY,
	SCREEN_COLOR_LIGHT_BLUE,
	SCREEN_COLOR_LIGHT_GREEN,
	SCREEN_COLOR_LIGHT_CYAN,
	SCREEN_COLOR_LIGHT_RED,
	SCREEN_COLOR_LIGHT_MAGENTA,
	SCREEN_COLOR_YELLOW,
	SCREEN_COLOR_WHITE
} TScreenColor;

/** A character representation in the VGA mode 3 video memory. */
typedef struct __attribute__((packed))
{
	char ASCII_Code; //! The character ASCII code.
	unsigned char Color; //! The character color.
} TScreenBufferCharacter;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Clear the whole screen using the current background color. */
void ScreenClear(void);

/** Set the screen font color.
 * @param Color_Code The color to use.
 */
void ScreenSetFontColor(TScreenColor Color_Code);

/** Set the screen background color.
 * @param Color_Code The color to use.
 */
void ScreenSetBackgroundColor(TScreenColor Color_Code);

/** Get the screen cursor position.
 * @param Pointer_Row On output will contain the row coordinate.
 * @param Pointer_Column On output will contain the column coordinate.
 */
void ScreenGetCursorPosition(unsigned int *Pointer_Row, unsigned int *Pointer_Column);

/** Set the screen cursor position.
 * @param Row Row coordinate in range [0, SCREEN_ROWS_COUNT - 1].
 * @param Column Column coordinate in range [0, SCREEN_COLUMNS_COUNT - 1].
 * @note The function does nothing if the specified coordinates are out of the screen bounds.
 */
void ScreenSetCursorPosition(unsigned int Row, unsigned int Column);

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

/** Display the hexadecimal representation of a 32-bit value (always padded to 8 characters).
 * @param Integer The 32-bit value to display (it can be also an unsigned integer).
 */
void ScreenWriteHexadecimalInteger(int Integer);

// "Rendering" functions
/** Display a buffer representing the raw video memory data on the screen.
 * @param Pointer_Buffer The buffer.
 * @note The buffer must be (SCREEN_ROWS_COUNT * SCREEN_COLUMNS_COUNT * 2) bytes large. Even byte is character ASCII code and odd byte is character color attributes.
 * @see IBM BIOS text mode 3 for more informations.
 */
void ScreenDisplayBuffer(void *Pointer_Buffer);

/** Scroll a SCREEN_ROWS_COUNT*SCREEN_COLUMNS_COUNT bytes buffer to one column on the left. The rightmost column is left as-is.
 * @param Pointer_Buffer The buffer to scroll.
 */
void ScreenScrollBufferToLeft(void *Pointer_Buffer);

/** Clear the buffer with 'space' characters and the specified color.
 * @param Pointer_Buffer The buffer to clear.
 * @param Color The clearing color (use SCREEN_MAKE_COLOR() to specify it).
 */
void ScreenClearBuffer(void *Pointer_Buffer, unsigned char Color);

#endif
