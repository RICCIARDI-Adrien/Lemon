/** @file Brain_Calculation.c
 * @see Brain_Calculation.h for description.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Brain_Calculation.h"

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
	Easy, Medium, Hard
} TDifficultyLevel;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The chosen difficulty level. */
static TDifficultyLevel Difficulty_Level;

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
	
	do
	{
		Number = RandomGenerateNumber() % Maximum;
	} while (Number < Minimum);
	
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
			case Easy:
				Minimum_Value = 0;
				Maximum_Value = 10;
				break;
			case Medium:
				Minimum_Value = 10;
				Maximum_Value = 100;
				break;
			case Hard:
				Minimum_Value = 100;
				Maximum_Value = 1000;
				break;
		}
		*Pointer_First_Number = RandomNumberRange(Minimum_Value, Maximum_Value);
		*Pointer_Second_Number = RandomNumberRange(Minimum_Value, Maximum_Value);
		
		// Choose operator and compute calculus
		switch (RandomGenerateNumber() % 3)
		{
			case 0:
				*Pointer_Operator = '+';
				*Pointer_Result = *Pointer_First_Number + *Pointer_Second_Number;
				break;
			case 1:
				*Pointer_Operator = '-';
				*Pointer_Result = *Pointer_First_Number - *Pointer_Second_Number;
				break;
			case 2:
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
		Character = KeyboardReadCharacter();
		
		// Backspace, delete last digit if possible
		if (Character == '\b')
		{
			if (Digits_Count <= 0) continue;
			Digits_Count--;
			String[Digits_Count] = 0;
			ScreenWriteCharacter('\b');
		}
		
		// Enter, convert and return number only if user entered almost one digit
		else if (Character == '\n')
		{
			// No empty number allowed
			if (Digits_Count == 0) continue;
			
			String[Digits_Count] = 0;
			ScreenWriteCharacter('\n');
			return (int) StringConvertStringToUnsignedInteger(String);
		}
		
		// Escape key, return exit code
		else if (Character == KEYBOARD_KEY_CODE_ESCAPE) return BRAIN_CALCULATION_EXIT_CODE;
		
		// Other characters, accept only digits if there is enough space in string
		else if ((Character >= '0') && (Character <= '9') && (Digits_Count < sizeof(String) - 1))
		{
			String[Digits_Count] = Character;
			Digits_Count++;
			ScreenWriteCharacter(Character);
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int First_Number, Second_Number, Result, Correct_Results_Count = 0, Number;
	char Operator;
	
	// Use selected difficulty level or select medium difficulty if user did not specify a difficulty
	if (argc == 2)
	{
		if (StringCompare(argv[1], "-e")) Difficulty_Level = Easy;
		else if (StringCompare(argv[1], "-h")) Difficulty_Level = Hard;
		else Difficulty_Level = Medium;
	}
	else Difficulty_Level = Medium;
	
	RandomInitialize();
	
	// Show instructions
	ScreenWriteString(STRING_INSTRUCTIONS);
	
	while (1)
	{
		// Show calculus
		ChooseCalculus(&First_Number, &Operator, &Second_Number, &Result);
		ScreenSetFontColor(SCREEN_COLOR_LIGHT_BLUE);
		ScreenWriteString("   ");
		ScreenWriteInteger(First_Number);
		ScreenWriteCharacter(' ');
		ScreenWriteCharacter(Operator);
		ScreenWriteCharacter(' ');
		ScreenWriteInteger(Second_Number);
		ScreenWriteString("  =  ");
		ScreenSetFontColor(SCREEN_COLOR_BLUE);
		
		// Get user's number
		Number = ReadUserNumber();
		
		// Does the user want to exit program ?
		if (Number == BRAIN_CALCULATION_EXIT_CODE)
		{
			ScreenWriteCharacter('\n');
			ScreenWriteString(STRING_CORRECT_ANSWERS_COUNT_1);
			ScreenWriteInteger(Correct_Results_Count);
			ScreenWriteString(STRING_CORRECT_ANSWERS_COUNT_2);
			return 0;
		}
		
		// Check if user's result is correct
		if (Number == Result)
		{
			ScreenSetFontColor(SCREEN_COLOR_GREEN);
			ScreenWriteString(STRING_GOOD_RESULT);
			ScreenSetFontColor(SCREEN_COLOR_BLUE);
			
			Correct_Results_Count++;
		}
		else
		{
			ScreenSetFontColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_BAD_RESULT_1);
			ScreenWriteInteger(Result);
			ScreenWriteString(STRING_BAD_RESULT_2);
			
			ScreenSetFontColor(SCREEN_COLOR_BLUE);
			ScreenWriteString(STRING_CORRECT_ANSWERS_COUNT_1);
			ScreenWriteInteger(Correct_Results_Count);
			ScreenWriteString(STRING_CORRECT_ANSWERS_COUNT_2);
			return 0;
		}
	}
	
	return 0;
}