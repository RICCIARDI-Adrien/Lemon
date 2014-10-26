/** @file Rain.h
 * Some ASCII characters are randomly raining on the screen (yes, it is completely useless).
 * @author Adrien RICCIARDI
 * @version 1.0 : 05/01/2013
 * @version 1.1 : 29/04/2014, updated program to fulfill new system requirements.
 */
#ifndef H_RAIN_H
#define H_RAIN_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
// Raining speed
#define RAINING_SPEED_SLOW 10
#define RAINING_SPEED_NORMAL 4
#define RAINING_SPEED_FAST 1
#define RAINING_SPEED_ULTRAFAST 0

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
	#define STRING_USAGE_1 "Erreur : mauvais param\212tres.\nUtilisation : "
	#define STRING_USAGE_2 " " PARAMETER_RAINING_SPEED_SLOW " | " PARAMETER_RAINING_SPEED_NORMAL " | " PARAMETER_RAINING_SPEED_FAST " | " PARAMETER_RAINING_SPEED_ULTRAFAST "\n"
	#define STRING_STATISTICS_1 "Gouttes de pluie "
	#define STRING_STATISTICS_2 " ; Boucles "
#endif

#endif