/** @file Brain_Calculation.h
 * A game in which the player should compute a maximum amount of small calculus without mistake.
 * @author Adrien RICCIARDI
 * @version 1.0 : 27/12/2012
 * @version 1.1 : 03/05/2014, updated program to fulfill new system requirements.
 */
#ifndef H_BRAIN_CALCULATION_H
#define H_BRAIN_CALCULATION_H

//-------------------------------------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------------------------------------
// English strings
#ifdef LEMON_LANGUAGE_ENGLISH
	#error "TODO"
// French strings
#else
	#define STRING_INSTRUCTIONS "Instructions : vous devez r\202soudre le plus grand nombre de calculs sans vous\ntromper. Il est possible de choisir le niveau de difficult\202 :\n   -e : facile, nombres \205 1 chiffre.\n   -m : moyen, nombres \205 2 chiffres.\n   -h : difficile, nombres \205 3 chiffres.\nLe niveau de difficult\202 par d\202faut est moyen.\nAppuyez sur Echap pour quitter.\n\n"
	#define STRING_GOOD_RESULT "F\202licitations, vous avez trouv\202 le bon r\202sultat !\n\n"
	#define STRING_BAD_RESULT "Erreur, le r\202sultat \202tait %d.\n"
	#define STRING_CORRECT_ANSWERS_COUNT "Vous avez r\202solu %d calcul(s).\n"
#endif

#endif