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
#ifdef CONFIGURATION_GLOBAL_LANGUAGE_ENGLISH
	#error "TODO"
// French strings
#else
	#define STRING_USAGE_1 "Utilisation : "
	#define STRING_USAGE_2 " Commande [Param\212tres]\nTaper '"
	#define STRING_USAGE_3 " <Commande> -h' pour obtenir plus de d\202tails sur l'utilisation d'une\ncommande.\n"
	
	#define STRING_AVAILABLE_COMMANDS "Commandes disponibles : "
	
	#define STRING_UNKNOWN_COMMAND "Commande inconnue.\n"
	
	// "date" command
	#define STRING_COMMAND_DATE_USAGE "Affiche la date et l'heure courantes.\n" \
		"Utilisez les fonctionnalit\202s de votre BIOS pour les r\202gler.\n" \
		"Cette commande n'a pas de param\212tre.\n"
	#define STRING_COMMAND_DATE_DAY_SUNDAY "Dimanche"
	#define STRING_COMMAND_DATE_DAY_MONDAY "Lundi"
	#define STRING_COMMAND_DATE_DAY_TUESDAY "Mardi"
	#define STRING_COMMAND_DATE_DAY_WEDNESDAY "Mercredi"
	#define STRING_COMMAND_DATE_DAY_THURSDAY "Jeudi"
	#define STRING_COMMAND_DATE_DAY_FRIDAY "Vendredi"
	#define STRING_COMMAND_DATE_DAY_SATURDAY "Samedi"
	#define STRING_COMMAND_DATE_MONTH_JANUARY "janvier"
	#define STRING_COMMAND_DATE_MONTH_FEBRUARY "f\202vrier"
	#define STRING_COMMAND_DATE_MONTH_MARCH "mars"
	#define STRING_COMMAND_DATE_MONTH_APRIL "avril"
	#define STRING_COMMAND_DATE_MONTH_MAY "mai"
	#define STRING_COMMAND_DATE_MONTH_JUNE "juin"
	#define STRING_COMMAND_DATE_MONTH_JULY "juillet"
	#define STRING_COMMAND_DATE_MONTH_AUGUST "ao\226t"
	#define STRING_COMMAND_DATE_MONTH_SEPTEMBER "septembre"
	#define STRING_COMMAND_DATE_MONTH_OCTOBER "octobre"
	#define STRING_COMMAND_DATE_MONTH_NOVEMBER "novembre"
	#define STRING_COMMAND_DATE_MONTH_DECEMBER "d\202cembre"
	
	// "df" command
	#define STRING_COMMAND_DF_USAGE "Affiche l'espace libre restant sur le disque dur.\n" \
		"Cette commande n'a pas de param\212tre.\n"
	#define STRING_COMMAND_DF_FILES_COUNT "Nombre de fichiers : "
	#define STRING_COMMAND_DF_REMAINING_STORAGE_1 "Espace de stockage utilis\202 : "
	#define STRING_COMMAND_DF_REMAINING_STORAGE_2 " octets"
	
	// "diff" command
	#define STRING_COMMAND_DIFF_USAGE "Compare deux fichiers et indique s'ils sont identiques ou diff\202rents.\n" \
		"Param\212tres : Fichier_1 Fichier_2\n" \
		"Fichier_1 et Fichier_2 sont les deux fichiers \205 comparer.\n"
	#define STRING_COMMAND_DIFF_FILE_CANT_BE_OPENED_1 "Erreur : le fichier '"
	#define STRING_COMMAND_DIFF_FILE_CANT_BE_OPENED_2 "' ne peut pas \210tre ouvert.\n"
	#define STRING_COMMAND_DIFF_SAME_FILE_CANT_BE_REOPENED "Attention, le m\210me fichier ne peut pas \210tre ouvert deux fois.\n"
	#define STRING_COMMAND_DIFF_FILE_READ_ERROR_1 "Erreur de lecture du fichier '"
	#define STRING_COMMAND_DIFF_FILE_READ_ERROR_2 "'.\n"
	#define STRING_COMMAND_DIFF_FILES_ARE_DIFFERENT "Les fichiers sont diff\202rents.\n"
	#define STRING_COMMAND_DIFF_FILES_ARE_EQUAL "Les fichiers sont identiques.\n"
	
	// "ls" command
	#define STRING_COMMAND_LS_USAGE "Liste dans l'ordre alphab\202tique tous les fichiers pr\202sents sur le disque dur.\n" \
		"Cette commande n'a pas de param\212tre.\n"
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
		"Param\212tres : Commande Nom_Fichier\n" \
		" - Commande : 'get' pour recevoir un fichier, 'put' pour envoyer un fichier\n" \
		" - Nom_Fichier : si la commande est 'get', nom du fichier \205 r\202cup\202rer sur le\n" \
		"   serveur ; si la commande est 'put', nom du fichier local \205 envoyer\n" \
		"L'adresse IP et le port du serveur se trouvent dans le fichier de configuration du syst\212me.\n"
	#define STRING_COMMAND_TFTP_CAN_RETRIEVE_SERVER_IP_ADDRESS "Erreur : impossible de trouver l'adresse IP du serveur dans le fichier de\nconfiguration.\n"
	#define STRING_COMMAND_TFTP_CAN_RETRIEVE_SERVER_PORT "Erreur : impossible de trouver le port du serveur dans le fichier de\nconfiguration.\n"
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
	// "put" command specific messages
	#define STRING_COMMAND_TFTP_PUT_CANT_OPEN_FILE "Erreur : le fichier \205 envoyer est introuvable.\n"
	#define STRING_COMMAND_TFTP_PUT_STARTING_UPLOAD "Transmission en cours...\n"
	#define STRING_COMMAND_TFTP_PUT_BAD_BLOCK_NUMBER_1 "Erreur : le bloc "
	#define STRING_COMMAND_TFTP_PUT_BAD_BLOCK_NUMBER_2 " a \202t\202 re\207u par le serveur alors que le bloc "
	#define STRING_COMMAND_TFTP_PUT_BAD_BLOCK_NUMBER_3 " \202t\202 envoy\202.\n"
	#define STRING_COMMAND_TFTP_PUT_CANT_READ_FROM_FILE "Erreur lors de la lecture du fichier local.\n"
	#define STRING_COMMAND_TFTP_PUT_UPLOAD_SUCCESSFUL_1 "Envoi de "
	#define STRING_COMMAND_TFTP_PUT_UPLOAD_SUCCESSFUL_2 " bloc(s) r\202ussi.\n"
	
	// "uptime" command
	#define STRING_COMMAND_UPTIME_USAGE "Indique le temps depuis lequel le syst\212me est d\202marr\202.\n" \
		"Cette commande n'a pas de param\212tre.\n"
	#define STRING_COMMAND_UPTIME_BOOTED_TIME_1 "Syst\212me d\202marr\202 depuis "
	#define STRING_COMMAND_UPTIME_BOOTED_TIME_2 " jour(s), "
	#define STRING_COMMAND_UPTIME_BOOTED_TIME_3 " heure(s), "
	#define STRING_COMMAND_UPTIME_BOOTED_TIME_4 " minute(s), "
	#define STRING_COMMAND_UPTIME_BOOTED_TIME_5 " seconde(s).\n"
#endif

#endif
