/** @file Driver_Screen.h
 * Handle 80x25 text mode screen.
 * @author Adrien RICCIARDI
 * @version 1.0 : 12/12/2013
 */
#ifndef H_DRIVER_SCREEN_H
#define H_DRIVER_SCREEN_H

//------------------------------------------------------------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------------------------------------------------------------
/** Video RAM base address. */
#define SCREEN_MEMORY_ADDRESS 0xB8000

/** Number of screen columns. */
#define SCREEN_COLUMNS_COUNT 80
/** Number of screen rows. */
#define SCREEN_ROWS_COUNT 25

/** Black color. */
#define SCREEN_COLOR_BLACK 0xF0
/** Blue color. */
#define SCREEN_COLOR_BLUE 0xF1
/** Green color. */
#define SCREEN_COLOR_GREEN 0xF2
/** Cyan color. */
#define SCREEN_COLOR_CYAN 0xF3
/** Red color. */
#define SCREEN_COLOR_RED 0xF4
/** Magenta color. */
#define SCREEN_COLOR_MAGENTA 0xF5
/** Brown color. */
#define SCREEN_COLOR_BROWN 0xF6
/** Light gray color. */
#define SCREEN_COLOR_LIGHT_GRAY 0xF7
/** Gray color. */
#define SCREEN_COLOR_GRAY 0xF8
/** Light blue color. */
#define SCREEN_COLOR_LIGHT_BLUE 0xF9
/** Light green color. */
#define SCREEN_COLOR_LIGHT_GREEN 0xFA
/** Light cyan color. */
#define SCREEN_COLOR_LIGHT_CYAN 0xFB
/** Light red color. */
#define SCREEN_COLOR_LIGHT_RED 0xFC
/** Light magenta color. */
#define SCREEN_COLOR_LIGHT_MAGENTA 0xFD
/** Yellow color. */
#define SCREEN_COLOR_YELLOW 0xFE
/** White color. */
#define SCREEN_COLOR_WHITE 0xFF

//------------------------------------------------------------------------------------------------------------------------------------
// Public variables
//------------------------------------------------------------------------------------------------------------------------------------
extern unsigned char Screen_Color;

//------------------------------------------------------------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------------------------------------------------------------
/** Clear the whole screen using the current background color and set cursor location to (0,0). */
void ScreenClear(void);

/** Write a character to the screen.
 * @param Character ASCII value of the character to print.
 * @note This function handles the new line '\\n' and backspace (\\b) characters.
 */
void ScreenWriteChar(char Character);

/** Print an ASCIIZ string to the screen.
 * @param String Pointer on the string to print.
 * @note This function handles the new line '\\n' and backspace (\\b) characters.
 */
void ScreenWriteString(char *String);

/** Get the current cursor position.
 * @param Pointer_Row Will hold the Y coordinate.
 * @param Pointer_Column Will hold the X coordinate.
 */
void ScreenGetCursorPosition(unsigned int *Pointer_Row, unsigned int *Pointer_Column);

/** Set the cursor position. Do nothing if the coordinates are not in screen ranges.
 * @param Row The Y coordinate (must be in range [0, SCREEN_ROWS_COUNT-1].
 * @param Column The X coordinate (must be in range [0, SCREEN_COLUMNS_COUNT-1].
 */
void ScreenSetCursorPosition(unsigned int Row, unsigned int Column);

/** Get the current text color.
 * @return The current color code.
 */
#ifdef DOXYGEN
	unsigned char ScreenGetColor(void);
#else
	#define ScreenGetColor() (Screen_Color)
#endif

/** Set the text color.
 * @param Color_Code The color code value represented by a SCREEN_COLOR_XXX macro.
 */
#ifdef DOXYGEN
	void ScreenSetColor(unsigned char Color_Code);
#else
	#define ScreenSetColor(Color_Code) (Screen_Color = Color_Code)
#endif

/** Display a buffer representing the raw video memory data on the screen.
 * @param Pointer_Buffer The userspace-provided buffer.
 * @note The buffer must be (SCREEN_ROWS_COUNT * SCREEN_COLUMNS_COUNT * 2) bytes large. Even byte is character ASCII code and odd byte is character color attributes.
 */
void ScreenDisplayBuffer(unsigned char *Pointer_Buffer);

#endif