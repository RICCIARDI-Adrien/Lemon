/** @file Cursor.h
 * Handle the cursor movements.
 * The cursor location is relative to the whole text, not only the screen area.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/02/2015
 */
#ifndef H_CURSOR_H
#define H_CURSOR_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
// /** Go to the beginning of the previous line.
//  * @return 1 if the cursor is located at the previous line beginning,
//  * @return 0 if the beginning of the file is reached.
//  */
// int CursorGoToPreviousLine(void);
// int CursorGoToNextLine(void);

void CursorGoToPreviousLine(void);
void CursorGoToNextLine(void);

//int CursorGoToPreviousWord(void);
//int CursorGoToNextWord(void);

void CursorMoveToTop(void);
void CursorMoveToBottom(void);
void CursorMoveToLeft(void); // TODO peut remonter une ligne
void CursorMoveToRight(void); // TODO peut descendre une ligne

//void CursorSetLocation(unsigned int Row, unsigned int Column); // TODO si position trop éloignée pour la fin mettre en bas du fichier
void CursorSetToHome(void);

/** Retrieve the cursor location in the buffer.
 * @return The character index.
 */
unsigned int CursorGetCharacterIndex(void);

/** Retrieve the line on which the cursor is.
 * @return The line.
 */
unsigned int CursorGetLineLocation(void);

/** Retrieve the screen row coordinate of the cursor.
 * @return The row screen coordinate.
 */
unsigned int CursorGetScreenRow(void);

/** Retrieve the screen column coordinate of the cursor.
 * @return The column screen coordinate.
 */
unsigned int CursorGetScreenColumn(void);

#if 0
/** Convert a in-buffer character index value to the corresponding screen coordinates.
 * @param Character_Index The character index value.
 * @param Pointer_Screen_Row On output, contain the screen row coordinate.
 * @param Pointer_Screen_Column On output, contain the screen column coordinate.
 */
void CursorConvertCharacterIndexToScreenLocation(unsigned int Character_Index, unsigned int *Pointer_Screen_Row, unsigned int *Pointer_Screen_Column);
#endif

//CursorGetDisplayLocation(*r, *c)

#endif