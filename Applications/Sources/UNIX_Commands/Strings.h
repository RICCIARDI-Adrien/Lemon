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
	#define STRING_USAGE_2 " Commande [Param\212tres]\n"
	
	#define STRING_AVAILABLE_COMMANDS "Commandes disponibles : "
	
	#define STRING_UNKNOWN_COMMAND "Commande inconnue.\n"
	
	// "df" command
	#define STRING_COMMAND_DF_USAGE "Affiche l'espace libre restant sur le disque dur.\nCette commande n'a pas de param\212tre.\n"
	#define STRING_COMMAND_DF_FILES_COUNT "Nombre de fichiers : "
	#define STRING_COMMAND_DF_REMAINING_STORAGE_1 "Espace de stockage utilis\202 : "
	#define STRING_COMMAND_DF_REMAINING_STORAGE_2 " octets"
#endif

#endif