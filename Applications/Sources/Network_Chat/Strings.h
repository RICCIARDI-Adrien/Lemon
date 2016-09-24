/** @file Strings.h
 * Gather all strings to ease translation.
 * @author Adrien RICCIARDI
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
	#define STRING_USAGE_2 " Adresse_IP_Correspondant Port_Correspondant\n"
	#define STRING_ERROR_INVALID_IP_ADDRESS "Erreur : l'adresse IP est invalide.\n"
	#define STRING_ERROR_NETWORK_INITIALIZATION "Erreur : le support r\202seau n'est pas activ\202.\n"
	#define STRING_ERROR_NETWORK_SOCKET_INITIALIZATION "Erreur lors de l'initialisation de la socket r\202seau (est-ce que la machine de\ndestination est op\202rationnelle ?).\n"
#endif

#endif