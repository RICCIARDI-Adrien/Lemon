/** @file Strings.h
 * Gather all program strings.
 * @author Adrien RICCIARDI
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
	// Main menu
	#define STRING_MAIN_MENU_TITLE "-- Jeux --"
	#define STRING_MAIN_MENU_PROMPT "Choisissez un jeu : "
	#define STRING_MAIN_MENU_GAME_NUMBERS "Trouver un nombre"
	#define STRING_MAIN_MENU_GAME_BRAIN_CALCULATION "Calcul mental"
	#define STRING_MAIN_MENU_EXIT "Quitter"

	// Brain calculation
	#define STRING_BRAIN_CALCULATION_MENU_TITLE "-- Calcul mental --"
	#define STRING_BRAIN_CALCULATION_MENU_PROMPT "Choisissez une difficult\202 : "
	#define STRING_BRAIN_CALCULATION_MENU_EASY_DIFFICULTY "Facile (1 chiffre)"
	#define STRING_BRAIN_CALCULATION_MENU_MEDIUM_DIFFICULTY "Moyen (2 chiffres)"
	#define STRING_BRAIN_CALCULATION_MENU_HARD_DIFFICULTY "Difficile (3 chiffres)"
	#define STRING_BRAIN_CALCULATION_MENU_EXIT "Retour"
	#define STRING_BRAIN_CALCULATION_INSTRUCTIONS "Instructions : vous devez r\202soudre le plus grand nombre de calculs sans vous\ntromper.\nAppuyez sur Echap pour quitter.\n\n"
	#define STRING_BRAIN_CALCULATION_GOOD_RESULT "F\202licitations, vous avez trouv\202 le bon r\202sultat !\n\n"
	#define STRING_BRAIN_CALCULATION_BAD_RESULT_1 "Erreur, le r\202sultat \202tait "
	#define STRING_BRAIN_CALCULATION_BAD_RESULT_2 ".\n"
	#define STRING_BRAIN_CALCULATION_CORRECT_ANSWERS_COUNT_1 "Vous avez r\202solu "
	#define STRING_BRAIN_CALCULATION_CORRECT_ANSWERS_COUNT_2 " calcul(s).\nAppuyez sur une touche pour continuer."
	//#define STRING_BRAIN_CALCULATION_ASK_TO_CONTINUE "\nVoulez-vous (Q)uitter ou (R)\202essayer ? "
#endif

#endif