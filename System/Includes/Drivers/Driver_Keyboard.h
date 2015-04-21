/** @file Driver_Keyboard.h
 * Driver for the 101/102 keys AZERTY keyboard.
 * @author Adrien RICCIARDI
 * @version 1.0 : 16/06/2011
 * @version 1.1 : 16/11/2011
 * @version 1.2 : 13/09/2012
 * @version 1.3 : 21/10/2012, added E0 state machine.
 * @version 1.4 : 18/07/2013, improved driver to support all AZERTY capabilities.
 * @version 1.5 : 12/12/2013, moved KeyboardReadString() from Console module.
 * @note Numpad is always activated and the "Num Lock" key is ignored.
 */
#ifndef H_DRIVER_KEYBOARD_H
#define H_DRIVER_KEYBOARD_H

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
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
/** Set the keyboard at the maximum key typing speed. */
void KeyboardInitialize(void);

/** Return the last hit key.
 * @return ASCII or virtual code of the last key hit by the user.
 * @note This is a blocking function.
 */
unsigned char KeyboardReadCharacter(void);

/** Read a string from the keyboard.
 * @param String Pointer on the destination string.
 * @param Maximum_Characters_Number Maximum allowed number of characters to be written on the screen and into the string.
 */
void KeyboardReadString(char *String, unsigned int Maximum_Characters_Number);

/** Tell if a key was pressed or not.
 * @return 1 if a key was pressed or 0 if no key was pressed.
 */
int KeyboardIsKeyAvailable(void);

/** Reboot the system by setting low the processor reset pin. */
void KeyboardRebootSystem(void);

/** Enable the A20 RAM gate, allowing more than 1MB of RAM. */
void KeyboardEnableA20Gate(void);

/** Keyboard interrupt handler. */
void KeyboardInterruptHandler(void);

#endif
