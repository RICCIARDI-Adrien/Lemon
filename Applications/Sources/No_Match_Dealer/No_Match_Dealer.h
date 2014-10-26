/** @file No_Match_Dealer.h
 * If the dealer gets 16 numbers (from range 0 to 99) with no match between them, the player wins.
 * @author Adrien RICCIARDI
 * @version 1.0 : 25/12/2012
 * @version 1.1 : 03/05/2014, updated program to fulfill new system requirements.
 */
#ifndef H_NO_MATCH_DEALER_H
#define H_NO_MATCH_DEALER_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
// English strings
#ifdef LEMON_LANGUAGE_ENGLISH
	#error "TODO"
// French strings
#else
	#define STRING_INSTRUCTIONS "Instructions : le dealer tire 16 nombres au hasard compris entre 0 et 99.\nSi tous les nombres sont uniques, le joueur gagne.\nSi au moins deux nombres sont identiques, le joueur perd.\n\n"
	#define STRING_GAME_LOST "Vous avez perdu, plusieurs nombres sont identiques !\n"
	#define STRING_GAME_WON "Vous avez gagn\202, tous les nombres sont diff\202rents !\n"
#endif

#endif