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
	#define STRING_MAIN_MENU_GAME_NO_MATCH_DEALER "No Match Dealer"
	#define STRING_MAIN_MENU_GAME_RAIN "Gouttes de pluie"
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
	
	// No match dealer
	#define STRING_NO_MATCH_DEALER_INSTRUCTIONS "Instructions : le dealer tire 16 nombres au hasard compris entre 0 et 99.\nSi tous les nombres sont uniques, le joueur gagne.\nSi au moins deux nombres sont identiques, le joueur perd.\n\n"
	#define STRING_NO_MATCH_DEALER_GAME_LOST "Vous avez perdu, plusieurs nombres sont identiques !\n"
	#define STRING_NO_MATCH_DEALER_GAME_WON "Vous avez gagn\202, tous les nombres sont diff\202rents !\n"
	#define STRING_NO_MATCH_DEALER_ASK_FOR_RETRY "Appuyez sur Entr\202e pour relancer ou sur Echap pour quitter.\n\n"
	
	// Rain
	#define STRING_RAIN_MENU_TITLE "Gouttes de pluie"
	#define STRING_RAIN_MENU_PROMPT "Choisissez le type de pluie : "
	#define STRING_RAIN_MENU_SLOW "Clairsem\202e"
	#define STRING_RAIN_MENU_NORMAL "Soutenue"
	#define STRING_RAIN_MENU_FAST "Diluvienne"
	#define STRING_RAIN_MENU_ULTRAFAST "Torrentielle"
	#define STRING_RAIN_MENU_EXIT "Retour"
	#define STRING_RAIN_STATISTICS_1 "Gouttes de pluie "
	#define STRING_RAIN_STATISTICS_2 " ; Boucles "
#endif

#endif