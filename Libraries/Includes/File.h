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

/** How many files can be opened in the same time. */
#define FILE_MAXIMUM_OPENED_COUNT 8

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** The allowed file opening modes. */
typedef enum
{
	FILE_OPENING_MODE_READ = 'r',
	FILE_OPENING_MODE_WRITE = 'w'
} TFileOpeningMode;

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
 * @return The file size in bytes if the file exists.
 * @return 0 if the file doesn't exist.
 */
unsigned int FileGetSize(char *String_File_Name);

/** Start a file listing. */
void FileListBegin(void);

/** Get the next file name for file listing.
 * @param String_File_Name Pointer on a FILE_NAME_LENGTH + 1 wide string which will receive the file name.
 */
void FileListNext(char *String_File_Name);

/** Open a file for reading or writing.
 * @param String_File_Name The file path. The file must exist for reading mode but can be non existent for writing mode.
 * @param Opening_Mode Select reading or writing mode.
 * @param Pointer_File_ID On output and if the call was successful, contain the opened file identifier to provide to other file functions.
 * @return ERROR_CODE_BAD_FILE_NAME if the provided file name is an empty string,
 * @return ERROR_CODE_FILE_OPENED_YET if the file was previously opened but not closed,
 * @return ERROR_CODE_FILE_NOT_FOUND if the file was opened in read only mode and it was not found,
 * @return ERROR_CODE_UNKNOWN_OPENING_MODE if the opening mode is not "read" or "write",
 * @return ERROR_CODE_CANT_OPEN_MORE_FILES if the kernel files quota was exceeded,
 * @return ERROR_CODE_FL_FULL if the file was opened in write mode an there is no more room in FL,
 * @return ERROR_CODE_BAT_FULL if the file was opened in write mode an there is no more room in BAT,
 * @return ERROR_CODE_NO_ERROR if the file was correctly opened.
 */
int FileOpen(char *String_File_Name, TFileOpeningMode Opening_Mode, unsigned int *Pointer_File_ID);

/** Read data from a previously opened file.
 * @param File_ID The file identifier.
 * @param Pointer_Buffer The buffer to store read data to.
 * @param Bytes_Count How many bytes to read. If the supplied number is larger than the file size, less bytes will be read.
 * @param Pointer_Bytes_Read On output, will contain the real number of read bytes.
 * @return ERROR_CODE_NO_ERROR if one or more bytes were successfully read or if 0 byte was requested to read,
 * @return ERROR_CODE_BAD_FILE_DESCRIPTOR if the supplied file descriptor exceeds the maximum number of files that the kernel can open at the same time,
 * @return ERROR_CODE_FILE_NOT_OPENED if the file is not opened,
 * @return ERROR_CODE_BAD_OPENING_MODE if the file is not opened in read mode.
 */
int FileRead(unsigned int File_ID, void *Pointer_Buffer, unsigned int Bytes_Count, unsigned int *Pointer_Bytes_Read);

/** Write data to an opened file.
 * @param File_ID The file identifier.
 * @param Pointer_Buffer The buffer containing the data to write.
 * @param Bytes_Count How many bytes to write.
 * @return ERROR_CODE_NO_ERROR if all bytes were successfully written,
 * @return ERROR_CODE_BAD_FILE_DESCRIPTOR if the supplied file descriptor exceeds the maximum number of files that the kernel can open at the same time,
 * @return ERROR_CODE_FILE_NOT_OPENED if the file is not opened,
 * @return ERROR_CODE_BAD_OPENING_MODE if the file is not opened in write mode,
 * @return ERROR_CODE_BAT_FULL if there no more free blocks in BAT and the data could not be written.
 */
int FileWrite(unsigned int File_ID, void *Pointer_Buffer, unsigned int Bytes_Count);

/** Close a previously opened file (do nothing if the file was not opened).
 * @param File_ID Identifier of the opened file.
 */
void FileClose(unsigned int File_ID);

/** Delete an existing file.
 * @param String_File_Name The file to delete.
 * @return ERROR_CODE_NO_ERROR if the file has been successfully deleted,
 * @return ERROR_CODE_FILE_NOT_FOUND if the file is not existing.
 */
int FileDelete(char *String_File_Name);

// TODO
/** Rename an existing file.
 * @param Current_File_Name The current file name.
 * @param New_File_Name The new name used to rename the file.
 * @return ERROR_CODE_NO_ERROR if the file was correctly renamed,
 * @return ERROR_CODE_BAD_FILENAME if Current_File_Name or New_File_Name is an empty string,
 * @return ERROR_CODE_FILE_NOT_FOUND if the file was not found,
 * @return ERROR_CODE_FILE_ALREADY_EXISTS if the new name is attributed to an existing file.
 */
//int FileRename(char *Current_File_Name, char *New_File_Name); // TODO à refaire

// FileCopy (avec calcul espace restant disque avant copie)


#endif
