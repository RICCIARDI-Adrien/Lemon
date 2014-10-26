/** @file String.h
 * Contain string manipulation functions.
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/10/2014
 */
#ifndef H_STRING_H
#define H_STRING_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Compare two strings.
 * @return 1 if the strings are the same,
 * @return 0 if the strings are different.
 */
int StringCompare(char *String_1, char *String_2);

/** Copy a string into another string.
 * @param String_Source The string to copy.
 * @param String_Destination The destination string.
 */
void StringCopy(char *String_Source, char *String_Destination);

/** Convert a string representation of a base 10 number to an unsigned integer.
 * @param String The string to convert (can contain only digits).
 * @return The converted number.
 * @note Providing a string that contains characters other than digits gives unpredictable result.
 */
unsigned int StringConvertStringToUnsignedInteger(char *String);

/** Retrieve the characters count in a string.
 * @param String The string to get size.
 * @return The size of the string in characters (not including the terminating zero).
 */
unsigned int StringSize(char *String);

#endif