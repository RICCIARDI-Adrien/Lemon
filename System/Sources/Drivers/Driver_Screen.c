/** @file Driver_Screen.c
 * @see Driver_Screen.h for description.
 * @author Adrien RICCIARDI
 */
#include <Drivers/Driver_Screen.h>
#include <Hardware_Functions.h> // Needed by inb() function

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Compute the character address in video memory corresponding to the current cursor location. */
#define COMPUTE_CURSOR_ADDRESS() (unsigned char *) ((Screen_Cursor_Row * SCREEN_COLUMNS_COUNT + Screen_Cursor_Column) * 2 + SCREEN_MEMORY_ADDRESS)

/** VGA Input Status 1 register containing VRetrace flag. */
#define SCREEN_VGA_REGISTER_INPUT_STATUS_1 0x03DA
/** VGA Input Status 1 register VRetrace bit index. */
#define SCREEN_VGA_REGISTER_INPUT_STATUS_1_BIT_VRETRACE 0x08

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
static unsigned int Screen_Cursor_Row, Screen_Cursor_Column;
unsigned char Screen_Color;

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

void ScreenWriteChar(char Character)
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
			Pointer_Video_Memory = COMPUTE_CURSOR_ADDRESS();
			*Pointer_Video_Memory = ' ';
			*(Pointer_Video_Memory + 1) = Screen_Color;
			break;
		
		// Display other characters
		default:
			// Display character
			Pointer_Video_Memory = COMPUTE_CURSOR_ADDRESS();
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
	}
}

void ScreenWriteString(char *String)
{
	while (*String != 0)
	{
		ScreenWriteChar(*String);
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
	}
}

void ScreenDisplayBuffer(unsigned char *Pointer_Buffer)
{
	unsigned int *Pointer_Source, *Pointer_Destination, i;
	
	// Access to source buffer 4-byte by time
	Pointer_Source = (unsigned int *) Pointer_Buffer;
	// Copy to video memory
	Pointer_Destination = (unsigned int *) SCREEN_MEMORY_ADDRESS;
	
	// Wait for the current frame to be entirely displayed
	while(inb(SCREEN_VGA_REGISTER_INPUT_STATUS_1 & SCREEN_VGA_REGISTER_INPUT_STATUS_1_BIT_VRETRACE));
	
	// Wait for the VSync signal to trigger
	while(!(inb(SCREEN_VGA_REGISTER_INPUT_STATUS_1) & SCREEN_VGA_REGISTER_INPUT_STATUS_1_BIT_VRETRACE));
	
	// Copy buffer into video memory 4 bytes by time
	for (i = 0; i < SCREEN_ROWS_COUNT * SCREEN_COLUMNS_COUNT * 2 / sizeof(unsigned int); i++)
	{
		*Pointer_Destination = *Pointer_Source;
		Pointer_Destination++;
		Pointer_Source++;
	}
}