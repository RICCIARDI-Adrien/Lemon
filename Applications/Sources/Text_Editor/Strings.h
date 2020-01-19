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
#ifdef CONFIGURATION_GLOBAL_LANGUAGE_ENGLISH
	#error "TODO"
// French strings
#else
	#define STRING_USAGE "Erreur : vous devez sp\202cifier un nom de fichier.\nLe fichier sera ouvert s'il existe ou sera cr\202\202 s'il n'existe pas.\n"
	#define STRING_MESSAGE_TITLE_ERROR "ERREUR"
	#define STRING_MESSAGE_TITLE_WARNING "ATTENTION"
	#define STRING_ERROR_CANT_OPEN_FILE "Impossible d'ouvrir le fichier."
	#define STRING_ERROR_CANT_LOAD_FILE "Impossible de charger le contenu du fichier."
	#define STRING_ERROR_CANT_SAVE_FILE "Impossible de sauvegarder le fichier."
	#define STRING_WARNING_FILE_IS_TOO_BIG "Le fichier est trop grand. Seul le d\202but a \202t\202 charg\202."
	#define STRING_HIT_ENTER_TO_CONTINUE "Appuyez sur la touche Entr\202e pour continuer."
	
	#define STRING_MESSAGE_TEXT_INFORMATION_TITLE "Statistiques du document"
	#define STRING_MESSAGE_TEXT_INFORMATION_CONTENT_1 "Nombre de lignes : "
	#define STRING_MESSAGE_TEXT_INFORMATION_CONTENT_2 "\nNombre de caract\212res : "
	
	#define STRING_MESSAGE_UNSAVED_TEXT_WARNING_CONTENT "Les derni\212res modifications n'ont pas \202t\202 enregistr\202es.\nAppuyez sur Entr\202e pour sauver les modifications ou sur Echap pour quitter sans sauvegarder."
	#define STRING_MESSAGE_UNSAVED_TEXT_WARNING_FOOTER "Entr\202e : sauvegarder, Echap : quitter."
#endif

#endif