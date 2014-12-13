/** @file Tests_String.c
 * Test the Libraries String API.
 * @author Adrien RICCIARDI
 * @version 1.0 : 13/12/2014
 */
#include <System.h>
#include "Display_Message.h"
#include "Tests.h"

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Test the StringCompare() function.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
static int TestsStringCompare(void)
{
	char *String_1 = "abcdefgh", *String_2 = "abcd", *String_3 = "abcdefgh";
	
	// Testing with two different strings
	if (StringCompare(String_2, String_1) != 0)
	{
		DisplayMessageError("two different strings are considered equal.");
		return 1;
	}
	
	// Testing with two equal strings
	if (StringCompare(String_1, String_3) != 1)
	{
		DisplayMessageError("two equal strings are considered different.");
		return 1;
	}
	
	return 0;
}

/** Test the StringConcatenate() function.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
static int TestsStringConcatenate(void)
{
	char String_Destination[30] = "This is ", *String_To_Concatenate = "a test", *String_Full = "This is a test";
	int i;
	
	StringConcatenate(String_Destination, String_To_Concatenate);
	
	// Check the full string
	for (i = 0; String_Full[i] != 0; i++)
	{
		if (String_Destination[i] != String_Full[i])
		{
			DisplayMessageError("bad string concatenation.");
			return 1;
		}
	}
	return 0;
}

/** Test the StringConvertStringToUnsignedInteger() function.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
static int TestsStringConvertStringToUnsignedInteger(void)
{
	char *String_Numbers[] = {"12345", "0", "16", "4294967295", "1000000"};
	unsigned int i, Results[] = {12345, 0, 16, 4294967295UL, 1000000};
	
	for (i = 0; i < sizeof(String_Numbers) / sizeof(char *); i++)
	{
		ScreenWriteUnsignedInteger(Results[i]);
		ScreenWriteCharacter('\n');
		
		if (StringConvertStringToUnsignedInteger(String_Numbers[i]) != Results[i])
		{
			DisplayMessageError("failed string to number conversion.");
			return 1;
		}
	}
	
	return 0;
}

/** Test the StringCopy() function.
 * @return 0 if all tests were successful,
 * @return 1 if a test failed.
 */
static int TestsStringCopy(void)
{
	int i;
	char *String_Source = "Irrelevant string content.", String_Destination[50];
	
	StringCopy(String_Source, String_Destination);
	
	// Check if the copy was successful
	for (i = 0; String_Source[i] != 0; i++)
	{
		if (String_Source[i] != String_Destination[i])
		{
			DisplayMessageError("bad string copy.");
			return 1;
		}
	}	
	return 0;
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int TestsString(void)
{
	DisplayMessageTestStarting("Libraries StringCompare()");
	if (TestsStringCompare() != 0) return 1;
	DisplayMessageTestSuccessful();
	
	DisplayMessageTestStarting("Libraries StringConcatenate()");
	if (TestsStringConcatenate() != 0) return 1;
	DisplayMessageTestSuccessful();
	
	DisplayMessageTestStarting("Libraries StringConvertStringToUnsignedInteger()");
	if (TestsStringConvertStringToUnsignedInteger() != 0) return 1;
	DisplayMessageTestSuccessful();
	
	DisplayMessageTestStarting("Libraries StringCopy()");
	if (TestsStringCopy() != 0) return 1;
	DisplayMessageTestSuccessful();
	
	return 0;
}