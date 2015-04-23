/** @file Strings.h
 * Gather all strings to ease translation.
 * @author Adrien RICCIARDI
 * @version 1.0 : 26/10/2014
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
	#define STRING_USAGE_2 " Commande\n\n"
	
	#define STRING_AVAILABLE_COMMANDS "Liste des commandes disponibles :\n"
	
	#define STRING_UNKNOWN_COMMAND_1 "La commande '"
	#define STRING_UNKNOWN_COMMAND_2 "' n'existe pas.\n" 
	
	// Commands short description
	#define STRING_COMMAND_SHORT_DESCRIPTION_CLEAR "Efface l'\202cran."
	#define STRING_COMMAND_SHORT_DESCRIPTION_COPY "Copie un fichier."
	#define STRING_COMMAND_SHORT_DESCRIPTION_DELETE "Supprime un fichier."
	#define STRING_COMMAND_SHORT_DESCRIPTION_DOWNLOAD "T\202l\202charge un fichier depuis le port s\202rie."
	#define STRING_COMMAND_SHORT_DESCRIPTION_HELP "Fournit de l'aide sur les commandes existantes."
	#define STRING_COMMAND_SHORT_DESCRIPTION_LIST "Liste les fichiers pr\202sents sur le disque dur."
	#define STRING_COMMAND_SHORT_DESCRIPTION_RENAME "Renomme un fichier existant."
	#define STRING_COMMAND_SHORT_DESCRIPTION_SIZE "Affiche la taille d'un fichier en octets."
	#define STRING_COMMAND_SHORT_DESCRIPTION_TEXT "Affiche le contenu d'un fichier texte."
	#define STRING_COMMAND_SHORT_DESCRIPTION_U "Rassemble des commandes du monde UNIX."
	#define STRING_COMMAND_SHORT_DESCRIPTION_VERSION "Affiche la version et la date de compilation du syst\212me."
	
	// Commands full description
	#define STRING_COMMAND_FULL_DESCRIPTION_HELP "Appel\202e sans argument, la commande 'help' affiche une liste et une br\212ve\ndescription de toutes les commandes disponibles.\n" \
		"Lorsque le nom d'une commande est sp\202cifi\202 comme argument, 'help' affiche une\ndescription d\202taill\202e des possibilit\202s de la commande."
	#define STRING_COMMAND_FULL_DESCRIPTION_TEXT "Affiche le contenu d'un fichier texte.\n" \
		"La touche 'Echap' permet de quitter le programme.\n" \
		"Il est possible de faire d\202filer le texte \205 l'aide des touches 'Fl\212che haut' et 'Fl\212che bas'."
	#define STRING_COMMAND_FULL_DESCRIPTION_U "Rassemble des versions simplifi\202es de plusieurs commandes UNIX.\n" \
		"Taper 'u' affiche la liste de toutes les commandes disponibles.\n" \
		"Il suffit de taper 'u <Commande>' pour lancer la commande souhait\202e.\n" \
		"Taper 'u <Commande> -h' pour afficher plus de d\202tails sur la commande."
	
#endif

#endif