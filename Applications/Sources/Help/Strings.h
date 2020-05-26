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
#ifdef CONFIGURATION_GLOBAL_LANGUAGE_ENGLISH
	#error "TODO"
// French strings
#else
	#define STRING_USAGE_1 "Utilisation : "
	#define STRING_USAGE_2 " Commande\n" \
		"\n" \
		"Tapez '"
	#define STRING_USAGE_3 "help System"
	#define STRING_USAGE_4 "' pour plus d'informations sur le fonctionnement du syst\212me.\n\n"
	
	#define STRING_AVAILABLE_COMMANDS "Liste des commandes disponibles :\n"
	
	#define STRING_UNKNOWN_COMMAND_1 "La commande '"
	#define STRING_UNKNOWN_COMMAND_2 "' n'existe pas.\n" 
	
	// Commands short description
	#define STRING_COMMAND_SHORT_DESCRIPTION_CHAT "Messagerie instantan\202e bas\202e sur le protocole UDP."
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
	#define STRING_COMMAND_SHORT_DESCRIPTION_SYSTEM "Description du syst\212me lui-m\210me."
	#define STRING_COMMAND_SHORT_DESCRIPTION_U "Rassemble des commandes du monde UNIX."
	#define STRING_COMMAND_SHORT_DESCRIPTION_VERSION "Affiche la version et la date de compilation du syst\212me."
	
	// Commands full description
	#define STRING_COMMAND_FULL_DESCRIPTION_CHAT "Le message en cours de r\202daction se trouve tout en haut de l'\202cran.\n" \
		"Le message est envoy\202 d\212s que la touche Entr\202e est press\202e. Le message\n" \
		"dispara\214t alors de la zone d'\202criture et appara\214t dans la zone d'affichage.\n" \
		"Les messages re\207us sont affich\202s avec une couleur diff\202rente de ceux envoy\202s. \n" \
		"Appuyer sur Echap pour quitter instantan\202ment le chat."
	#define STRING_COMMAND_FULL_DESCRIPTION_EDIT "Permet d'\202diter un fichier texte.\n" \
		"Liste des commandes :\n" \
		"    Fl\212ches : d\202placer le curseur\n" \
		"    Echap : quitter le programme\n" \
		"    Home : aller au d\202but de la ligne\n" \
		"    End : aller \205 la fin de la ligne\n" \
		"    Ctrl+X : couper une ou plusieurs lignes\n" \
		"    Ctrl+V : coller le texte pr\202c\202demment coup\202\n" \
		"    Ctrl+S : sauvegarder le document en cours d'\202dition\n" \
		"    F1 : afficher les statistiques du document"
	#define STRING_COMMAND_FULL_DESCRIPTION_GAMES "Rassemble plusieurs jeux en mode texte.\n" \
		"Les commandes de chaque jeu sont d\202taill\202es lors du lancement, sauf pour\n"\
		"le Jeu de la Vie dont voici les commandes :\n" \
		"    F : acc\202l\202rer la simulation\n" \
		"    S : ralentir la simulation\n" \
		"    N : g\202n\202rer un nouveau monde\n" \
		"    Echap : quitter"
	#define STRING_COMMAND_FULL_DESCRIPTION_HELP "Appel\202e sans argument, la commande 'help' affiche une liste et une br\212ve\ndescription de toutes les commandes disponibles.\n" \
		"Lorsque le nom d'une commande est sp\202cifi\202 comme argument, 'help' affiche une\ndescription d\202taill\202e des possibilit\202s de la commande."
	#define STRING_COMMAND_FULL_DESCRIPTION_SYSTEM "Voici quelques informations sur Lemon :\n" \
		"- Appuyez sur la touche F12 \205 tout moment pour quitter le programme en cours et\n  revenir au syst\212me.\n" \
		"- Un programme portant le nom 'Autostart' sera automatiquement lanc\202 au\n  d\202marrage du syst\212me.\n" \
		"- Le syst\212me de fichiers est sensible \205 la casse des caract\212res.\n" \
		"- Appuyer sur la touche 'fl\212che haut' dans la console du syst\212me pour afficher\n  la derni\212re commande tap\202e.\n" \
		"- Appuyer sur la touche 'fl\212che bas' dans la console du syst\212me pour effacer le\n  texte saisi dans l'invite de commande."
	#define STRING_COMMAND_FULL_DESCRIPTION_U "Rassemble des versions simplifi\202es de plusieurs commandes UNIX.\n" \
		"Taper 'u' affiche la liste de toutes les commandes disponibles.\n" \
		"Il suffit de taper 'u <Commande>' pour lancer la commande souhait\202e.\n" \
		"Taper 'u <Commande> -h' pour afficher plus de d\202tails sur la commande."
	
#endif

#endif
