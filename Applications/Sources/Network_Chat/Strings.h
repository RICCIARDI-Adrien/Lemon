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
	#define STRING_ERROR_NETWORK_INITIALIZATION "Erreur lors de l'initialisation de la pile r\202seau (est-ce que la passerelle par d\202faut est op\202rationnelle ?).\n"
	#define STRING_ERROR_NETWORK_SOCKET_INITIALIZATION "Erreur lors de l'initialisation de la socket r\202seau (est-ce que la machine de\ndestination est op\202rationnelle ?).\n"
#endif

#endif