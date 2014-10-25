/** @file Tests_Ctype.c
 * Test the standard ctype library.
 * @author Adrien RICCIARDI
 * @version 1.0 : 09/06/2014
 */
#include <ctype.h>
#include <stdio.h>
#include "Tests.h"

//-------------------------------------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Test the isalnum() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestIsAlnum(void)
{
	int i, Result;

	for (i = 0; i < 256; i++)
	{
		Result = isalnum(i);

		// Only digits, capital letters and lowercase letters must be accepted
		if (((i >= '0') && (i <= '9')) || ((i >= 'A') && (i <= 'Z')) || ((i >= 'a') && (i <= 'z')))
		{
			if (!Result) return 0; // Error : a good character was not correctly recognized
		}
		else if (Result) return 0; // Error : a bad character has been recognized as good
	}

	return 1;
}

/** Test the isalpha() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestIsAlpha(void)
{
	int i, Result;

	for (i = 0; i < 256; i++)
	{
		Result = isalpha(i);

		// Only capital and lowercase letters must be accepted
		if (((i >= 'A') && (i <= 'Z')) || ((i >= 'a') && (i <= 'z')))
		{
			if (!Result) return 0; // Error : a good character was not correctly recognized
		}
		else if (Result) return 0; // Error : a bad character has been recognized as good
	}

	return 1;
}

/** Test the isblank() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestIsBlank(void)
{
	int i, Result;

	for (i = 0; i < 256; i++)
	{
		Result = isblank(i);

		// Only '\t' and space must be accepted
		if ((i == '\t') || (i == ' '))
		{
			if (!Result) return 0; // Error : a good character was not correctly recognized
		}
		else if (Result) return 0; // Error : a bad character has been recognized as good
	}

	return 1;
}

/** Test the iscntrl() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestIsCntrl(void)
{
	int i, Result;

	for (i = 0; i < 256; i++)
	{
		Result = iscntrl(i);

		// Only control characters must be accepted
		if (((i >= 0) && (i <= 31)) || (i == 127))
		{
			if (!Result) return 0; // Error : a good character was not correctly recognized
		}
		else if (Result) return 0; // Error : a bad character has been recognized as good
	}

	return 1;
}

/** Test the isdigit() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestIsDigit(void)
{
	int i, Result;

	for (i = 0; i < 256; i++)
	{
		Result = isdigit(i);

		// Only digits must be accepted
		if ((i >= '0') && (i <= '9'))
		{
			if (!Result) return 0; // Error : a good character was not correctly recognized
		}
		else if (Result) return 0; // Error : a bad character has been recognized as good
	}

	return 1;
}

/** Test the isgraph() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestIsGraph(void)
{
	int i, Result;

	for (i = 0; i < 256; i++)
	{
		Result = isgraph(i);

		// Only control, digits, lowercase and uppercase characters must be accepted
		if ((i >= 33) && (i <= 126))
		{
			if (!Result) return 0; // Error : a good character was not correctly recognized
		}
		else if (Result) return 0; // Error : a bad character has been recognized as good
	}

	return 1;
}

/** Test the islower() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestIsLower(void)
{
	int i, Result;

	for (i = 0; i < 256; i++)
	{
		Result = islower(i);

		// Only lowercase characters must be accepted
		if ((i >= 'a') && (i <= 'z'))
		{
			if (!Result) return 0; // Error : a good character was not correctly recognized
		}
		else if (Result) return 0; // Error : a bad character has been recognized as good
	}

	return 1;
}

/** Test the isprint() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestIsPrint(void)
{
	int i, Result;

	for (i = 0; i < 256; i++)
	{
		Result = isprint(i);

		// Only control, digits, lowercase, uppercase and space characters must be accepted
		if ((i >= 32) && (i <= 126))
		{
			if (!Result) return 0; // Error : a good character was not correctly recognized
		}
		else if (Result) return 0; // Error : a bad character has been recognized as good
	}

	return 1;
}

/** Test the ispunct() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestIsPunct(void)
{
	int i, Result;

	for (i = 0; i < 256; i++)
	{
		Result = ispunct(i);

		// Only punctuation characters must be accepted
		if (((i >= 33) && (i <= 47)) || ((i >= 58) && (i <= 64)) || ((i >= 91) && (i <= 96)) || ((i >= 123) && (i <= 126)))
		{
			if (!Result) return 0; // Error : a good character was not correctly recognized
		}
		else if (Result) return 0; // Error : a bad character has been recognized as good
	}

	return 1;
}

/** Test the isspace() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestIsSpace(void)
{
	int i, Result;

	for (i = 0; i < 256; i++)
	{
		Result = isspace(i);

		if (((i == ' ') || (i == '\f') || (i == '\n') || (i == '\r') || (i == '\t') || (i == '\v')))
		{
			if (!Result) return 0; // Error : a good character was not correctly recognized
		}
		else if (Result) return 0; // Error : a bad character has been recognized as good
	}

	return 1;
}

/** Test the isupper() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestIsUpper(void)
{
	int i, Result;

	for (i = 0; i < 256; i++)
	{
		Result = isupper(i);

		// Only uppercase characters must be accepted
		if ((i >= 'A') && (i <= 'Z'))
		{
			if (!Result) return 0; // Error : a good character was not correctly recognized
		}
		else if (Result) return 0; // Error : a bad character has been recognized as good
	}

	return 1;
}

/** Test the isxdigit() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestIsXDigit(void)
{
	int i, Result;

	for (i = 0; i < 256; i++)
	{
		Result = isxdigit(i);

		// Only hexadecimal characters must be accepted
		if (((i >= '0') && (i <= '9')) || ((i >= 'A') && (i <= 'F')) || ((i >= 'a') && (i <= 'f')))
		{
			if (!Result) return 0; // Error : a good character was not correctly recognized
		}
		else if (Result) return 0; // Error : a bad character has been recognized as good
	}

	return 1;
}

/** Test the tolower() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestToLower(void)
{
	int i, Result;

	// Transform uppercase characters in lowercase ones
	for (i = 0; i < 256; i++)
	{
		Result = tolower(i);

		if (((i >= 'A') && (i <= 'Z')) && (Result != i + 32)) return 0; // Error : an uppercase character was not converted in lowercase
	}

	return 1;
}

/** Test the toupper() function.
 * @return 1 if the test was successful,
 * @return 0 if the test failed.
 */
static int TestToUpper(void)
{
	int i, Result;

	// Transform lowercase characters in uppercase ones
	for (i = 0; i < 256; i++)
	{
		Result = toupper(i);

		if (((i >= 'a') && (i <= 'z')) && (Result != i - 32)) return 0; // Error : a lowercase character was not converted in uppercase
	}

	return 1;
}

//-------------------------------------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------------------------------------
int TestsCtype(void)
{
	printf("isalnum()\n");
	if (!TestIsAlnum()) return 0;

	printf("isalpha()\n");
	if (!TestIsAlpha()) return 0;

	printf("isblank()\n");
	if (!TestIsBlank()) return 0;

	printf("iscntrl()\n");
	if (!TestIsCntrl()) return 0;
	
	printf("isdigit()\n");
	if (!TestIsDigit()) return 0;

	printf("isgraph()\n");
	if (!TestIsGraph()) return 0;
	
	printf("islower()\n");
	if (!TestIsLower()) return 0;

	printf("isprint()\n");
	if (!TestIsPrint()) return 0;

	printf("ispunct()\n");
	if (!TestIsPunct()) return 0;

	printf("isspace()\n");
	if (!TestIsSpace()) return 0;

	printf("isupper()\n");
	if (!TestIsUpper()) return 0;

	printf("isxdigit()\n");
	if (!TestIsXDigit()) return 0;

	printf("toupper()\n");
	if (!TestToUpper()) return 0;

	printf("tolower()\n");
	if (!TestToLower()) return 0;
	
	return 1;
}