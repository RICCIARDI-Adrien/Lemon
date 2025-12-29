/** @file Driver_Screen.c
 * See Driver_Screen.h for description.
 * @author Adrien RICCIARDI
 */
#include <Drivers/Driver_Screen.h>
#include <Hardware_Functions.h>

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Video RAM base address. */
#define SCREEN_MEMORY_ADDRESS 0xB8000

/** Compute the character address in video memory corresponding to the current cursor location. */
#define SCREEN_COMPUTE_CURSOR_ADDRESS() (unsigned char *) ((Screen_Cursor_Row * SCREEN_COLUMNS_COUNT + Screen_Cursor_Column) * 2 + SCREEN_MEMORY_ADDRESS)

/** The CRTC registers are multiplexed. The address register needs to be set with the internal register index (address) to read or write. */
#define SCREEN_VGA_REGISTER_CRTC_ADDRESS 0x03D4
/** The CRTC registers are multiplexed. Reading from or writing to this data register accesses the internal register pointed by SCREEN_VGA_REGISTER_CRTC_ADDRESS. */
#define SCREEN_VGA_REGISTER_CRTC_DATA 0x03D5

/** The CRTC Cursor Start internal register address. */
#define SCREEN_VGA_REGISTER_CRTC_INDEX_CURSOR_START 0x0A
/** The CRTC Cursor Location High internal register address. */
#define SCREEN_VGA_REGISTER_CRTC_INDEX_CURSOR_LOCATION_HIGH 0x0E
/** The CRTC Cursor Location Low internal register address. */
#define SCREEN_VGA_REGISTER_CRTC_INDEX_CURSOR_LOCATION_LOW 0x0F

/** The CRTC Cursor Start internal register CD bit mask. */
#define SCREEN_VGA_REGISTER_CRTC_INDEX_CURSOR_START_BIT_CURSOR_DISABLE 0x20

/** The VGA Input Status 1 register containing VRetrace flag. */
#define SCREEN_VGA_REGISTER_INPUT_STATUS_1 0x03DA
/** The VGA Input Status 1 register VRetrace bit mask. */
#define SCREEN_VGA_REGISTER_INPUT_STATUS_1_BIT_VRETRACE 0x08

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The screen cursor Y position. */
static volatile unsigned int Screen_Cursor_Row;
/** The screen cursor X position. */
static volatile unsigned int Screen_Cursor_Column;
/** Cache the cursor current visibility state. */
static int Screen_Is_Cursor_Visible = 0;

/** The screen color attributes. */
static unsigned char Screen_Color;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Scroll the screen vertically from bottom to top by a line wide and update cursor position. */
static inline __attribute__((always_inline)) void ScreenScrollVertically(void)
{
	unsigned int *Pointer_Source, *Pointer_Destination, i; // We use an "int" in order to be 4 times faster than using a "char"
	unsigned char *Pointer_Last_Row;
	
	Pointer_Source = (unsigned int *) SCREEN_MEMORY_ADDRESS + ((SCREEN_COLUMNS_COUNT * 2) / sizeof(int));
	Pointer_Destination = (unsigned int *) SCREEN_MEMORY_ADDRESS;
	Pointer_Last_Row = (unsigned char *) SCREEN_MEMORY_ADDRESS + (SCREEN_COLUMNS_COUNT * (SCREEN_ROWS_COUNT - 1) * 2);
	
	for (i = 0; i < (SCREEN_COLUMNS_COUNT * (SCREEN_ROWS_COUNT - 1) * 2) / sizeof(int); i++)
	{
		*Pointer_Destination = *Pointer_Source;
		Pointer_Source++;
		Pointer_Destination++;
	}
	
	// Erase last row
	for (i = 0; i < SCREEN_COLUMNS_COUNT * 2; i++)
	{
		*Pointer_Last_Row = ' ';
		*(Pointer_Last_Row + 1) = Screen_Color;
		Pointer_Last_Row += 2;
	}
}

/** Set the location of the VGA hardware cursor. */
static void ScreenUpdateHardwareCursorPosition(void)
{
	unsigned int Cursor_Position = Screen_Cursor_Row * SCREEN_COLUMNS_COUNT + Screen_Cursor_Column;
	unsigned char *Pointer_Video_Memory = (unsigned char *) (Cursor_Position * 2 + SCREEN_MEMORY_ADDRESS + 1); // Add 1 to address the attributes byte of the character

	// Set the low register
	outb(SCREEN_VGA_REGISTER_CRTC_ADDRESS, SCREEN_VGA_REGISTER_CRTC_INDEX_CURSOR_LOCATION_LOW);
	outb(SCREEN_VGA_REGISTER_CRTC_DATA, (unsigned char) Cursor_Position);

	// Set the high register
	outb(SCREEN_VGA_REGISTER_CRTC_ADDRESS, SCREEN_VGA_REGISTER_CRTC_INDEX_CURSOR_LOCATION_HIGH);
	outb(SCREEN_VGA_REGISTER_CRTC_DATA, (unsigned char) (Cursor_Position >> 8));

	// Update the character color under the cursor with the currently defined color, otherwise the cursor may be of a color different from the chosen one (due to screen scrolling for example)
	*Pointer_Video_Memory = Screen_Color;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ScreenClear(void)
{
	char *Pointer_Video_Memory;
	int i;
	
	Screen_Cursor_Column = 0;
	Screen_Cursor_Row = 0;
		
	Pointer_Video_Memory = (char *) SCREEN_MEMORY_ADDRESS;
	
	for (i = 0; i < SCREEN_COLUMNS_COUNT * SCREEN_ROWS_COUNT * 2; i++)
	{
		*Pointer_Video_Memory = ' '; // Character
		*(Pointer_Video_Memory + 1) = Screen_Color; // Color
		Pointer_Video_Memory += 2;
	}
}

void ScreenWriteCharacter(char Character)
{
	unsigned char *Pointer_Video_Memory;
	
	switch (Character)
	{
		// Set cursor location to beginning of console line
		case '\r':
			Screen_Cursor_Column = 0;
			break;
		
		// Go to the beginning of the next line
		case '\n':
			// Go to next line
			Screen_Cursor_Column = 0;
			Screen_Cursor_Row++;
			
			// Is the end of the screen reached ?
			if (Screen_Cursor_Row >= SCREEN_ROWS_COUNT)
			{
				ScreenScrollVertically();
				Screen_Cursor_Row = SCREEN_ROWS_COUNT - 1;
			}
			break;
			
		// Erase last entered character
		case '\b':
			// Can't erase further than the beginning of the screen
			if ((Screen_Cursor_Column == 0) && (Screen_Cursor_Row == 0)) break;
			
			// Go to character to delete location
			if (Screen_Cursor_Column > 0) Screen_Cursor_Column--;
			else
			{
				Screen_Cursor_Column = SCREEN_COLUMNS_COUNT - 1;
				Screen_Cursor_Row--;
			}
			
			// Overwrite character with space
			Pointer_Video_Memory = SCREEN_COMPUTE_CURSOR_ADDRESS();
			*Pointer_Video_Memory = ' ';
			*(Pointer_Video_Memory + 1) = Screen_Color;
			break;
		
		// Display other characters
		default:
			// Display character
			Pointer_Video_Memory = SCREEN_COMPUTE_CURSOR_ADDRESS();
			*Pointer_Video_Memory = Character;
			*(Pointer_Video_Memory + 1) = Screen_Color;
			
			// Update cursor location
			Screen_Cursor_Column++;
			if (Screen_Cursor_Column >= SCREEN_COLUMNS_COUNT) // Go to next line
			{
				Screen_Cursor_Column = 0;
				Screen_Cursor_Row++;
				
				// Scroll screen if needed
				if (Screen_Cursor_Row >= SCREEN_ROWS_COUNT)
				{
					ScreenScrollVertically();
					Screen_Cursor_Row = SCREEN_ROWS_COUNT - 1;
				}
			}
			break;
	}

	// Do not put the "if (Screen_Is_Cursor_Visible)" condition into the ScreenUpdateHardwareCursorPosition() function to save some cycles by not calling the function at all
	if (Screen_Is_Cursor_Visible) ScreenUpdateHardwareCursorPosition();
}

void ScreenWriteString(char *String)
{
	while (*String != 0)
	{
		ScreenWriteCharacter(*String);
		String++;
	}
}

void ScreenGetCursorPosition(unsigned int *Pointer_Row, unsigned int *Pointer_Column)
{
	*Pointer_Row = Screen_Cursor_Row;
	*Pointer_Column = Screen_Cursor_Column;
}

void ScreenSetCursorPosition(unsigned int Row, unsigned int Column)
{
	// Update cursor location only if it is in screen bounds
	if ((Row < SCREEN_ROWS_COUNT) && (Column < SCREEN_COLUMNS_COUNT))
	{
		Screen_Cursor_Row = Row;
		Screen_Cursor_Column = Column;

		// Do not put the "if (Screen_Is_Cursor_Visible)" condition into the ScreenUpdateHardwareCursorPosition() function to save some cycles by not calling the function at all
		if (Screen_Is_Cursor_Visible) ScreenUpdateHardwareCursorPosition();
	}
}

void ScreenSetCursorVisible(int Is_Visible)
{
	unsigned char Value;

	// Get the current register value
	outb(SCREEN_VGA_REGISTER_CRTC_ADDRESS, SCREEN_VGA_REGISTER_CRTC_INDEX_CURSOR_START);
	Value = inb(SCREEN_VGA_REGISTER_CRTC_DATA);

	// Update the register value accordingly
	if (Is_Visible) Value &= ~SCREEN_VGA_REGISTER_CRTC_INDEX_CURSOR_START_BIT_CURSOR_DISABLE;
	else Value |= SCREEN_VGA_REGISTER_CRTC_INDEX_CURSOR_START_BIT_CURSOR_DISABLE;

	// Write back the new register value
	outb(SCREEN_VGA_REGISTER_CRTC_ADDRESS, SCREEN_VGA_REGISTER_CRTC_INDEX_CURSOR_START); // Just in case the address has been altered in the meanwhile
	outb(SCREEN_VGA_REGISTER_CRTC_DATA, Value);

	Screen_Is_Cursor_Visible = Is_Visible;
}

unsigned char ScreenGetColor(void)
{
	return Screen_Color;
}

void ScreenSetColor(unsigned char Color_Code)
{
	unsigned char *Pointer_Video_Memory = (unsigned char *) (SCREEN_COMPUTE_CURSOR_ADDRESS() + 1); // Add 1 to address the attributes byte of the character

	Screen_Color = Color_Code;

	// Update the character color under the cursor with the currently defined color, otherwise the cursor may be of a color different from the chosen one (due to screen scrolling for example)
	*Pointer_Video_Memory = Screen_Color;
}

void ScreenDisplayBuffer(unsigned char *Pointer_Buffer)
{
	unsigned int *Pointer_Source, *Pointer_Destination, i;
	
	// Access to source buffer 4-byte at a time
	Pointer_Source = (unsigned int *) Pointer_Buffer;
	// Copy to video memory
	Pointer_Destination = (unsigned int *) SCREEN_MEMORY_ADDRESS;
	
	// Wait for the current frame to be entirely displayed
	while (inb(SCREEN_VGA_REGISTER_INPUT_STATUS_1 & SCREEN_VGA_REGISTER_INPUT_STATUS_1_BIT_VRETRACE));
	
	// Wait for the VSync signal to be triggered
	while (!(inb(SCREEN_VGA_REGISTER_INPUT_STATUS_1) & SCREEN_VGA_REGISTER_INPUT_STATUS_1_BIT_VRETRACE));
	
	// Copy buffer into video memory 4 bytes at a time
	for (i = 0; i < SCREEN_ROWS_COUNT * SCREEN_COLUMNS_COUNT * 2 / sizeof(unsigned int); i++)
	{
		*Pointer_Destination = *Pointer_Source;
		Pointer_Destination++;
		Pointer_Source++;
	}
}
