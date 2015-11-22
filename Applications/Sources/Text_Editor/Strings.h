/** @file Strings.h
 * Gather all the program strings.
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
	#define STRING_USAGE "Erreur : vous devez sp\202cifier un nom de fichier.\nLe fichier sera ouvert s'il existe ou sera cr\202\202 s'il n'existe pas.\n"
	#define STRING_ERROR_CANT_OPEN_FILE "Erreur : impossible d'ouvrir fichier.\n"
	#define STRING_ERROR_CANT_LOAD_FILE "Erreur : impossible de charger le contenu du fichier.\n"
	#define STRING_FILE_IS_TOO_BIG "Attention : le fichier est trop grand. Seul le d\202but a \202t\202 charg\202.\nAppuyez sur une touche pour continuer.\n"
#endif

#endif