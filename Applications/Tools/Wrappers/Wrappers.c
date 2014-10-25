/** @file Wrappers.c
 * @see Wrappers.h for description.
 * @author Adrien RICCIARDI
 */
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "Screen.h"
#include "Wrappers.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** The number of columns the test program will use to display all the ASCII characters. */
#define WRAPPERS_CHARACTERS_DISPLAYING_COLUMNS_COUNT 16

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A VT100 tty color. */
typedef struct
{
	int Is_Brightness_Enabled;
	int Color_Code;
} TColor;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** ASCII to UTF-8 correspondance table. */
static wchar_t Characters_Translation_Table[256] =
{
	L'✘', // NULL character, but use another sign for debugging purpose
	L'\u263A', // White smiley
	L'\u263B', // Black smiley
	L'\u2665', // Heart
	L'\u2666', // Diamond
	L'\u2663', // Club
	L'\u2660', // Spade
	L'\u26AB', // Small black circle
	L'◘',
	L'○',
	L'◙',
	L'\u2642', // Male sign
	L'\u2640', // Female sign
	L'\u266A', // Music eighth note
	L'\u266B', // Music beamed eighth note
	L'\u2732', // Open center asterisk
	L'\u25B6', // Black right arrow
	L'\u25C0', // Black left arrow
	L'\u2195', // Up down arrow
	L'\u203C', // Double exclamation mark
	L'\u00B6', // Pilcrow sign
	L'\u00A7', // Section sign
	L'\u2014', // Em dash
	L'\u21A8', // Up down arrow with base
	L'\u2191', // Upwards arrow
	L'\u2193', // Downwards arrow
	L'\u2192', // Rightwards arrow
	L'\u2190', // Leftwards arrow
	L'\u02EA', // Sort of 'L'
	L'\u2194', // Left right arrow
	L'\u25B2', // Black up arrow
	L'\u25BC', // Black down arrow
	L' ', // Space
	L'!',
	L'\"',
	L'#',
	L'$',
	L'%',
	L'&',
	L'`',
	L'(',
	L')',
	L'*',
	L'+',
	L',',
	L'-',
	L'.',
	L'/',
	L'0',
	L'1',
	L'2',
	L'3',
	L'4',
	L'5',
	L'6',
	L'7',
	L'8',
	L'9',
	L':',
	L';',
	L'<',
	L'=',
	L'>',
	L'?',
	L'@',
	L'A',
	L'B',
	L'C',
	L'D',
	L'E',
	L'F',
	L'G',
	L'H',
	L'I',
	L'J',
	L'K',
	L'L',
	L'M',
	L'N',
	L'O',
	L'P',
	L'Q',
	L'R',
	L'S',
	L'T',
	L'U',
	L'V',
	L'W',
	L'X',
	L'Y',
	L'Z',
	L'[',
	L'\\',
	L']',
	L'^',
	L'_',
	L'\'',
	L'a',
	L'b',
	L'c',
	L'd',
	L'e',
	L'f',
	L'g',
	L'h',
	L'i',
	L'j',
	L'k',
	L'l',
	L'm',
	L'n',
	L'o',
	L'p',
	L'q',
	L'r',
	L's',
	L't',
	L'u',
	L'v',
	L'w',
	L'x',
	L'y',
	L'z',
	L'{',
	L'|',
	L'}',
	L'~',
	L'\u2302', // House
	L'Ç',
	L'ü',
	L'é',
	L'â',
	L'ä',
	L'à',
	L'å',
	L'ç',
	L'ê',
	L'ë',
	L'è',
	L'ï',
	L'î',
	L'ì',
	L'Ä',
	L'Å',
	L'É',
	L'æ',
	L'Æ',
	L'ô',
	L'ö',
	L'ò',
	L'û',
	L'ù',
	L'ÿ',
	L'Ö',
	L'Ü',
	L'\u20B5', // Cedi sign
	L'£',
	L'\u00A5', // Yen sign
	L'\u20A7', // Peseta sign
	L'\u0192', // Latin small letter 'f' with hook (not sure about this letter)
	L'á',
	L'í',
	L'ó',
	L'ú',
	L'ñ',
	L'Ñ',
	L'ª',
	L'º',
	L'¿',
	L'⌐',
	L'¬',
	L'½',
	L'¼',
	L'¡',
	L'«',
	L'»',
	L'░',
	L'▒',
	L'▓',
	L'│',
	L'┤',
	L'╡',
	L'╢',
	L'╖',
	L'╕',
	L'╣',
	L'║',
	L'╗',
	L'╝',
	L'╜',
	L'╛',
	L'┐',
	L'└',
	L'┴',
	L'┬',
	L'├',
	L'─',
	L'┼',
	L'╞',
	L'╟',
	L'╚',
	L'╔',
	L'╩',
	L'╦',
	L'╠',
	L'═',
	L'╬',
	L'╧',
	L'╨',
	L'╤',
	L'╥',
	L'╙',
	L'╘',
	L'╒',
	L'╓',
	L'╫',
	L'╪',
	L'┘',
	L'┌',
	L'█',
	L'▄',
	L'▌',
	L'▐',
	L'▀',
	L'α',
	L'β',
	L'Γ',
	L'π',
	L'Σ',
	L'σ',
	L'µ',
	L'τ',
	L'Φ',
	L'Θ',
	L'Ω',
	L'δ',
	L'∞',
	L'⌀',
	L'∊',
	L'∩',
	L'≡',
	L'±',
	L'≥',
	L'≤',
	L'⌠',
	L'⌡',
	L'÷',
	L'≈',
	L'°',
	L'∙',
	L'⋅',
	L'√',
	L'ⁿ',
	L'²',
	L'■',
	L'✖', // It should be a blank character, but use something else for debugging purpose
};

static TColor Foreground_Colors_Translation_Table[] =
{
	{0, 30}, // Black
	{0, 34}, // Blue
	{0, 32}, // Green
	{0, 36}, // Cyan
	{0, 31}, // Red
	{0, 35}, // Magenta
	{0, 33}, // Brown
	{0, 37}, // Light gray
	{1, 30}, // Dark gray
	{1, 34}, // Light blue
	{1, 32}, // Light green
	{1, 36}, // Light cyan
	{1, 31}, // Light red
	{1, 35}, // Light magenta
	{1, 33}, // Yellow
	{1, 37} // White
};

static TColor Background_Colors_Translation_Table[] =
{
	{0, 40}, // Black
	{0, 44}, // Blue
	{0, 42}, // Green
	{0, 46}, // Cyan
	{0, 41}, // Red
	{0, 45}, // Magenta
	{0, 43}, // Brown
	{0, 47}, // Light gray
	{0, 100}, // Dark gray
	{0, 104}, // Light blue
	{0, 102}, // Light green
	{0, 106}, // Light cyan
	{0, 101}, // Light red
	{0, 105}, // Light magenta
	{0, 103}, // Yellow
	{0, 107} // White
};

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Set a VT100 color specified by a VGA color code.
 * @param VGA_Color_Code The color code to define (use SCREEN_MAKE_COLOR() to create it).
 */
static void WrappersSetColor(unsigned char VGA_Color_Code)
{
	wprintf(L"\033[%d;%d;%dm",
		Foreground_Colors_Translation_Table[VGA_Color_Code & 0x0F].Is_Brightness_Enabled,
		Foreground_Colors_Translation_Table[VGA_Color_Code & 0x0F].Color_Code,
		Background_Colors_Translation_Table[VGA_Color_Code >> 4].Color_Code);
}

/** Called when exiting the program. */
static void WrappersExit(void)
{
	// Reenable the terminal cursor
	wprintf(L"\033[?25h");
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void WrappersInitialize(void)
{
	// Needed to allow wide characters usage
	setlocale(LC_ALL, "");
	
	// Disable terminal cursor displaying
	wprintf(L"\033[?25l");
	
	atexit(WrappersExit);
}

void ScreenDisplayBuffer(unsigned char *Pointer_Buffer)
{
	int i;
	
	// Set the cursor to home position
	wprintf(L"\033[H");
	
	for (i = 0; i < SCREEN_ROWS_COUNT * SCREEN_COLUMNS_COUNT * 2; i += 2)
	{
		// Set text and background colors
		WrappersSetColor(Pointer_Buffer[i + 1]);
		
		// Display the character
		fputwc(Characters_Translation_Table[(unsigned int) Pointer_Buffer[i]], stdout);
	}
}

#if WRAPPERS_ENABLE_UNIT_TESTS == 1
	int main(void)
	{
		int Row, Column;
		unsigned int i;
		char Video_Buffer[SCREEN_ROWS_COUNT * SCREEN_COLUMNS_COUNT * 2];

		WrappersInitialize();

		// Display all ASCII characters in an UTF-8 terminal
		for (Row = 0; Row < 256 / WRAPPERS_CHARACTERS_DISPLAYING_COLUMNS_COUNT; Row++)
		{
			i = Row;
			for (Column = 0; Column < WRAPPERS_CHARACTERS_DISPLAYING_COLUMNS_COUNT; Column++)
			{
				wprintf(L"\033[33m%3d:\033[0m %lc\t", i, Characters_Translation_Table[i]);
				i += 256 / WRAPPERS_CHARACTERS_DISPLAYING_COLUMNS_COUNT;
			}
			wprintf(L"\n");
		}
		
		// Display all available foreground colors
		wprintf(L"\n");
		for (i = 0; i < 16; i++)
		{
			WrappersSetColor(i);
			wprintf(L"%X", i);
		}
		wprintf(L"\033[0m");
		
		// Display all available background colors
		wprintf(L"\n");
		for (i = 0; i < 16; i++)
		{
			WrappersSetColor(i << 4);
			wprintf(L"%X", i);
		}
		wprintf(L"\033[0m\n");
		
		// Clear screen
		for (i = 0; i < sizeof(Video_Buffer); i += 2)
		{
			Video_Buffer[i] = ' ';
			Video_Buffer[i + 1] = SCREEN_MAKE_COLOR(SCREEN_COLOR_BLACK, SCREEN_COLOR_LIGHT_GREEN);
		}
		
		// Display a simple text
		Video_Buffer[0] = 'T';
		Video_Buffer[1] = SCREEN_MAKE_COLOR(SCREEN_COLOR_YELLOW, SCREEN_COLOR_MAGENTA);
		Video_Buffer[2] = 'E';
		Video_Buffer[3] = SCREEN_MAKE_COLOR(SCREEN_COLOR_YELLOW, SCREEN_COLOR_MAGENTA);
		Video_Buffer[4] = 'S';
		Video_Buffer[5] = SCREEN_MAKE_COLOR(SCREEN_COLOR_YELLOW, SCREEN_COLOR_MAGENTA);
		Video_Buffer[6] = 'T';
		Video_Buffer[7] = SCREEN_MAKE_COLOR(SCREEN_COLOR_YELLOW, SCREEN_COLOR_MAGENTA);
		ScreenDisplayBuffer(Video_Buffer);

		return 0;
	}
#endif

