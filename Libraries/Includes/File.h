/** @file File.h
 * @author Adrien RICCIARDI
 * @version 1.0 : 27/04/2012
 * @version 1.1 : 14/09/2012
 */
#ifndef H_FILE_H
#define H_FILE_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Length of a file name string (it does not include the terminating zero). */
#define FILE_NAME_LENGTH 12

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Indicate if a file exists in the file system.
 * @param String_File_Name Name of the file.
 * @return 0 (false) if the file doesn't exist.
 * @return A non-zero value if the file exists.
 */
int FileExists(char *String_File_Name);

/** Tell the size of a file.
 * @param String_File_Name Name of the file.
 * @return The file size in 512-byte sectors if the file exists.
 * @return 0 if the file doesn't exist.
 */
unsigned int FileSize(char *String_File_Name);

/** Start a file listing. */
void FileListBegin(void);

/** Get the next file name for file listing.
 * @param String_File_Name Pointer on a FILE_NAME_LENGTH + 1 wide string which will receive the file name.
 */
void FileListNext(char *String_File_Name);

// TODO
/** Rename an existing file.
 * @param Current_File_Name The current file name.
 * @param New_File_Name The new name used to rename the file.
 * @return ERROR_CODE_NO_ERROR if the file was correctly renamed,
 * @return ERROR_CODE_BAD_FILENAME if Current_File_Name or New_File_Name is an empty string,
 * @return ERROR_CODE_FILE_NOT_FOUND if the file was not found,
 * @return ERROR_CODE_FILE_ALREADY_EXISTS if the new name is attributed to an existing file.
 */
int FileRename(char *Current_File_Name, char *New_File_Name); // TODO à refaire

// FileDelete
// FileCopy (avec calcul espace restant disque avant copie)
// FileOpen
// FileRead
// FileWrite
// FileClose


#endif
