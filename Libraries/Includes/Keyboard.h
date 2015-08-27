/** @file Keyboard.h
 * @author Adrien RICCIARDI
 * @version 1.0 : 18/12/2013
 */
#ifndef H_KEYBOARD_H
#define H_KEYBOARD_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Keyboard enter key. */
#define KEYBOARD_KEY_CODE_ENTER '\n'
/** Keyboard escape key. */
#define KEYBOARD_KEY_CODE_ESCAPE 27
/** Keyboard up arrow key. */
#define KEYBOARD_KEY_CODE_ARROW_UP 224
/** Keyboard down arrow key. */
#define KEYBOARD_KEY_CODE_ARROW_DOWN 225
/** Keyboard left arrow key. */
#define KEYBOARD_KEY_CODE_ARROW_LEFT 226
/** Keyboard right arrow key. */
#define KEYBOARD_KEY_CODE_ARROW_RIGHT 227
/** Keyboard page up key. */
#define KEYBOARD_KEY_CODE_PAGE_UP 228
/** Keyboard page down key. */
#define KEYBOARD_KEY_CODE_PAGE_DOWN 229
/** Keyboard Origin key. */
#define KEYBOARD_KEY_CODE_ORIGIN 230
/** Keyboard end key. */
#define KEYBOARD_KEY_CODE_END 231
/** Keyboard insert key. */
#define KEYBOARD_KEY_CODE_INSERT 232
/** Keyboard delete key. */
#define KEYBOARD_KEY_CODE_DELETE 233
/** Keyboard Windows left key. */
#define KEYBOARD_KEY_CODE_WINDOWS_LEFT 234
/** Keyboard Windows right key. */
#define KEYBOARD_KEY_CODE_WINDOWS_RIGHT 235
/** Keyboard right mouse menu click key. */
#define KEYBOARD_KEY_CODE_RIGHT_MENU 236
/** Keyboard left control. */
#define KEYBOARD_KEY_CODE_CONTROL_LEFT 237
/** Keyboard alt. */
#define KEYBOARD_KEY_CODE_ALT 238
/** Keyboard right control. */
#define KEYBOARD_KEY_CODE_CONTROL_RIGHT 239
/** Keyboard F1 key. */
#define KEYBOARD_KEY_CODE_F1 240
/** Keyboard F2 key. */
#define KEYBOARD_KEY_CODE_F2 241
/** Keyboard F3 key. */
#define KEYBOARD_KEY_CODE_F3 242
/** Keyboard F4 key. */
#define KEYBOARD_KEY_CODE_F4 243
/** Keyboard F5 key. */
#define KEYBOARD_KEY_CODE_F5 244
/** Keyboard F6 key. */
#define KEYBOARD_KEY_CODE_F6 245
/** Keyboard F7 key. */
#define KEYBOARD_KEY_CODE_F7 246
/** Keyboard F8 key. */
#define KEYBOARD_KEY_CODE_F8 247
/** Keyboard F9 key. */
#define KEYBOARD_KEY_CODE_F9 248
/** Keyboard F10 key. */
#define KEYBOARD_KEY_CODE_F10 249
/** Keyboard F11 key. */
#define KEYBOARD_KEY_CODE_F11 250

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Tell if the user hit a key or not.
 * @return 0 (false) if no key was pressed or 1 (true) if the user pressed a key.
 * @note This function is non blocking.
 */
int KeyboardIsKeyAvailable(void);

/** Read a character from the keyboard.
 * @return The character code.
 * @note This function is blocking.
 * @note The read character is not echoed on the terminal.
 */
int KeyboardReadCharacter(void);

/** Read an unsigned 32-bit number from the keyboard.
 * @return The read number converted to binary.
 * @note Only digits are allowed, other characters are discarded.
 * @note The result will be unpredictable if the entered number is greater than 4294967295.
 */
unsigned int KeyboardReadUnsignedInteger(void);

// TODO
// KeyboardReadString
// KeyboardReadInteger

#endif