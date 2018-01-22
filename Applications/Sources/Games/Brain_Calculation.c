/** @file Brain_Calculation.c
 * @see Games.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Games.h"
#include "Strings.h"

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** This value can't be issued by the user input function. */
#define BRAIN_CALCULATION_EXIT_CODE -1

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** All the available game difficulties. */
typedef enum
{
	BRAIN_CALCULATION_DIFFICULTY_LEVEL_EASY, //! Calculations are made with only one digit.
	BRAIN_CALCULATION_DIFFICULTY_LEVEL_MEDIUM, //! Calculations are made with 2 digits.
	BRAIN_CALCULATION_DIFFICULTY_LEVEL_HARD //! Calculations are made with 3 digits.
} TBrainCalculationDifficultyLevel;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** Difficulty-level selection menu. */
static TSystemMenu Menu_Difficulty_Level =
{
	STRING_BRAIN_CALCULATION_MENU_TITLE,
	STRING_BRAIN_CALCULATION_MENU_PROMPT,
	{
		STRING_BRAIN_CALCULATION_MENU_EASY_DIFFICULTY,
		STRING_BRAIN_CALCULATION_MENU_MEDIUM_DIFFICULTY,
		STRING_BRAIN_CALCULATION_MENU_HARD_DIFFICULTY,
		STRING_BRAIN_CALCULATION_MENU_EXIT,
		NULL
	}
};

/** The chosen difficulty level. */
static TBrainCalculationDifficultyLevel Difficulty_Level;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Return a random number in the range [Minimum ; Maximum[.
 * @param Minimum The minimum value the number can take.
 * @param Maximum The maximum value the number can take.
 * @return The random number.
 */
static int RandomNumberRange(int Minimum, int Maximum)
{
	int Number;
	
	Number = SystemRandomGenerateNumber() % (Maximum - Minimum);
	Number += Minimum;
	
	return Number;
}

/** Choose a calculus according to current difficulty level. The result is always positive.
 * @param Pointer_First_Number The first number of the calculus.
 * @param Pointer_Operator The operator (+, -, *).
 * @param Pointer_Second_Number The second number.
 * @param Pointer_Result The result of the choosen calculus.
 */
static void ChooseCalculus(int *Pointer_First_Number, char *Pointer_Operator, int *Pointer_Second_Number, int *Pointer_Result)
{
	int Minimum_Value, Maximum_Value;
	
	do
	{
		// Choose numbers according to difficulty level
		switch (Difficulty_Level)
		{
			case BRAIN_CALCULATION_DIFFICULTY_LEVEL_EASY:
				Minimum_Value = 0;
				Maximum_Value = 10;
				break;
			case BRAIN_CALCULATION_DIFFICULTY_LEVEL_MEDIUM:
				Minimum_Value = 10;
				Maximum_Value = 100;
				break;
			default:
				Minimum_Value = 100;
				Maximum_Value = 1000;
				break;
		}
		*Pointer_First_Number = RandomNumberRange(Minimum_Value, Maximum_Value);
		*Pointer_Second_Number = RandomNumberRange(Minimum_Value, Maximum_Value);
		
		// Choose operator and compute calculus
		switch (SystemRandomGenerateNumber() % 3)
		{
			case 0:
				*Pointer_Operator = '+';
				*Pointer_Result = *Pointer_First_Number + *Pointer_Second_Number;
				break;
			case 1:
				*Pointer_Operator = '-';
				*Pointer_Result = *Pointer_First_Number - *Pointer_Second_Number;
				break;
			default:
				*Pointer_Operator = '*';
				*Pointer_Result = *Pointer_First_Number * *Pointer_Second_Number;
				break;
		}
	} while (*Pointer_Result < 0);
}

/** Read user number. Allow only numbers to be typed.
 * @return The number or BRAIN_CALCULATION_EXIT_CODE if the user hit escape key.
 */
static int ReadUserNumber(void)
{
	char String[11], Character;
	unsigned int Digits_Count = 0; // Must be an unsigned value to avoid triggering a warning when compared to sizeof()
	
	while (1)
	{
		Character = SystemKeyboardReadCharacter();
		
		// Backspace, delete last digit if possible
		if (Character == '\b')
		{
			if (Digits_Count <= 0) continue;
			Digits_Count--;
			String[Digits_Count] = 0;
			SystemScreenWriteCharacter('\b');
		}
		
		// Enter, convert and return number only if user entered almost one digit
		else if (Character == '\n')
		{
			// No empty number allowed
			if (Digits_Count == 0) continue;
			
			String[Digits_Count] = 0;
			SystemScreenWriteCharacter('\n');
			return (int) SystemStringConvertStringToUnsignedInteger(String);
		}
		
		// Escape key, return exit code
		else if (Character == SYSTEM_KEYBOARD_KEY_CODE_ESCAPE) return BRAIN_CALCULATION_EXIT_CODE;
		
		// Other characters, accept only digits if there is enough space in string
		else if ((Character >= '0') && (Character <= '9') && (Digits_Count < sizeof(String) - 1))
		{
			String[Digits_Count] = Character;
			Digits_Count++;
			SystemScreenWriteCharacter(Character);
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void BrainCalculation(void)
{
	int First_Number, Second_Number, Result, Correct_Results_Count = 0, Number;
	char Operator;
	
	while (1)
	{
		// Let the player select the game difficulty
		switch (MenuDisplay(&Menu_Difficulty_Level))
		{
			case 1:
				Difficulty_Level = BRAIN_CALCULATION_DIFFICULTY_LEVEL_EASY;
				break;
			case 2:
				Difficulty_Level = BRAIN_CALCULATION_DIFFICULTY_LEVEL_MEDIUM;
				break;
			case 3:
				Difficulty_Level = BRAIN_CALCULATION_DIFFICULTY_LEVEL_HARD;
				break;
			default:
				return;
		}
		
		SystemRandomInitialize();
		
		// Show instructions
		SystemScreenWriteString(STRING_BRAIN_CALCULATION_INSTRUCTIONS);
		
		while (1)
		{
			// Show calculus
			ChooseCalculus(&First_Number, &Operator, &Second_Number, &Result);
			SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_LIGHT_BLUE);
			SystemScreenWriteString("   ");
			SystemScreenWriteInteger(First_Number);
			SystemScreenWriteCharacter(' ');
			SystemScreenWriteCharacter(Operator);
			SystemScreenWriteCharacter(' ');
			SystemScreenWriteInteger(Second_Number);
			SystemScreenWriteString("  =  ");
			SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_BLUE);
			
			// Get user's number
			Number = ReadUserNumber();
			
			// Does the user want to exit program ?
			if (Number == BRAIN_CALCULATION_EXIT_CODE)
			{
				SystemScreenWriteCharacter('\n');
				SystemScreenWriteString(STRING_BRAIN_CALCULATION_CORRECT_ANSWERS_COUNT_1);
				SystemScreenWriteInteger(Correct_Results_Count);
				SystemScreenWriteString(STRING_BRAIN_CALCULATION_CORRECT_ANSWERS_COUNT_2);
				return;
			}
			
			// Check if user's result is correct
			if (Number == Result)
			{
				SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_GREEN);
				SystemScreenWriteString(STRING_BRAIN_CALCULATION_GOOD_RESULT);
				SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_BLUE);
				
				Correct_Results_Count++;
			}
			else
			{
				SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_RED);
				SystemScreenWriteString(STRING_BRAIN_CALCULATION_BAD_RESULT_1);
				SystemScreenWriteInteger(Result);
				SystemScreenWriteString(STRING_BRAIN_CALCULATION_BAD_RESULT_2);
				
				SystemScreenSetFontColor(SYSTEM_SCREEN_COLOR_BLUE);
				SystemScreenWriteString(STRING_BRAIN_CALCULATION_CORRECT_ANSWERS_COUNT_1);
				SystemScreenWriteInteger(Correct_Results_Count);
				SystemScreenWriteString(STRING_BRAIN_CALCULATION_CORRECT_ANSWERS_COUNT_2);
				
				SystemKeyboardReadCharacter();
				break;
			}
		}
	}
}