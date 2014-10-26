/** @file Driver_Keyboard.c
 * @see Driver_Keyboard.h for description.
 * @author Adrien RICCIARDI
 */
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_Screen.h> // To have the ScreenClear() function and needed by KeyboardReadString() function
#include <Hardware_Functions.h> // In order to have the inb() and outb() functions
#include <Kernel.h> // In order to have the KernelStartShell() prototype

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
#define KEYBOARD_PORT_DATA 0x60
#define KEYBOARD_PORT_COMMANDS 0x64
#define KEYBOARD_LED_CAPS_LOCK 0x04
#define KEYBOARD_LED_NUM_LOCK 0x02
#define KEYBOARD_LED_SCROLL_LOCK 0x01

/** Wait until the keyboard controller becomes ready. */
#define WAIT_KEYBOARD_READY() while (inb(KEYBOARD_PORT_COMMANDS) & 2)
/** Send the End of Interrupt code to the Programmable Interrupt Controller. */
#define KEYBOARD_ACKNOWLEDGE() outb(0x20, 32)

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
static unsigned char Keyboard_Led_State = KEYBOARD_LED_NUM_LOCK, Keyboard_ASCII_Code, Keyboard_Is_Key_Available = 0;

/** Uppercase keyboard layout, used when Shift or Caps. Lock. key is active. */
static unsigned char Keyboard_Uppercase_ASCII_Table[] =
{
    0,   27,  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',
  '9',  '0', 0xF8,  '+', '\b', '\t',  'A',  'Z',  'E',  'R',
  'T',  'Y',  'U',  'I',  'O',  'P',    0, 0x9C, '\n',
  KEYBOARD_KEY_CODE_CONTROL_LEFT,
  'Q',  'S',  'D',  'F',  'G',  'H',  'J',  'K',  'L',  'M',
  '%', 0xFD,    0, 0xE6,  'W',  'X',  'C',  'V',  'B',  'N',
  '?',  '.',  '/', 0x15,    0,  '*',
  KEYBOARD_KEY_CODE_ALT,  ' ',    0,
  KEYBOARD_KEY_CODE_F1,
  KEYBOARD_KEY_CODE_F2,
  KEYBOARD_KEY_CODE_F3,
  KEYBOARD_KEY_CODE_F4,
  KEYBOARD_KEY_CODE_F5,
  KEYBOARD_KEY_CODE_F6,
  KEYBOARD_KEY_CODE_F7,
  KEYBOARD_KEY_CODE_F8,
  KEYBOARD_KEY_CODE_F9,
  KEYBOARD_KEY_CODE_F10,    0,
    0,  '7',  '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',    0,    0,  '>',
  KEYBOARD_KEY_CODE_F11,
  0
};  

/** Lowercase keyboard layout, it the default layout. */
static unsigned char Keyboard_Lowercase_ASCII_Table[] = 
{   
    0,   27,  '&', 0x82,  '"', 0x27,  '(',  '-', 0x8A,  '_',
 0x87, 0x85,  ')',  '=', '\b', '\t',  'a',  'z',  'e',  'r',
  't',  'y',  'u',  'i',  'o',  'p',    0,  '$', '\n',
 KEYBOARD_KEY_CODE_CONTROL_LEFT,
  'q',  's',  'd',  'f',  'g',  'h',  'j',  'k',  'l',  'm',
 0x97, 0xFD,    0,  '*',  'w',  'x',  'c',  'v',  'b',  'n',
  ',',  ';',  ':',  '!',    0,  '*',
  KEYBOARD_KEY_CODE_ALT,  ' ',    0,
  KEYBOARD_KEY_CODE_F1,
  KEYBOARD_KEY_CODE_F2,
  KEYBOARD_KEY_CODE_F3,
  KEYBOARD_KEY_CODE_F4,
  KEYBOARD_KEY_CODE_F5,
  KEYBOARD_KEY_CODE_F6,
  KEYBOARD_KEY_CODE_F7,
  KEYBOARD_KEY_CODE_F8,
  KEYBOARD_KEY_CODE_F9,
  KEYBOARD_KEY_CODE_F10,    0,
    0,  '7',  '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',    0,    0,  '<',
  KEYBOARD_KEY_CODE_F11,
  0
}; 

/** Alternate keyboard layout enabled when Alt Gr is held. */
static unsigned char Keyboard_Alternate_ASCII_Table[] =
{
    0,   27,    0,  '~',  '#',  '{',  '[',  '|',  '`', 0x5C,
  '^',  '@',  ']',  '}', '\b', '\t',    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, 0x0F, '\n',
    KEYBOARD_KEY_CODE_CONTROL_LEFT,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, 0x80,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
    KEYBOARD_KEY_CODE_ALT,  0,    0,
    KEYBOARD_KEY_CODE_F1,
    KEYBOARD_KEY_CODE_F2,
    KEYBOARD_KEY_CODE_F3,
    KEYBOARD_KEY_CODE_F4,
    KEYBOARD_KEY_CODE_F5,
    KEYBOARD_KEY_CODE_F6,
    KEYBOARD_KEY_CODE_F7,
    KEYBOARD_KEY_CODE_F8,
    KEYBOARD_KEY_CODE_F9,
    KEYBOARD_KEY_CODE_F10,  0,
    0,  '7',  '8',  '9',    0,  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',    0,    0,    0,
    KEYBOARD_KEY_CODE_F11,
    0
};

/** Extended keys layout reserved for 0xE0 scan codes. */
static unsigned char Keyboard_Extended_ASCII_Table[] =
{
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0,    0,    0, '\n',
	KEYBOARD_KEY_CODE_CONTROL_RIGHT,
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,    0,    0,  '/',    0,    0,    0,    0,    0,    0,
	0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	0,
	KEYBOARD_KEY_CODE_ORIGIN, // Scan code = 71
	KEYBOARD_KEY_CODE_ARROW_UP, // Scan code = 72
	KEYBOARD_KEY_CODE_PAGE_UP, // Scan code = 73
	0,
	KEYBOARD_KEY_CODE_ARROW_LEFT, // Scan code = 75
	0,
	KEYBOARD_KEY_CODE_ARROW_RIGHT, // Scan code = 77
	0,
	KEYBOARD_KEY_CODE_END, // Scan code = 79
	KEYBOARD_KEY_CODE_ARROW_DOWN, // Scan code = 80
	KEYBOARD_KEY_CODE_PAGE_DOWN, // Scan code = 81
	KEYBOARD_KEY_CODE_INSERT, // Scan code = 82
	KEYBOARD_KEY_CODE_DELETE, // Scan code = 83
	0,    0,    0,    0,    0,    0,    0,
	KEYBOARD_KEY_CODE_WINDOWS_LEFT, // Scan code = 91
	KEYBOARD_KEY_CODE_WINDOWS_RIGHT, // Scan code = 92
	KEYBOARD_KEY_CODE_RIGHT_MENU // Scan code = 93
};

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Light or turn off keyboard leds according to Keyboard_Led_State value. */
static void KeyboardActivateLeds(void)
{
	// Send "set leds" command
	WAIT_KEYBOARD_READY();
	outb(KEYBOARD_PORT_DATA, 0xED);
	// Send a byte indicating which leds to light and/or turn off
	WAIT_KEYBOARD_READY();
	outb(KEYBOARD_PORT_DATA, Keyboard_Led_State);
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void KeyboardInitialize(void)
{
	// Set the fastest repeat rate and the lowest response delay
	WAIT_KEYBOARD_READY();
	outb(KEYBOARD_PORT_DATA, 0xF3);
	WAIT_KEYBOARD_READY();
	outb(KEYBOARD_PORT_DATA, 0);
	
	// Set Num. Lock. led
	KeyboardActivateLeds();
}

unsigned char KeyboardReadChar(void)
{
	// Wait for a key to be hit
	while (!Keyboard_Is_Key_Available);
	Keyboard_Is_Key_Available = 0;
	return Keyboard_ASCII_Code;
}

void KeyboardReadString(char *String, unsigned int Maximum_Characters_Number)
{
	unsigned int i = 0;
	unsigned char Key;
	
	while (1)
	{
		Key = KeyboardReadChar();
		switch (Key)
		{
			// Backspace
			case '\b':
				// Check if the string contains at least one character
				if (i == 0) break;
				// Delete the last string character
				i--;
				String[i] = 0;
				// Clean the deleted character from the screen
				ScreenWriteChar(8);
				break;
			
			// Enter
			case '\n':
				String[i] = 0;
				ScreenWriteChar('\n'); // Go to a new line
				return;
				
			default:
				// Check if the character is a printable character
				if (Key < 32) break;
				// Check if this doesn't overflow the maximum string width
				if (i >= Maximum_Characters_Number) break;
				// Add the character to the string
				String[i] = Key;
				i++;
				// Print to screen
				ScreenWriteChar(Key);
		}
	}
}

int KeyboardIsKeyAvailable(void)
{
	return Keyboard_Is_Key_Available;
}

void KeyboardRebootSystem(void)
{
	WAIT_KEYBOARD_READY();
	outb(KEYBOARD_PORT_COMMANDS, 0xFE);
}

void KeyboardEnableA20Gate(void)
{
	WAIT_KEYBOARD_READY();
	outb(KEYBOARD_PORT_COMMANDS, 0xDF);
}

//-------------------------------------------------------------------------------------------------
// Interrupt handler
//-------------------------------------------------------------------------------------------------
void KeyboardInterruptHandler(void)
{
	static char Is_Uppercase_Mode = 0, Is_Alternate_Mode = 0, Is_Extended_Mode = 0, Is_Stress_Mode = 0;
	unsigned char Scan_Code;
	
	// Wait for the complete key transfer from the motherboard controller
	while ((inb(KEYBOARD_PORT_COMMANDS) & 0x01) == 0);
	// Get scan code
	Scan_Code = inb(KEYBOARD_PORT_DATA);
	
	// Enable the following lines to see raw data from keyboard (debugging purpose only)
	/*ConsoleWriteString(itoa(Scan_Code));
	ConsoleWriteChar('\n');
	outb(0x20, 32);
	return;*/
	
	// F12 : go back instantly to the kernel console
	#ifndef INSTALLER
		if (Scan_Code == 0x58)
		{
			// Send the End of Interrupt code to the PIC
			KEYBOARD_ACKNOWLEDGE(); // It is not sent automatically when the interrupt returns because it never returns...
			ScreenClear();
			KernelStartShell();
		}
	#endif
	
	// Check extended mode state
	if (Scan_Code == 0xE0)
	{
		Is_Extended_Mode = 1;
		return;
	}
	
	// Handle extended mode
	if (Is_Extended_Mode)
	{
		switch (Scan_Code)
		{
			// Alt Gr press
			case 0x38:
				Is_Alternate_Mode = 1;
				break;
				
			// Alt Gr release
			case 0x38 + 128:
				Is_Alternate_Mode = 0;
				break;
			
			// Get corresponding alternate key or virtual code
			default:
				// Avoid release codes
				if (Scan_Code & 0x80) break;
				
				// Get virtual or ASCII code only if it exists
				Keyboard_ASCII_Code = Keyboard_Extended_ASCII_Table[Scan_Code];
				if (Keyboard_ASCII_Code != 0) Keyboard_Is_Key_Available = 1;
				break;
		}
		Is_Extended_Mode = 0;
		return;
	}
	
	// Is this a key press or a key release ? (break code = make code + 128)
	// Check special keys first
	switch (Scan_Code)
	{
		// This section is for key pressing
 		// Left shift : turn off Caps. Lock. led
		case 0x2A:
			Is_Uppercase_Mode = 1;
			Keyboard_Led_State &= ~KEYBOARD_LED_CAPS_LOCK;
			KeyboardActivateLeds();
			return;

		// Right shift : turn off Caps. Lock. led
		case 0x36:
			Is_Uppercase_Mode = 1;
			Keyboard_Led_State &= ~KEYBOARD_LED_CAPS_LOCK;
			KeyboardActivateLeds();
			return;

		// Caps lock : light corresponding led
		case 0x3A:
			Is_Uppercase_Mode = 1;
			Keyboard_Led_State |= KEYBOARD_LED_CAPS_LOCK;
			KeyboardActivateLeds();
			return;
			
		// Stress key
		case 0x1A:
			Is_Stress_Mode = 1;
			return;
					
		// This section is for key releasing
		// Left shift
		case 0x2A + 128:
			Is_Uppercase_Mode = 0;
			return;
			
		// Right shift
		case 0x36 + 128:
			Is_Uppercase_Mode = 0;
			return;
	}
	
	// Return ASCII code only on key pressing
	if (Scan_Code & 0x80) return;

	// Get the ASCII or virtual code corresponding to the scan code
	// Handle accents
	if (Is_Stress_Mode)
	{
		switch (Scan_Code)
		{
			// Letter A
			case 16:
				if (Is_Uppercase_Mode) Keyboard_ASCII_Code = 132;
				else Keyboard_ASCII_Code = 131;
				break;
			// Letter E
			case 18:
				if (Is_Uppercase_Mode) Keyboard_ASCII_Code = 137;
				else Keyboard_ASCII_Code = 136;
				break;
			// Letter U
			case 22:
				if (Is_Uppercase_Mode) Keyboard_ASCII_Code = 129;
				else Keyboard_ASCII_Code = 150;
				break;
			// Letter I
			case 23:
				if (Is_Uppercase_Mode) Keyboard_ASCII_Code = 139;
				else Keyboard_ASCII_Code = 140;
				break;
			// Letter O
			case 24:
				if (Is_Uppercase_Mode) Keyboard_ASCII_Code = 148;
				else Keyboard_ASCII_Code = 147;
				break;
			// Not allowed characters
			default:
				break;
		}
		Is_Stress_Mode = 0;
	}
	else if (Is_Alternate_Mode) Keyboard_ASCII_Code = Keyboard_Alternate_ASCII_Table[Scan_Code];
	else if (Is_Uppercase_Mode) Keyboard_ASCII_Code = Keyboard_Uppercase_ASCII_Table[Scan_Code];
	else Keyboard_ASCII_Code = Keyboard_Lowercase_ASCII_Table[Scan_Code];
	
	// Signal the new key only if it has an appropriate code
	if (Keyboard_ASCII_Code != 0) Keyboard_Is_Key_Available = 1;
}