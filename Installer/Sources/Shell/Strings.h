/** @file Strings.h
 * Gather all displayable strings to ease translation.
 * @author Adrien RICCIARDI
 * @version 1.0 : 27/04/2014
 */
#ifndef H_STRINGS_H
#define H_STRINGS_H

//-------------------------------------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------------------------------------
#ifdef LEMON_LANGUAGE_ENGLISH
	// TODO
	#error "Not done yet..."
// Italian
#elif defined(LEMON_LANGUAGE_ITALIAN)
	// TODO
	#error "Non è stato fatto per il momento..."
// French (default language)
#else
	#define STRING_TITLE "### Lemon - Assistant d'installation ###\n\n"
	#define STRING_WELCOME "Bienvenue dans l'assistant d'installation du syst\212me.\nVous allez installer le syst\212me sur votre disque dur.\n\n"
	#define STRING_CAUTION_MESSAGE_1 "ATTENTION : l'installation peut d\202truire le contenu actuel du disque dur !\n"
	#define STRING_CAUTION_MESSAGE_2 "Souhaitez-vous continuer ? [o/n] : "
	#define STRING_CHARACTER_YES 'o'
	#define STRING_CHARACTER_NO 'n'
	#define STRING_INSTALLATION_ABORTED "Installation annul\202e.\n"
	#define STRING_REBOOT "\nRetirez le m\202dia d'installation et appuyez sur une touche pour red\202marrer.\n"
	#define STRING_INSTALLATION_BEGINNING "\nD\202but de l'installation...\n"
	#define STRING_CREATING_FILE_SYSTEM "Cr\202ation du syst\212me de fichiers...\n"
	#define STRING_INSTALLING_FILES "Installation des fichiers...\n"
	#define STRING_FILE_NAME_1 "Installation de '"
	#define STRING_FILE_NAME_2 "' ("
	#define STRING_FILE_NAME_3 " octets)...\n"
	#define STRING_INSTALLATION_COMPLETED "Installation termin\202e.\n"
	
	// Partition selection menu
	#define STRING_PARTITION_MENU_SHOW_PARTITION_TABLE_1 "\nChoisissez une partition sur laquelle installer le syst\212me.\n"
	#define STRING_PARTITION_MENU_SHOW_PARTITION_TABLE_2 "Table des partitions :\n"
	#define STRING_PARTITION_MENU_PARTITIONS_DETAILS_1 "Partition "
	#define STRING_PARTITION_MENU_PARTITIONS_DETAILS_2 " : type : "
	#define STRING_PARTITION_MENU_PARTITIONS_DETAILS_3 ", secteur LBA de d\202part : "
	#define STRING_PARTITION_MENU_PARTITIONS_DETAILS_4 "\n              taille en octets : "
	#define STRING_PARTITION_MENU_PARTITIONS_DETAILS_EMPTY_PARTITION " (vide)"
	#define STRING_PARTITION_MENU_CHOOSE_PARTITION_NUMBER "Num\202ro de la partition (1-4) : "
	
	// Errors
	#define STRING_ERROR_BAD_FILE_SYSTEM_PARAMETERS "Erreur : les param\212tres du syst\212me de fichiers sont invalides.\n"
	#define STRING_ERROR_FILE_SYSTEM_TOO_BIG_FOR_HARD_DISK_1 "Erreur : le disque dur est trop petit pour stocker le syst\212me de fichiers.\nTaille du disque : "
	#define STRING_ERROR_FILE_SYSTEM_TOO_BIG_FOR_HARD_DISK_2 " octets\n"
	#define STRING_ERROR_CANT_CREATE_FILE "Erreur : impossible de cr\202er le fichier.\n"
#endif

#endif