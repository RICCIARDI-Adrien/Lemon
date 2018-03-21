/** @file Strings.h
 * Contain all system strings in order to ease translation work.
 * @author Adrien RICCIARDI
 */
#ifndef H_STRINGS_H
#define H_STRINGS_H

#include <Configuration.h>

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
/** Convert the macro identifier to a C string. */
#define STRING_CONVERT_MACRO_NAME_TO_STRING(X) #X
/** Convert the macro value to a C string. The preprocessor needs two passes to do the conversion, so the STRING_CONVERT_MACRO_NAME_TO_STRING() is needed. */
#define STRING_CONVERT_MACRO_VALUE_TO_STRING(X) STRING_CONVERT_MACRO_NAME_TO_STRING(X)

// English
#if defined(CONFIGURATION_LANGUAGE_ENGLISH)
	// TODO
	#error "Not done yet..."
// Italian
#elif defined(CONFIGURATION_LANGUAGE_ITALIAN)
	// TODO
	#error "Non è stato fatto per il momento..."
// French (default language)
#else
	// Kernel
	#define STRING_KERNEL_EXCEPTION_DIVISION_BY_ZERO "Erreur : le programme a tent\202 de diviser par z\202ro.\nAppuyez sur Entr\202e pour continuer.\n"
	#define STRING_KERNEL_EXCEPTION_GENERAL_PROTECTION_FAULT "Erreur de protection g\202n\202rale. Le programme a \202t\202 stopp\202.\nAppuyez sur Entr\202e pour continuer.\n"
	#define STRING_KERNEL_EXCEPTION_STACK_OVERFLOW "Erreur fatale : la pile noyau a d\202bord\202.\nAppuyez sur Entr\202e pour red\202marrer.\n"
	#define STRING_KERNEL_ERROR_INVALID_FILE_SYSTEM "Erreur fatale : le syst\212me de fichiers du disque dur n'est pas valide !\nAppuyez sur Entr\202e pour red\202marrer."
	#define STRING_KERNEL_ERROR_UNKNOWN_SYSTEM_CALL "Erreur : le programme a demand\202 un appel syst\212me inconnu.\nAppuyez sur Entr\202e pour continuer.\n"
	#define STRING_KERNEL_ERROR_HARD_DISK_NOT_LBA_COMPATIBLE "Erreur : le disque dur n'est pas compatible avec l'adressage LBA.\n"
	#define STRING_KERNEL_ERROR_SATA_HARD_DISK_NOT_FOUND "Erreur : le disque dur SATA est introuvable.\n"
	#define STRING_KERNEL_ERROR_FAILED_TO_CREATE_RAM_DISK "Erreur : impossible de cr\202er le syst\212me de fichiers en RAM.\n"
	#define STRING_KERNEL_ERROR_FAILED_TO_POPULATE_RAM_DISK "Erreur : impossible d'installer les fichiers dans le disque RAM.\n"
	#define STRING_KERNEL_ERROR_ETHERNET_CONTROLLER_NOT_FOUND "Erreur : aucun contr\223leur ethernet n'a \202t\202 d\202tect\202.\n"
	#define STRING_KERNEL_ERROR_ETHERNET_CONTROLLER_BAD_DRIVER "Erreur : le contr\223leur ethernet n'est pas compatible avec le pilote.\n"
	
	// SATA hard disk driver
	#define STRING_DRIVER_HARD_DISK_SATA_ERROR_INPUT_OUTPUT "Erreur : impossible d'acc\202der au disque dur SATA.\nAppuyez sur Entr\202e pour continuer.\n"

	// Shell welcoming message shown only at system startup
	#define STRING_SHELL_WELCOME "Bienvenue sur Lemon !\n"
	
	// Shell copy command
	#define STRING_SHELL_COPY_FILE_BAD_PARAMETERS_COUNT "Erreur : mauvais nombre de param\212tres.\nUtilisation : copy Nom_Fichier_Source Nom_Fichier_Destination\n"
	#define STRING_SHELL_COPY_FILE_SOURCE_FILE_NOT_FOUND "Erreur : le fichier source est introuvable.\n"
	#define STRING_SHELL_COPY_FILE_DESTINATION_FILE_EXISTING_YET "Erreur : le fichier de destination existe d\202j\205.\n"
	#define STRING_SHELL_COPY_FILE_NOT_ENOUGH_ROOM "Erreur : il n'y a pas assez de place sur le syst\212me de fichiers.\n"
	#define STRING_SHELL_COPY_SOURCE_FILE_READ_ERROR "Erreur lors de la lecture du fichier source.\n"
	#define STRING_SHELL_COPY_DESTINATION_FILE_WRITE_ERROR "Erreur lors de l'\202criture dans le fichier destination.\n"
	#define STRING_SHELL_COPY_FILE_STARTING_COPY "Copie en cours...\n"
	#define STRING_SHELL_COPY_FILE_SUCCESS "Copie r\202ussie.\n"
	
	// Shell delete command
	#define STRING_SHELL_DELETE_FILE_BAD_PARAMETERS_COUNT "Erreur : mauvais nombre de param\212tres.\nUtilisation : delete Nom_Fichier\n"
	#define STRING_SHELL_DELETE_FILE_SUCCESS "Suppression r\202ussie.\n"
	#define STRING_SHELL_DELETE_FILE_FAILURE_1 "Erreur : le fichier '"
	#define STRING_SHELL_DELETE_FILE_FAILURE_2 "' n'existe pas !\n"

	// Shell download command
	#define STRING_SHELL_DOWNLOAD_WAITING_FOR_SERVER "Attente de la connexion au serveur...\nAppuyez sur Echap pour annuler.\n"
	#define STRING_SHELL_DOWNLOAD_SHOW_FILE_INFORMATIONS_1 "T\202l\202chargement du fichier '"
	#define STRING_SHELL_DOWNLOAD_SHOW_FILE_INFORMATIONS_2 "' ("
	#define STRING_SHELL_DOWNLOAD_SHOW_FILE_INFORMATIONS_3 " octets)...\n"
	#define STRING_SHELL_DOWNLOAD_NO_MORE_FILE_LIST_ENTRY "Impossible de lancer le t\202l\202chargement : aucun emplacement libre sur le syst\212me de fichiers.\n"
	#define STRING_SHELL_DOWNLOAD_FILE_SIZE_NULL "Abandon du t\202l\202chargement : il n'y a rien \205 t\202l\202charger.\n"
	#define STRING_SHELL_DOWNLOAD_FILE_SIZE_TOO_BIG "Abandon du t\202l\202chargement : la taille du fichier est trop importante pour le\nstocker en m\202moire.\n"
	#define STRING_SHELL_DOWNLOAD_NO_MORE_BLOCK_LIST_ENTRY "Abandon du t\202l\202chargement : il n'y a pas assez de place sur le syst\212me de\nfichiers pour stocker le fichier.\n"
	#define STRING_SHELL_DOWNLOAD_DOWNLOADING_COMPLETED "T\202l\202chargement termin\202.\n"
	#define STRING_SHELL_DOWNLOAD_BAD_FILE_NAME "Erreur : nom de fichier incorrect.\n"
	#define STRING_SHELL_DOWNLOAD_FILE_OPENING_FAILED "Erreur lors de l'ouverture du fichier de destination.\n"
	#define STRING_SHELL_DOWNLOAD_FILE_WRITING_FAILED "Erreur lors de l'\202criture du fichier.\n"
	
	// Shell list command
	#define STRING_SHELL_LIST_WAIT_FOR_USER_INPUT "Appuyez sur une touche pour continuer."

	// Shell rename command
	#define STRING_SHELL_RENAME_FILE_BAD_PARAMETERS_COUNT "Erreur : mauvais nombre de param\212tres.\nUtilisation : rename Ancien_Nom_Fichier Nouveau_Nom_Fichier\n"
	#define STRING_SHELL_RENAME_FILE_ALREADY_EXISTS_1 "Erreur : le fichier destination '"
	#define STRING_SHELL_RENAME_FILE_ALREADY_EXISTS_2 "' existe d\202j\205 !\n"
	#define STRING_SHELL_RENAME_FILE_NOT_FOUND_1 "Erreur : le fichier source '"
	#define STRING_SHELL_RENAME_FILE_NOT_FOUND_2 "' n'existe pas !\n"
	#define STRING_SHELL_RENAME_FILE_SUCCESS "Renommage r\202ussi.\n"
	#define STRING_SHELL_RENAME_FILE_FAILURE "Erreur : le nouveau nom est d\202j\205 attribu\202.\n"

	// Shell size command
	#define STRING_SHELL_FILE_SIZE_BAD_PARAMETERS_COUNT "Erreur : mauvais nombre de param\212tres.\nUtilisation : size Nom_Fichier\n"
	#define STRING_SHELL_FILE_SIZE_BAD_FILE_NAME_1 "Erreur : le fichier '"
	#define STRING_SHELL_FILE_SIZE_BAD_FILE_NAME_2 "' n'existe pas !\n"
	#define STRING_SHELL_FILE_SIZE_SHOW_SIZE_1 "Taille : "
	#define STRING_SHELL_FILE_SIZE_SHOW_SIZE_2 " octets.\n"

	// Shell version command
	#define STRING_SHELL_VERSION "Version du syst\212me : 2.5.3 (" __DATE__ ", " __TIME__ ")\n" \
		"Copyright (C) Adrien Ricciardi\n\n" \
		"Configuration :\n" \
		STRING_BUILD_CONFIGURATION_VARIABLES

	// Shell error strings
	#define STRING_SHELL_ERROR_UNKNOWN_COMMAND "Ce programme est introuvable !\n"
	#define STRING_SHELL_ERROR_CANT_READ_PROGRAM_HEADER "Impossible de lire l'en-t\210te du fichier.\n"
	#define STRING_SHELL_ERROR_FILE_NOT_EXECUTABLE "Ce fichier n'est pas un programme !\n"
	#define STRING_SHELL_ERROR_CANT_LOAD_PROGRAM "Impossible de charger le programme en m\202moire.\n"
	#define STRING_SHELL_ERROR_FILE_TO_LOAD_LARGER_THAN_RAM "Le syst\212me ne dispose pas d'assez de m\202moire pour lancer ce programme.\n"
	
	// Installer shell strings
	#define STRING_SHELL_INSTALLER_TITLE "                    ### Lemon - Assistant d'installation ###\n\n"
	#define STRING_SHELL_INSTALLER_WELCOME "Bienvenue dans l'assistant d'installation du syst\212me.\n"
	
	#define STRING_SHELL_INSTALLER_SECTION_WARNING_TITLE "\nAvertissement\n"
	#define STRING_SHELL_INSTALLER_SECTION_WARNING_MESSAGE "ATTENTION : l'installation peut d\202truire le contenu actuel du disque dur !\n"
	#define STRING_SHELL_INSTALLER_SECTION_WARNING_QUESTION "Continuer ? [o/n] : "
	#define STRING_SHELL_INSTALLER_SECTION_WARNING_INSTALLATION_ABORTED "Installation annul\202e.\n"
	
	#define STRING_SHELL_INSTALLER_SECTION_HARD_DISK_TITLE "\nPartitionnement du disque dur\n"
	#define STRING_SHELL_INSTALLER_SECTION_HARD_DISK_MESSAGE "Taper 'o' pour utiliser tout l'espace du disque dur (attention : le disque sera compl\212tement effac\202) ou taper 'n' pour choisir une partition existante.\n"
	#define STRING_SHELL_INSTALLER_SECTION_HARD_DISK_QUESTION "Utiliser tout l'espace du disque dur ? [o/n] : "
	
	#define STRING_SHELL_INSTALLER_REBOOT "\nRetirez le m\202dia d'installation et appuyez sur une touche pour red\202marrer.\n"
	#define STRING_SHELL_INSTALLER_INSTALLATION_BEGINNING "\nD\202but de l'installation\n"
	#define STRING_SHELL_INSTALLER_CREATING_FILE_SYSTEM "Cr\202ation du syst\212me de fichiers...\n"
	#define STRING_SHELL_INSTALLER_INSTALLING_FILES "Installation des fichiers...\n"
	#define STRING_SHELL_INSTALLER_FILE_NAME_1 "Installation de '"
	#define STRING_SHELL_INSTALLER_FILE_NAME_2 "' ("
	#define STRING_SHELL_INSTALLER_FILE_NAME_3 " octets)...\n"
	#define STRING_SHELL_INSTALLER_INSTALLATION_COMPLETED "Installation termin\202e.\n"
	
	// Partition selection menu
	#define STRING_SHELL_INSTALLER_PARTITION_MENU_SHOW_PARTITION_TABLE_1 "\nChoisissez une partition sur laquelle installer le syst\212me\n"
	#define STRING_SHELL_INSTALLER_PARTITION_MENU_SHOW_PARTITION_TABLE_2 "Table des partitions :\n"
	#define STRING_SHELL_INSTALLER_PARTITION_MENU_PARTITIONS_DETAILS_1 "Partition "
	#define STRING_SHELL_INSTALLER_PARTITION_MENU_PARTITIONS_DETAILS_2 " : type : "
	#define STRING_SHELL_INSTALLER_PARTITION_MENU_PARTITIONS_DETAILS_3 ", secteur LBA de d\202part : "
	#define STRING_SHELL_INSTALLER_PARTITION_MENU_PARTITIONS_DETAILS_4 "\n              taille en octets : "
	#define STRING_SHELL_INSTALLER_PARTITION_MENU_PARTITIONS_DETAILS_EMPTY_PARTITION " (vide)"
	#define STRING_SHELL_INSTALLER_PARTITION_MENU_CHOOSE_PARTITION_NUMBER "Num\202ro de la partition (1-4) : "
	
	// Errors
	#define STRING_SHELL_INSTALLER_ERROR_BAD_FILE_SYSTEM_PARAMETERS "Erreur : les param\212tres du syst\212me de fichiers sont invalides.\n"
	#define STRING_SHELL_INSTALLER_ERROR_FILE_SYSTEM_TOO_BIG_FOR_HARD_DISK_1 "Erreur : le disque dur est trop petit pour stocker le syst\212me de fichiers.\nTaille du disque : "
	#define STRING_SHELL_INSTALLER_ERROR_FILE_SYSTEM_TOO_BIG_FOR_HARD_DISK_2 " secteurs.\n"
	#define STRING_SHELL_INSTALLER_ERROR_CANT_OPEN_FILE "Erreur : impossible d'ouvrir le fichier en \202criture.\n"
	#define STRING_SHELL_INSTALLER_ERROR_CANT_WRITE_FILE_CONTENT "Erreur : impossible d'\202crire le contenu du fichier.\n"
	
	// Question answer characters
	#define STRING_SHELL_INSTALLER_CHARACTER_YES 'o'
	#define STRING_SHELL_INSTALLER_CHARACTER_NO 'n'
#endif

#endif