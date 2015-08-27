/** @file Games.h
 * Gather all game functions prototype.
 * @author Adrien RICCIARDI
 */
#ifndef H_GAMES_H
#define H_GAMES_H

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
/** A game in which the player should compute a maximum amount of small calculus without mistake. */
void BrainCalculation(void);

/** If the dealer gets 16 numbers (from range 0 to 99) with no match between them, the player wins. */
void NoMatchDealer(void);

/** Some ASCII characters are randomly raining on the screen, as in the classic BSD game (yes, it is completely useless). */
void Rain(void);

#endif