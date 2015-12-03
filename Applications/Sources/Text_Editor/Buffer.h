/** @file Buffer.h
 * This buffer contains the full text and allows some simple operations on it.
 * @author Adrien RICCIARDI
 */

#ifndef H_BUFFER_H
#define H_BUFFER_H

//-------------------------------------------------------------------------------------------------
// Variables
//-------------------------------------------------------------------------------------------------
/** The shared text buffer. */
extern char Buffer[];

/** How many characters are present in the buffer. */
extern unsigned int Buffer_Characters_Count;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Append a character somewhere in the buffer. All the characters behind the added one are shifted.
 * @param Index Where to add the character.
 * @param Character The character to add.
 * @return 0 if the character was successfully added,
 * @return 1 if the character is out of the buffer limits, if the buffer is full or if the index is too far from the text end.
 */
int BufferAddCharacter(unsigned int Index, char Character);

/** Remove a character from the buffer. All the following characters are shifted.
 * @param Index Where to remove the character.
 * @note The function does nothing if the index is out of the buffer bounds or if there is no character at the specified index.
 */
void BufferRemoveCharacter(unsigned int Index);

#if 0
// TODO later (may be useful in insert mode)
int BufferReplaceCharacter(unsigned int Index, char Character);
#endif

/** Find the first character of the specified line.
 * @param Line_Index The line to find index.
 * @return The character index. It is equal to Buffer_Characters_Count if the line index was too far from the text end.
 */
unsigned int BufferFindLineBeginning(unsigned int Line_Index);

/** Compute the number of lines that are currently present in the buffer.
 * @return The lines count.
 */
unsigned int BufferGetLinesCount(void);

unsigned int BufferGetCharactersCount(void);

#if 0
/** Get a pointer at the beginning of the requested line.
 * @param Index The line to display (starting from 0 to BufferGetLinesCount() - 1).
 * @return NULL if the line is not present in the buffer,
 * @return a valid pointer if the line was found.
 */
char *BufferGetLine(unsigned int Index);
#endif

void BufferDisplayPage(unsigned int Beginning_Line_Index);

/** Get the length in characters of the requested line.
 * @param Line_Index The line to find length.
 * @param Pointer_Length On output, contain the line length (limited to CONFIGURATION_DISPLAY_COLUMNS_COUNT - 1 if the line is too long).
 * @return 0 if the length was found (so the line exists),
 * @return 1 if the line does not exist (so the specified index is bad).
 */
int BufferGetLineLength(unsigned int Line_Index, unsigned int *Pointer_Length);

#endif