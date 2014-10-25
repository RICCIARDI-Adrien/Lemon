/** @file Rain.h
 * Some ASCII characters are randomly raining on the screen (yes, it is completely useless).
 * @author Adrien RICCIARDI
 * @version 1.0 : 05/01/2013
 * @version 1.1 : 29/04/2014, updated program to fulfill new system requirements.
 */
#ifndef H_RAIN_H
#define H_RAIN_H

//-------------------------------------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------------------------------------
// Raining speed
#define RAINING_SPEED_SLOW 20
#define RAINING_SPEED_NORMAL 10
#define RAINING_SPEED_FAST 4
#define RAINING_SPEED_ULTRAFAST 1

// Speed command line parameters value
#define PARAMETER_RAINING_SPEED_SLOW "-slow"
#define PARAMETER_RAINING_SPEED_NORMAL "-normal"
#define PARAMETER_RAINING_SPEED_FAST "-fast"
#define PARAMETER_RAINING_SPEED_ULTRAFAST "-ultrafast"

// English strings
#ifdef LEMON_LANGUAGE_ENGLISH
	#error "TODO"
// French strings
#else
	#define STRING_USAGE "Erreur : mauvais param\212tres.\nUtilisation : %s " PARAMETER_RAINING_SPEED_SLOW " | " PARAMETER_RAINING_SPEED_NORMAL " | " PARAMETER_RAINING_SPEED_FAST " | " PARAMETER_RAINING_SPEED_ULTRAFAST "\n"
	#define STRING_STATISTICS "Gouttes de pluie %u ; Boucles %u"
#endif

#endif