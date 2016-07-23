/** @file Strings.h
 * Gather all displayable strings to ease translation.
 * @author Adrien RICCIARDI
 */
#ifndef H_STRINGS_H
#define H_STRINGS_H

//-------------------------------------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------------------------------------
#if defined(CONFIGURATION_LANGUAGE_ENGLISH)
	// TODO
	#error "Not done yet..."
// Italian
#elif defined(CONFIGURATION_LANGUAGE_ITALIAN)
	// TODO
	#error "Non è stato fatto per il momento..."
// French (default language)
#else
	#define STRING_TITLE "                    ### Lemon - Assistant d'installation ###\n\n"
	#define STRING_WELCOME "Bienvenue dans l'assistant d'installation du syst\212me.\n"
	
	#define STRING_SECTION_WARNING_TITLE "\nAvertissement\n"
	#define STRING_SECTION_WARNING_MESSAGE "ATTENTION : l'installation peut d\202truire le contenu actuel du disque dur !\n"
	#define STRING_SECTION_WARNING_QUESTION "Continuer ? [o/n] : "
	#define STRING_SECTION_WARNING_INSTALLATION_ABORTED "Installation annul\202e.\n"
	
	#define STRING_SECTION_HARD_DISK_TITLE "\nPartitionnement du disque dur\n"
	#define STRING_SECTION_HARD_DISK_MESSAGE "Taper 'o' pour utiliser tout l'espace du disque dur (attention : le disque sera compl\212tement effac\202) ou taper 'n' pour choisir une partition existante.\n"
	#define STRING_SECTION_HARD_DISK_QUESTION "Utiliser tout l'espace du disque dur ? [o/n] : "
	
	#define STRING_REBOOT "\nRetirez le m\202dia d'installation et appuyez sur une touche pour red\202marrer.\n"
	#define STRING_INSTALLATION_BEGINNING "\nD\202but de l'installation\n"
	#define STRING_CREATING_FILE_SYSTEM "Cr\202ation du syst\212me de fichiers...\n"
	#define STRING_INSTALLING_FILES "Installation des fichiers...\n"
	#define STRING_FILE_NAME_1 "Installation de '"
	#define STRING_FILE_NAME_2 "' ("
	#define STRING_FILE_NAME_3 " octets)...\n"
	#define STRING_INSTALLATION_COMPLETED "Installation termin\202e.\n"
	
	// Partition selection menu
	#define STRING_PARTITION_MENU_SHOW_PARTITION_TABLE_1 "\nChoisissez une partition sur laquelle installer le syst\212me\n"
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
	#define STRING_ERROR_FILE_SYSTEM_TOO_BIG_FOR_HARD_DISK_2 " secteurs.\n"
	#define STRING_ERROR_CANT_OPEN_FILE "Erreur : impossible d'ouvrir le fichier en \202criture.\n"
	#define STRING_ERROR_CANT_WRITE_FILE_CONTENT "Erreur : impossible d'\202crire le contenu du fichier.\n"
	
	// Question answer characters
	#define STRING_CHARACTER_YES 'o'
	#define STRING_CHARACTER_NO 'n'
#endif

#endif