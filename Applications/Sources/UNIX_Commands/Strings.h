/** @file Strings.h
 * Gather all program strings to ease translation.
 * @author Adrien RICCIARDI
 * @version 1.0 : 20/03/2015
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
	#define STRING_USAGE_1 "Utilisation : "
	#define STRING_USAGE_2 " Commande [Param\212tres]\nTaper '"
	#define STRING_USAGE_3 " <Commande> -h' pour obtenir plus de d\202tails sur l'utilisation d'une\ncommande.\n"
	
	#define STRING_AVAILABLE_COMMANDS "Commandes disponibles : "
	
	#define STRING_UNKNOWN_COMMAND "Commande inconnue.\n"
	
	// "df" command
	#define STRING_COMMAND_DF_USAGE "Affiche l'espace libre restant sur le disque dur.\nCette commande n'a pas de param\212tre.\n"
	#define STRING_COMMAND_DF_FILES_COUNT "Nombre de fichiers : "
	#define STRING_COMMAND_DF_REMAINING_STORAGE_1 "Espace de stockage utilis\202 : "
	#define STRING_COMMAND_DF_REMAINING_STORAGE_2 " octets"
	
	// "ls" command
	#define STRING_COMMAND_LS_USAGE "Liste dans l'ordre alphab\202tique tous les fichiers pr\202sents sur le disque dur.\nCette commande n'a pas de param\212tre.\n"
	#define STRING_COMMAND_LS_NOT_ENOUGH_SPACE_TO_STORE_FILES "Erreur : le programme n'a pas assez de m\202moire pour afficher tous les fichiers.\n"
	#define STRING_COMMAND_LS_DISPLAYED_UNIT " octets\n"
	#define STRING_COMMAND_LS_WAIT_FOR_USER_INPUT "Appuyez sur une touche pour continuer."
	
	// "more" command
	#define STRING_COMMAND_MORE_USAGE "Affiche le contenu d'un fichier texte ligne apr\212s ligne.\n" \
		"L'unique param\212tre de cette commande est le fichier \205 afficher.\n" \
		"Utilisation des touches :\n" \
		"  - Entr\202e : affiche la ligne suivante\n" \
		"  - Espace : affiche la page suivante\n" \
		"  - q      : quitte le programme\n"
	#define STRING_COMMAND_MORE_FILE_NOT_FOUND_1 "Erreur : le fichier '"
	#define STRING_COMMAND_MORE_FILE_NOT_FOUND_2 "' est introuvable.\n"
	#define STRING_COMMAND_MORE_ERROR_READING_FROM_FILE "Erreur : impossible de lire le fichier.\n"
#endif

#endif