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
	#define STRING_COMMAND_SHORT_DESCRIPTION_EDIT "Editeur de texte avanc\202."
	#define STRING_COMMAND_SHORT_DESCRIPTION_GAMES "Comporte plusieurs jeux simples et divertissants."
	#define STRING_COMMAND_SHORT_DESCRIPTION_HELP "Fournit de l'aide sur les commandes existantes."
	#define STRING_COMMAND_SHORT_DESCRIPTION_LIST "Liste les fichiers pr\202sents sur le disque dur."
	#define STRING_COMMAND_SHORT_DESCRIPTION_RENAME "Renomme un fichier existant."
	#define STRING_COMMAND_SHORT_DESCRIPTION_SIZE "Affiche la taille d'un fichier en octets."
	#define STRING_COMMAND_SHORT_DESCRIPTION_U "Rassemble des commandes du monde UNIX."
	#define STRING_COMMAND_SHORT_DESCRIPTION_VERSION "Affiche la version et la date de compilation du syst\212me."
	
	// Commands full description
	#define STRING_COMMAND_FULL_DESCRIPTION_EDIT "Permet d'\202diter un fichier texte.\n" \
		"Liste des commandes :\n" \
		"    Fl\212ches : d\202placer le curseur\n" \
		"    Echap : quitter le programme\n" \
		"    Home : aller au d\202but de la ligne\n" \
		"    End : aller \205 la fin de la ligne\n" \
		"    F3 : couper une ou plusieurs lignes\n" \
		"    F4 : coller le texte pr\202c\202demment coup\202\n" \
		"    F5 : sauvegarder le document en cours d'\202dition\n" \
		"    F6 : afficher les statistiques du document"
	#define STRING_COMMAND_FULL_DESCRIPTION_GAMES "Rassemble plusieurs jeux en mode texte.\n" \
		"Les commandes de chaque jeu sont d\202taill\202es lors du lancement, sauf pour\n"\
		"le Jeu de la Vie dont voici les commandes :\n" \
		"    F : acc\202l\202rer la simulation\n" \
		"    S : ralentir la simulation\n" \
		"    N : g\202n\202rer un nouveau monde\n" \
		"    Echap : quitter"
	#define STRING_COMMAND_FULL_DESCRIPTION_HELP "Appel\202e sans argument, la commande 'help' affiche une liste et une br\212ve\ndescription de toutes les commandes disponibles.\n" \
		"Lorsque le nom d'une commande est sp\202cifi\202 comme argument, 'help' affiche une\ndescription d\202taill\202e des possibilit\202s de la commande."
	#define STRING_COMMAND_FULL_DESCRIPTION_U "Rassemble des versions simplifi\202es de plusieurs commandes UNIX.\n" \
		"Taper 'u' affiche la liste de toutes les commandes disponibles.\n" \
		"Il suffit de taper 'u <Commande>' pour lancer la commande souhait\202e.\n" \
		"Taper 'u <Commande> -h' pour afficher plus de d\202tails sur la commande."
	
#endif

#endif