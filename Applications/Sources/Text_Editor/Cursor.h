/** @file Cursor.h
 * Handle the cursor movements.
 * The cursor location is relative to the whole text, not only the display area.
 * @author Adrien RICCIARDI
 */
#ifndef H_CURSOR_H
#define H_CURSOR_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Go to the beginning of the previous line. Does nothing if the buffer beginning is reached. */
void CursorGoToPreviousLine(void);

/** Go to the beginning of the next line. Does nothing if the buffer end is reached. */
void CursorGoToNextLine(void);

//int CursorGoToPreviousWord(void);
//int CursorGoToNextWord(void);

/** Go one line upper.
 * @return 0 if there is no need to redraw the text page (no scrolling is needed),
 * @return 1 if the text page must be redrawn.
 */
int CursorMoveToUp(void);

/** Go down line upper.
 * @return 0 if there is no need to redraw the text page (no scrolling is needed),
 * @return 1 if the text page must be redrawn.
 */
int CursorMoveToDown(void);

/** Go one character to the left. The cursor goes to the upper line if it reached the line beginning.
 * @return 0 if there is no need to redraw the text page (no scrolling is needed),
 * @return 1 if the text page must be redrawn.
 */
int CursorMoveToLeft(void);

/** Go one character to the right. The cursor goes to the downer line if it reached the line end.
 * @return 0 if there is no need to redraw the text page (no scrolling is needed),
 * @return 1 if the text page must be redrawn.
 */
int CursorMoveToRight(void);

/** Move the cursor to the beginning of the line. */
void CursorGoToLineBeginning(void);

/** Move the cursor to the end of the line. */
void CursorGoToLineEnd(void);

/** Retrieve the cursor location in the buffer.
 * @return The character index.
 */
unsigned int CursorGetBufferCharacterIndex(void);

/** Retrieve the display row coordinate of the cursor.
 * @return The row display coordinate.
 */
unsigned int CursorGetDisplayRow(void);

/** Retrieve the display column coordinate of the cursor.
 * @return The column display coordinate.
 */
unsigned int CursorGetDisplayColumn(void);

/** TODO */
unsigned int CursorGetBufferRow(void);

#endif