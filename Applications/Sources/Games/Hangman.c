/** @file Hangman.c
 * @see Games.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Games.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Compute automatically the total number of words. */
#define WORDS_TOTAL_COUNT SYSTEM_GET_ARRAY_ELEMENTS_COUNT(String_Words)

/** The word to find row position. */
#define ROW_FOUND_LETTERS 2
/** The invalid letters row position. */
#define ROW_INVALID_LETTERS 5
/** The remaining attempts row position. */
#define ROW_REMAINING_ATTEMPTS 8

/** This symbol is displayed at the location of the not yet found letters. */
#define NOT_YET_FOUND_LETTER_SYMBOL '-'

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Hold all available words. */
static char *String_Words[] = { STRING_HANGMAN_WORDS_LIST };

/** Tell if a word has solved yet. */
static int Is_Word_Solved[WORDS_TOTAL_COUNT];

/** The invalid letters displaying column. */
static int Invalid_Letters_Column;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Randomly choose a word, bypassing the one which were chosen yet.
 * @return a pointer on the chosen word string on success,
 * @return NULL if no more word is available.
 */
static char *ChooseNextWord(void)
{
	struct TUnsolvedWord
	{
		char *String_Word;
		unsigned int Word_Index;
	} Unsolved_Words_List[WORDS_TOTAL_COUNT];
	unsigned int i;
	int Unsolved_Words_Count = 0;
	
	// List all unsolved words
	for (i = 0; i < WORDS_TOTAL_COUNT; i++)
	{
		if (!Is_Word_Solved[i])
		{
			Unsolved_Words_List[Unsolved_Words_Count].String_Word = String_Words[i];
			Unsolved_Words_List[Unsolved_Words_Count].Word_Index = i;
			Unsolved_Words_Count++;
		}
	}
	
	// No more word remain
	if (Unsolved_Words_Count == 0) return NULL;
	
	// Select an unsolved word
	i = RandomGenerateNumber() % Unsolved_Words_Count;
	Is_Word_Solved[Unsolved_Words_List[i].Word_Index] = 1;
	return Unsolved_Words_List[i].String_Word;
}

/** Wait for the user to type a letter or the Escape key.
 * @return the read letter code.
 */
static char ReadLetter(void)
{
	char Letter;
	
	while (1)
	{
		Letter = SystemKeyboardReadCharacter();
		
		// Cast the letter to uppercase now to ease the following test
		if ((Letter >= 'a') && (Letter <= 'z')) Letter -= 32;
		
		// Is the character really a letter (or Escape) ?
		if (((Letter >= 'A') && (Letter <= 'Z')) || (Letter == SYSTEM_KEYBOARD_KEY_CODE_ESCAPE)) return Letter;
	}
}

/** Tell if the specified letter is present in the word.
 * @param Letter The letter to search for. It must be in uppercase.
 * @param String_Word The word to search into. It must be in uppercase too.
 * @return 1 if the letter is present,
 * @return 0 if the letter is absent.
 */
static int IsLetterPresentInWord(char Letter, char *String_Word)
{
	// Is the letter present in the word ?
	while (*String_Word != 0)
	{
		if (*String_Word == Letter) return 1;
		String_Word++;
	}
	
	return 0;
}

/** Display the remaining attempts count.
 * @param Remaining_Attempts The number to display.
 */
static void DisplayRemainingAttempts(int Remaining_Attempts)
{
	// Display the remaining attempts
	ScreenSetCursorPosition(ROW_REMAINING_ATTEMPTS, 0);
	ScreenSetFontColor(SCREEN_COLOR_LIGHT_BLUE);
	ScreenWriteInteger(Remaining_Attempts);
	ScreenWriteString("     "); // Clear the eventually remaining characters (as the number is decrementing)
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
}

/** Display an invalid letter.
 * @param Letter The letter to append to the displayed invalid letters list.
 */
static void DisplayInvalidLetter(char Letter)
{
	// Display the letter
	ScreenSetCursorPosition(ROW_INVALID_LETTERS, Invalid_Letters_Column);
	ScreenSetFontColor(SCREEN_COLOR_LIGHT_BLUE);
	ScreenWriteCharacter(Letter);
	ScreenWriteCharacter(' ');
	ScreenSetFontColor(SCREEN_COLOR_BLUE);
	Invalid_Letters_Column += 2;
}

/** Let the player find the word.
 * @param String_Word_To_Find The word to find.
 * @param Attempts_Count How many attempts the player has to find the word.
 * @return 0 if the player found the word,
 * @return 1 if the player lost,
 * @return 2 if the player hit the Escape key.
 */
static int Play(char *String_Word_To_Find, int Attempts_Count)
{
	char String_Found_Letters[64], Letter;
	int i, Is_Word_Found, Is_Invalid_Letter_Attempted[26] = {0};
	
	// Initialize the found letters string with '-'
	for (i = 0; String_Word_To_Find[i] != 0; i++) String_Found_Letters[i] = NOT_YET_FOUND_LETTER_SYMBOL;
	String_Found_Letters[i] = 0; // Add terminating zero
	
	// Display the updated interface
	ScreenSetCursorPosition(ROW_FOUND_LETTERS, 0);
	ScreenWriteString(String_Found_Letters);
	Invalid_Letters_Column = 0; // Reset the invalid letters column
	DisplayRemainingAttempts(Attempts_Count);
	
	while (1)
	{
		// Get the next letter
		Letter = ReadLetter();
		if (Letter == SYSTEM_KEYBOARD_KEY_CODE_ESCAPE) return 2;
		
		// Is this letter present in the word ?
		if (IsLetterPresentInWord(Letter, String_Word_To_Find))
		{
			// Add the letter to the "found letters" string
			for (i = 0; String_Word_To_Find[i] != 0; i++)
			{
				if (String_Word_To_Find[i] == Letter) String_Found_Letters[i] = Letter;
			}
			
			// Display the currently found letters
			ScreenSetCursorPosition(ROW_FOUND_LETTERS, 0);
			ScreenWriteString(String_Found_Letters);
			
			// Stop if all letters were found
			Is_Word_Found = 1;
			for (i = 0; String_Found_Letters[i] != 0; i++)
			{
				if (String_Found_Letters[i] == NOT_YET_FOUND_LETTER_SYMBOL)
				{
					Is_Word_Found = 0;
					break;
				}
			}
			if (Is_Word_Found) return 0;
		}
		else
		{
			i = Letter - 'A'; // Address the letters from 0 to 25
			if (!Is_Invalid_Letter_Attempted[i])
			{
				Attempts_Count--;
				DisplayRemainingAttempts(Attempts_Count);
				
				// Add the letter to the bad letters list if it was not in the list yet
				DisplayInvalidLetter(Letter);
				Is_Invalid_Letter_Attempted[i] = 1;
				
				// Has the player squandered all his attempts ?
				if (Attempts_Count == 0) return 1;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
void Hangman(void)
{	
	char *String_Current_Word;
	
	SystemRandomInitialize();
	
	// Reset the array
	SystemMemorySetAreaValue(Is_Word_Solved, sizeof(Is_Word_Solved), 0);
	
	while (1)
	{
		// Display the title
		ScreenClear();
		ScreenSetFontColor(SCREEN_COLOR_GREEN);
		ScreenWriteCenteredString(STRING_HANGMAN_TITLE);
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		ScreenWriteCharacter('\n');
			
		// Display the interface parts that will never change
		ScreenSetCursorPosition(ROW_INVALID_LETTERS - 1, 0);
		ScreenWriteString(STRING_HANGMAN_BAD_LETTERS_LIST);
		ScreenSetCursorPosition(ROW_REMAINING_ATTEMPTS - 1, 0);
		ScreenWriteString(STRING_HANGMAN_REMAINING_ATTEMPTS);
		
		String_Current_Word = ChooseNextWord();
		if (String_Current_Word == NULL)
		{
			// The player found all words
			ScreenClear();
			ScreenSetFontColor(SCREEN_COLOR_GREEN);
			ScreenWriteString(STRING_HANGMAN_GAME_WON_1);
			ScreenSetFontColor(SCREEN_COLOR_BLUE);
			ScreenWriteString(STRING_HANGMAN_GAME_WON_2);
			
			while (SystemKeyboardReadCharacter() != SYSTEM_KEYBOARD_KEY_CODE_ENTER);
			ScreenClear();
			return;
		}
		
		// Let the player find the word
		switch (Play(String_Current_Word, 16))
		{
			case 0:
				// The player successfully found the word
				ScreenClear();
				ScreenSetFontColor(SCREEN_COLOR_GREEN);
				ScreenWriteString(STRING_HANGMAN_WORD_FOUND_1);
				ScreenSetFontColor(SCREEN_COLOR_BLUE);
				ScreenWriteString(STRING_HANGMAN_WORD_FOUND_2);
				while (SystemKeyboardReadCharacter() != SYSTEM_KEYBOARD_KEY_CODE_ENTER);
				break;
			
			case 1:
				// The player did not find the word
				ScreenClear();
				ScreenSetFontColor(SCREEN_COLOR_RED);
				ScreenWriteString(STRING_HANGMAN_WORD_NOT_FOUND_1);
				ScreenSetFontColor(SCREEN_COLOR_BLUE);
				ScreenWriteString(STRING_HANGMAN_WORD_NOT_FOUND_2);
				ScreenWriteString(String_Current_Word);
				ScreenWriteString(STRING_HANGMAN_WORD_NOT_FOUND_3);
				while (SystemKeyboardReadCharacter() != SYSTEM_KEYBOARD_KEY_CODE_ENTER);
				return;
			
			default:
				return;
		}
	}
}