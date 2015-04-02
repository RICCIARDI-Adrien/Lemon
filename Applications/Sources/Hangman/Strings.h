/** @file Strings.h
 * Gather all program strings.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/03/2015
 */
#ifndef H_STRINGS_H
#define H_STRINGS_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
// English strings
#ifdef LEMON_LANGUAGE_ENGLISH
	#error "TODO"
// French strings
#else
	#define STRING_TITLE "Jeu du Pendu"
	#define STRING_BAD_LETTERS_LIST "Lettres absentes du mot :"
	#define STRING_REMAINING_ATTEMPTS "Tentatives restantes :"
	
	#define STRING_WORD_FOUND_1 "Bravo, vous avez trouv\202 le mot !\n"
	#define STRING_WORD_FOUND_2 "Appuyez sur Entr\202e pour continuer."
	#define STRING_WORD_NOT_FOUND_1 "Perdu !\n"
	#define STRING_WORD_NOT_FOUND_2 "Le mot \202tait \""
	#define STRING_WORD_NOT_FOUND_3 "\".\nAppuyez sur Entr\202e pour quitter."
	#define STRING_GAME_WON_1 "F\202licitations, vous avez trouv\202 tous les mots !\n"
	#define STRING_GAME_WON_2 "Appuyez sur Entr\202e pour continuer."
	
	#define STRINGS_WORDS "INSPIRATION", "CORNICHE", "TENTATIVE"
#endif
	
#endif