/** @file Strings.h
 * Contain all system strings in order to ease translation work.
 * @author Adrien RICCIARDI
 * @version 1.0 : 22/08/2013, moved strings from Kernel.h to this file.
 */
#ifndef H_STRINGS_H
#define H_STRINGS_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
// English
#ifdef LEMON_LANGUAGE_ENGLISH
	// TODO
	#error "Not done yet..."
// Italian
#elif defined(LEMON_LANGUAGE_ITALIAN)
	// TODO
	#error "Non è stato fatto per il momento..."
// French (default language)
#else
	// Kernel
	#define STRING_KERNEL_CONSOLE_DIVISION_ERROR_EXCEPTION "Erreur : le programme a tent\202 de diviser par z\202ro.\nAppuyez sur Entr\202e pour continuer.\n"
	#define STRING_KERNEL_CONSOLE_GENERAL_PROTECTION_FAULT_EXCEPTION "Erreur de protection g\202n\202rale. Le programme a \202t\202 stopp\202.\nAppuyez sur Entr\202e pour continuer.\n"
	#define STRING_KERNEL_CONSOLE_STACK_EXCEPTION "Erreur fatale : la pile noyau a d\202bord\202.\nAppuyez sur Entr\202e pour red\202marrer.\n"
	#define STRING_KERNEL_CONSOLE_FILE_SYSTEM_ERROR "Erreur fatale : le syst\212me de fichiers du disque dur n'est pas valide !\nAppuyez sur Entr\202e pour red\202marrer."
	#define STRING_KERNEL_CONSOLE_UNKNWOWN_SYSTEM_CALL_ERROR "Erreur : le programme a demand\202 un appel syst\212me inconnu.\nAppuyez sur Entr\202e pour continuer.\n"

	// Shell welcoming message shown only at system startup
	#define STRING_SHELL_WELCOME "Bienvenue sur Lemon !\n"
	
	// Shell copy command
	#define STRING_SHELL_COPY_FILE_BAD_PARAMETERS_COUNT "Erreur : mauvais nombre de param\212tres.\nUtilisation : copy Nom_Fichier_Source Nom_Fichier_Destination\n"
	#define STRING_SHELL_COPY_FILE_SOURCE_FILE_NOT_FOUND "Erreur : le fichier source est introuvable.\n"
	#define STRING_SHELL_COPY_FILE_DESTINATION_FILE_EXISTING_YET "Erreur : le fichier de destination existe d\202j\205.\n"
	#define STRING_SHELL_COPY_FILE_NOT_ENOUGH_ROOM "Erreur : il n'y a pas assez de place sur le syst\212me de fichiers.\n"
	#define STRING_SHELL_COPY_FILE_STARTING_COPY "Copie en cours...\n"
	#define STRING_SHELL_COPY_FILE_SUCCESS "Copie r\202ussie.\n"
	
	// Shell delete command
	#define STRING_SHELL_DELETE_FILE_BAD_PARAMETERS_COUNT "Erreur : mauvais nombre de param\212tres.\nUtilisation : delete Nom_Fichier\n"
	#define STRING_SHELL_DELETE_FILE_SUCCESS "Suppression r\202ussie.\n"
	#define STRING_SHELL_DELETE_FILE_FAILURE_1 "Erreur : le fichier '"
	#define STRING_SHELL_DELETE_FILE_FAILURE_2 "' n'existe pas !\n"

	// Shell download command
	#define STRING_SHELL_DOWNLOAD_WAITING_FOR_SERVER "Attente de la connexion au serveur...\nAppuyez sur Echap pour annuler.\n"
	#define STRING_SHELL_DOWNLOAD_SHOW_FILE_INFORMATIONS_1 "\nT\202l\202chargement du fichier '"
	#define STRING_SHELL_DOWNLOAD_SHOW_FILE_INFORMATIONS_2 "' ("
	#define STRING_SHELL_DOWNLOAD_SHOW_FILE_INFORMATIONS_3 " octets)...\n"
	#define STRING_SHELL_DOWNLOAD_FILE_SIZE_NULL "Abandon du t\202l\202chargement : il n'y a rien \205 t\202l\202charger.\n"
	#define STRING_SHELL_DOWNLOAD_FILE_SIZE_TOO_BIG "Abandon du t\202l\202chargement : la taille du fichier est trop importante pour le\nstocker en m\202moire.\n"
	#define STRING_SHELL_DOWNLOAD_DOWNLOADING_COMPLETED "T\202l\202chargement termin\202.\n"
	#define STRING_SHELL_DOWNLOAD_FINAL_QUESTION "\nQue souhaitez-vous faire ? (s)auvegarder, (a)nnuler : "
	#define STRING_SHELL_DOWNLOAD_ASK_FILE_NAME "Nom du fichier : "
	#define STRING_SHELL_DOWNLOAD_FILE_SAVE_COMPLETED "Sauvegarde r\202ussie.\n"
	#define STRING_SHELL_DOWNLOAD_BAD_FILE_NAME "Erreur : nom de fichier incorrect.\n"
	#define STRING_SHELL_DOWNLOAD_FILE_SAVE_FAILED "Echec de la sauvegarde ! Le syst\212me de fichiers est plein !\n"
	
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
	#define STRING_SHELL_VERSION "Version du syst\212me : 2.1.8 (" __DATE__ ", " __TIME__ ")\nCopyright (C) Adrien Ricciardi\n"

	// Shell error strings
	#define STRING_SHELL_ERROR_FILE_NOT_EXECUTABLE "Ce fichier n'est pas un programme !\n"
	#define STRING_SHELL_ERROR_UNKNOWN_COMMAND "Ce programme est introuvable !\n"
	#define STRING_SHELL_ERROR_FILE_TO_LOAD_LARGER_THAN_RAM "Le syst\212me ne dispose pas d'assez de m\202moire pour lancer ce programme !\n"
#endif

#endif