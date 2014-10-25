/** @file Text_Viewer.h
 * A simple text file viewer like the UNIX "less" program.
 * @author Adrien RICCIARDI
 * @version 1.0 : 23/12/2013
 * @version 1.1 : 24/05/2014, ported to Lemon.
 * @version 1.2 : 06/06/2014, optimized code and enabled previous lines displaying.
 */
#ifndef H_TEXT_VIEWER_H
#define H_TEXT_VIEWER_H

//-------------------------------------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------------------------------------
/** Internal buffer size in bytes. */
#define TEXT_VIEWER_BUFFER_SIZE_BYTES (1024 * 1024 * 10)

/** Screen width in characters. */
#define TEXT_VIEWER_SCREEN_WIDTH 80
/** Screen height in characters. */
#define TEXT_VIEWER_SCREEN_HEIGHT 25

// English text translation
#ifdef LANGUAGE_ENGLISH
	#define STRING_ERROR_BAD_PARAMETERS "Error : bad arguments.\nUsage : %s File_Name\n"
	#define STRING_ERROR_FILE_NOT_FOUND "Error : the file '%s' was not found.\n"
	#define STRING_ERROR_FILE_TOO_BIG "Error : the file is too big to be loaded (maximum file size is %u bytes).\n"
	#define STRING_ERROR_READ_FAILED "Error : failed to read from the file.\n"
// French text translation (default one)
#else
	#define STRING_ERROR_BAD_PARAMETERS "Erreur : mauvais param\212tres.\nUtilisation : %s Nom_Fichier\n"
	#define STRING_ERROR_FILE_NOT_FOUND "Erreur : le fichier '%s' est introuvable.\n"
	#define STRING_ERROR_FILE_TOO_BIG "Erreur : le fichier a une taille trop importante (taille maximum accept\202e :  %u octets).\n"
	#define STRING_ERROR_READ_FAILED "Erreur : la lecture du fichier a \202chou\202.\n"
#endif

#endif