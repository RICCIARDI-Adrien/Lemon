/** @file Libraries_String.h
 * Contain string manipulation functions.
 * @author Adrien RICCIARDI
 */
#ifndef H_LIBRARIES_STRING_H
#define H_LIBRARIES_STRING_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Compare two strings.
 * @return 1 if the strings are the same,
 * @return 0 if the strings are different.
 */
int LibrariesStringCompare(char *String_1, char *String_2);

/** Copy a string into another string.
 * @param String_Source The string to copy.
 * @param String_Destination The destination string.
 */
void LibrariesStringCopy(char *String_Source, char *String_Destination);

/** Copy a string into another string.
 * @param String_Source The string to copy.
 * @param String_Destination The destination string.
 * @param Bytes_Count Copy up to this amount of bytes if the source string is too large.
 */
void LibrariesStringCopyUpToNumber(char *String_Source, char *String_Destination, unsigned int Bytes_Count);

/** Convert a string representation of a base 10 number to an unsigned integer.
 * @param String The string to convert (can contain only digits).
 * @return The converted number.
 * @note Providing a string that contains characters other than digits gives unpredictable result.
 */
unsigned int LibrariesStringConvertStringToUnsignedInteger(char *String);

/** Convert a number into its string representation.
 * @param Number The number to convert.
 * @param String_Destination On output, contain the converted number. Be sure that the string has room enough for the number.
 */
void LibrariesStringConvertUnsignedIntegerToString(unsigned int Number, char *String_Destination);

/** Convert a 32-bit value to an hexadecimal string (always padded to 8 characters).
 * @param Integer The 32-bit value to convert (it can be also an unsigned integer).
 * @param String_Hexadecimal_Output On output, contain the hexadecimal string. Make sure the string has enough room for 8 characters + terminating zero.
 */
void LibrariesStringConvertIntegerToHexadecimal(int Integer, char *String_Hexadecimal_Output);

/** Retrieve the characters count in a string.
 * @param String The string to get size.
 * @return The size of the string in characters (not including the terminating zero).
 */
unsigned int LibrariesStringGetSize(char *String);

/** Concatenate a string at the end of another string.
 * @param String_Destination The string to concatenate to.
 * @param String_To_Append The string to append to the other string.
 */
void LibrariesStringConcatenate(char *String_Destination, char *String_To_Append);

/** Concatenate up to a specified number of a string's characters at the end of another string.
 * @param String_Destination The string to concatenate to.
 * @param String_To_Append The string to append to the other string.
 * @param Characters_Count The maximum amount of characters to concatenate.
 */
void LibrariesStringConcatenateUpToNumber(char *String_Destination, char *String_To_Append, unsigned int Characters_Count);

/** Tell if the character is a lowercase or an uppercase letter.
 * @param Character The character to test.
 * @return 0 if the character is not a letter,
 * @return 1 if the character is a letter.
 */
int LibrariesStringIsCharacterALetter(char Character);

/** Tell if the character is a digit or not.
 * @param Character The character to test.
 * @return 0 if the character is not a digit,
 * @return 1 if the character is a digit.
 */
int LibrariesStringIsCharacterADigit(char Character);

#endif
