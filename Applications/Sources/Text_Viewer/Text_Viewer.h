/** @file Text_Viewer.h
 * A simple text file viewer like the UNIX "less" program.
 * @author Adrien RICCIARDI
 * @version 1.0 : 23/12/2013
 * @version 1.1 : 24/05/2014, ported to Lemon.
 * @version 1.2 : 06/06/2014, optimized code and enabled previous lines displaying.
 */
#ifndef H_TEXT_VIEWER_H
#define H_TEXT_VIEWER_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Internal buffer size in bytes. */
#define TEXT_VIEWER_BUFFER_SIZE_BYTES (1024 * 1024 * 10)

/** Screen width in characters. */
#define TEXT_VIEWER_SCREEN_WIDTH 80
/** Screen height in characters. */
#define TEXT_VIEWER_SCREEN_HEIGHT 25

// English text translation
#ifdef LANGUAGE_ENGLISH
	#define STRING_USAGE_1 "Error : bad arguments.\nUsage : "
	#define STRING_USAGE_2 " File_Name\n"
	#define STRING_ERROR_FILE_NOT_FOUND_1 "Error : the file '"
	#define STRING_ERROR_FILE_NOT_FOUND_2 "' was not found.\n"
	#define STRING_ERROR_FILE_TOO_BIG_1 "Error : the file is too big to be loaded (maximum file size is "
	#define STRING_ERROR_FILE_TOO_BIG_2 " bytes).\n"
	#define STRING_ERROR_READ_FAILED "Error : failed to read from the file.\n"
// French text translation (default one)
#else
	#define STRING_USAGE_1 "Erreur : mauvais param\212tres.\nUtilisation : "
	#define STRING_USAGE_2 " Nom_Fichier\n"
	#define STRING_ERROR_FILE_NOT_FOUND_1 "Erreur : le fichier '"
	#define STRING_ERROR_FILE_NOT_FOUND_2 "' est introuvable.\n"
	#define STRING_ERROR_FILE_TOO_BIG_1 "Erreur : le fichier a une taille trop importante (taille maximum accept\202e : "
	#define STRING_ERROR_FILE_TOO_BIG_2 " octets).\n"
	#define STRING_ERROR_READ_FAILED "Erreur : la lecture du fichier a \202chou\202.\n"
#endif

#endif