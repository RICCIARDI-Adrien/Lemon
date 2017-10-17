/** @file Tests_String.c
 * Test the Libraries String API.
 * @author Adrien RICCIARDI
 */
#include <System.h>
#include "Display_Message.h"
#include "Tests.h"

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int TestsSystemStringCompare(void)
{
	char *String_1 = "abcdefgh", *String_2 = "abcd", *String_3 = "abcdefgh";
	
	// Testing with two different strings
	if (SystemStringCompare(String_2, String_1))
	{
		DisplayMessageError("two different strings are considered equal.");
		return 1;
	}
	
	// Testing with two equal strings
	if (!SystemStringCompare(String_1, String_3))
	{
		DisplayMessageError("two equal strings are considered different.");
		return 1;
	}
	
	return 0;
}

int TestsStringConcatenate(void)
{
	char String_Destination[30] = "This is ", *String_To_Concatenate = "a test", *String_Full = "This is a test";
	int i;
	
	SystemStringConcatenate(String_Destination, String_To_Concatenate);
	
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

int TestsStringConcatenateUpToNumber(void)
{
	char String_Destination[30] = "This is ", *String_To_Concatenate = "a test", *String_First_Test_Result = "This is a test", *String_Second_Test_Result = "This is a testa t";
	int i;
	
	// First test
	StringConcatenateUpToNumber(String_Destination, String_To_Concatenate, 40);
	// Check the string
	for (i = 0; String_First_Test_Result[i] != 0; i++)
	{
		if (String_Destination[i] != String_First_Test_Result[i])
		{
			DisplayMessageError("first concatenation failed.");
			return 1;
		}
	}
	
	// Second test
	StringConcatenateUpToNumber(String_Destination, String_To_Concatenate, 3);
	// Check the string
	for (i = 0; String_Second_Test_Result[i] != 0; i++)
	{
		if (String_Destination[i] != String_Second_Test_Result[i])
		{
			DisplayMessageError("second concatenation failed.");
			return 1;
		}
	}
	
	return 0;
}

int TestsStringConvertStringToUnsignedInteger(void)
{
	char *String_Numbers[] = {"12345", "0", "16", "4294967295", "1000000"};
	unsigned int i, Results[] = {12345, 0, 16, 4294967295UL, 1000000};
	
	for (i = 0; i < SYSTEM_GET_ARRAY_ELEMENTS_COUNT(Results); i++)
	{
		ScreenWriteString("Testing string \"");
		ScreenWriteString(String_Numbers[i]);
		ScreenWriteString("\"...\n");
		
		if (SystemStringConvertStringToUnsignedInteger(String_Numbers[i]) != Results[i])
		{
			DisplayMessageError("failed string to number conversion.");
			return 1;
		}
	}
	
	return 0;
}

int TestsStringConvertUnsignedIntegerToString(void)
{
	char *String_Results[] = {"12345", "0", "16", "4294967295", "1000000"}, String_Result[20];
	unsigned int i, Numbers[] = {12345, 0, 16, 4294967295UL, 1000000};
	
	for (i = 0; i < SYSTEM_GET_ARRAY_ELEMENTS_COUNT(Numbers); i++)
	{
		ScreenWriteString("Testing number ");
		ScreenWriteString(String_Results[i]);
		ScreenWriteString("...\n");
		
		SystemStringConvertUnsignedIntegerToString(Numbers[i], String_Result);
		if (!SystemStringCompare(String_Result, String_Results[i]))
		{
			DisplayMessageError("failed number to string conversion.");
			return 1;
		}
	}
	
	return 0;
}

int TestsStringConvertIntegerToHexadecimal(void)
{
	char *String_Results[] = {"00000000", "00000001", "FFFFFFFF", "12345678", "DEADBEEF", "42AC8F9B", "00010B57"}, String_Result[9];
	unsigned int i, Numbers[] = {0, 1, 0xFFFFFFFF, 0x12345678, 0xDEADBEEF, 0x42AC8F9B, 0x00010B57};
	
	for (i = 0; i < SYSTEM_GET_ARRAY_ELEMENTS_COUNT(Numbers); i++)
	{
		ScreenWriteString("Testing number ");
		ScreenWriteString(String_Results[i]);
		ScreenWriteString("...\n");
		
		SystemStringConvertIntegerToHexadecimal((int) Numbers[i], String_Result);
		if (!SystemStringCompare(String_Result, String_Results[i]))
		{
			DisplayMessageError("failed to convert number to hexadecimal string.");
			return 1;
		}
	}
	
	return 0;
}

int TestsSystemStringCopy(void)
{
	int i;
	char *String_Source = "Irrelevant string content.", String_Destination[50];
	
	SystemStringCopy(String_Source, String_Destination);
	
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

int TestsStringGetSize(void)
{
	char *String_Empty = "", String[] = "123456789"; // String must be an array in order to use sizeof() to get its size
	
	// Test with an empty string
	if (SystemStringGetSize(String_Empty) != 0)
	{
		DisplayMessageError("an empty string has not a size of 0.\n");
		return 1;
	}
	
	// Test with an non-empty string
	if (SystemStringGetSize(String) != sizeof(String) - 1) // -1 to remove the terminating zero automatically added by the compiler
	{
		DisplayMessageError("the string size is bad.\n");
		return 1;
	}
	
	return 0;
}