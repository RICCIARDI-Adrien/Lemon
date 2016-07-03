/** @file Standard_Functions.h
 * Standard libc functions.
 * @author Adrien RICCIARDI
 * @warning These functions have the same behaviour than libc functions, but their prototypes are customized for speed improvement.
 */
#ifndef H_STANDARD_FUNCTIONS_H
#define H_STANDARD_FUNCTIONS_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** The indispensable NULL value. */
#define NULL ((void *) 0)

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Compare String_1 to String_2.
 * @return 0 if the strings are equal,
 * @return -1 if String_1 < String_2,
 * @return 1 if String_1 > String_2.
 */
int strcmp(char *String_1, char *String_2);

/** Compare Byte_Count bytes of String_1 to String_2.
 * @return 0 if the strings are equal,
 * @return -1 if String_1 < String_2,
 * @return 1 if String_1 > String_2.
 */
int strncmp(char *String_1, char *String_2, unsigned int Byte_Count);

/** Copy a whole string into another buffer.
 * @param String_Destination The destination buffer where to copy the source string.
 * @param String_Source The string to copy.
 */
void strcpy(char *String_Destination, char *String_Source);

/** Copy a fixed amount of bytes from a string into another buffer.
 * @param String_Destination The destination buffer where to copy the source string.
 * @param String_Source The string to copy.
 * @param Bytes_Count How many bytes to copy.
 */
void strncpy(char *String_Destination, char *String_Source, unsigned int Bytes_Count);

/** Fill a buffer with a specified value.
 * @param Pointer_Buffer The buffer to fill.
 * @param Value The value used to fill the buffer.
 * @param Bytes_Count How many bytes to fill.
 */
void memset(void *Pointer_Buffer, unsigned char Value, unsigned int Bytes_Count);

/** Copy a memory area into another.
 * @param Pointer_Destination_Buffer The destination memory area.
 * @param Pointer_Source_Buffer The source memory area.
 * @param Bytes_Count How many bytes to copy.
 */
void memcpy(void *Pointer_Destination_Buffer, void *Pointer_Source_Buffer, unsigned int Bytes_Count);

/** Concatenate a string at the end of another. 
 * @param String_Destination The string at the end of which the other string will be concatenated.
 * @param String The string to concatenate.
 */
void strcat(char *String_Destination, char *String);

/** Retrieve the size of a string in characters.
 * @param String The string to which retrieve size.
 * @return The string size.
 */
unsigned int strlen(char *String);

/** Convert a binary number to its string representation.
 * @param Number The number to convert.
 * @return A static string containing the ASCII representation of the number.
 */
char *itoa(unsigned int Number);

#endif