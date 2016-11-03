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
#ifdef CONFIGURATION_LANGUAGE_ENGLISH
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
	
	// "tftp" command
	#define STRING_COMMAND_TFTP_USAGE "Envoie ou re\207oit des fichiers sur le net via le protocole TFTP.\n" \
		"Param\212tres : IP_Serveur Port_Serveur Commande Nom_Fichier\n" \
		" - IP_Serveur : l'adresse IP du serveur TFTP\n" \
		" - Port_Serveur : le port du serveur TFTP\n" \
		" - Commande : 'get' pour recevoir un fichier, 'put' pour envoyer un fichier\n" \
		" - Nom_Fichier : si la commande est 'get', nom du fichier \205 r\202cup\202rer sur le\n" \
		"   serveur ; si la commande est 'set', nom du fichier local \205 envoyer\n"
	#define STRING_COMMAND_TFTP_INVALID_IP_ADDRESS "Erreur : l'adresse IP est invalide.\n"
	#define STRING_COMMAND_TFTP_INVALID_COMMAND "Erreur : la commande n'est pas reconnue.\n"
	#define STRING_COMMAND_TFTP_FILE_NAME_TOO_LONG "Erreur : le nom du fichier est trop long.\n"
	#define STRING_COMMAND_TFTP_NETWORK_INITIALIZATION_FAILED "Erreur : impossible d'initialiser le r\202seau.\n"
	#define STRING_COMMAND_TFTP_NETWORK_SOCKET_INITIALIZATION_FAILED "Erreur : impossible de contacter le serveur.\n"
	// Messages working for all commands
	#define STRING_COMMAND_TFTP_GENERIC_NETWORK_RECEPTION_FAILED "Erreur lors de la r\202ception d'un paquet.\n"
	#define STRING_COMMAND_TFTP_GENERIC_NETWORK_TRANSMISSION_FAILED "Erreur lors de la transmission d'un paquet.\n"
	#define STRING_COMMAND_TFTP_GENERIC_SERVER_ERROR_1 "Erreur : le serveur a renvoy\202 l'erreur : "
	#define STRING_COMMAND_TFTP_GENERIC_SERVER_ERROR_2 ".\n"
	#define STRING_COMMAND_TFTP_GENERIC_BAD_NETWORK_PACKET_RECEIVED "Erreur : un paquet erron\202 a \202t\202 re\207u.\n"
	// "get" command specific messages
	#define STRING_COMMAND_TFTP_GET_CANT_OPEN_FILE "Erreur lors de l'ouverture du fichier local.\n"
	#define STRING_COMMAND_TFTP_GET_CANT_WRITE_TO_FILE "Erreur lors de l'\202criture dans le fichier.\n"
	#define STRING_COMMAND_TFTP_GET_STARTING_DOWNLOAD "T\202l\202chargement en cours...\n"
	#define STRING_COMMAND_TFTP_GET_DOWNLOAD_SUCCESSFUL_1 "T\202l\202chargement de "
	#define STRING_COMMAND_TFTP_GET_DOWNLOAD_SUCCESSFUL_2 " bloc(s) r\202ussi.\n"
	#define STRING_COMMAND_TFTP_GET_BAD_BLOCK_NUMBER_1 "Erreur : le bloc "
	#define STRING_COMMAND_TFTP_GET_BAD_BLOCK_NUMBER_2 " a \202t\202 re\207u alors que le bloc "
	#define STRING_COMMAND_TFTP_GET_BAD_BLOCK_NUMBER_3 " \202t\202 attendu.\n"
	// "put" command specific 
	#define STRING_COMMAND_TFTP_PUT_CANT_OPEN_FILE "Erreur : le fichier \205 envoyer est introuvable.\n"
	#define STRING_COMMAND_TFTP_PUT_STARTING_UPLOAD "Transmission en cours...\n"
	#define STRING_COMMAND_TFTP_PUT_BAD_BLOCK_NUMBER_1 "Erreur : le bloc "
	#define STRING_COMMAND_TFTP_PUT_BAD_BLOCK_NUMBER_2 " a \202t\202 re\207u par le serveur alors que le bloc "
	#define STRING_COMMAND_TFTP_PUT_BAD_BLOCK_NUMBER_3 " \202t\202 envoy\202.\n"
	#define STRING_COMMAND_TFTP_PUT_CANT_READ_FROM_FILE "Erreur lors de la lecture du fichier local.\n"
	#define STRING_COMMAND_TFTP_PUT_UPLOAD_SUCCESSFUL_1 "Envoi de "
	#define STRING_COMMAND_TFTP_PUT_UPLOAD_SUCCESSFUL_2 " bloc(s) r\202ussi.\n"
#endif

#endif