/** @file Numbers.h
 * A game in which the player has to find a number using dichotomy.
 * @author Adrien RICCIARDI
 * @version 1.0 : 20/10/2012
 * @version 1.1 : 31/12/2012
 * @version 1.2 : 03/05/2014, updated program to fulfill new system requirements.
 */
#ifndef H_NUMBERS_H
#define H_NUMBERS_H

//-------------------------------------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------------------------------------
/** The number to find is in range [1 ; MAXIMUM_NUMBER_VALUE]. */ 
#define MAXIMUM_NUMBER_VALUE 100000

/** How many attempts are allowed. */
#define ATTEMPTS_COUNT 20

// English strings
#ifdef LEMON_LANGUAGE_ENGLISH
	#error "TODO"
// French strings
#else
	#define STRING_INSTRUCTIONS_1 "Vous devez trouver un nombre compris entre 1 et "
	#define STRING_INSTRUCTIONS_2 ".\n" \
								  "Pour cela, vous ne disposerez que de "
	#define STRING_INSTRUCTIONS_3 " tentatives.\n" \
								  "Lorsque vous proposez un nombre, le jeu vous indique s'il est plus petit\n" \
								  "ou plus grand que celui recherch\202.\n" \
								  "Appuyez sur Echap pour quitter.\n\n"
	#define STRING_INSERT_NUMBER "Votre nombre : "
	#define STRING_PLAYER_LOST_1 "Vous avez perdu ! Le nombre \202tait "
	#define STRING_PLAYER_LOST_2 ".\n"
	#define STRING_PLAYER_WON_1 "F\202licitations, vous avez gagn\202 en "
	#define STRING_PLAYER_WON_2 " tentative(s) !\n"
	#define STRING_NUMBER_TOO_SMALL "Trop petit !\n"
	#define STRING_NUMBER_TOO_BIG "Trop grand !\n"
	#define STRING_REMAINING_ATTEMPTS_1 "Il vous reste "
	#define STRING_REMAINING_ATTEMPTS_2 " tentative(s).\n\n"
#endif

#endif