/** @file File.h
 * File interface with userspace.
 * @author Adrien RICCIARDI
 */
#ifndef H_FILE_H
#define H_FILE_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Tell if a file exists or not in hard disk's file system.
 * @param String_File_Name Name of the file.
 * @return 1 if the file exists,
 * @return 0 if the file doesn't exist.
 */
int FileExists(char *String_File_Name);

/** Begin a file listing.
 * @warning This function must be called one time only before calling FileListNext();
 */
void FileListInitialize(void);

/** Get list file names.
 * @param String_File_Name A pointer on a buffer which will receive the file name (this buffer must be 13-byte wide).
 * @note The end of the listing is reached when String_File_Name holds an empty string.
 * @warning The file listing must have been initialized by calling FileListInitialize() before any call to this function.
 */
void FileListNext(char *String_File_Name);

/** Rename an existing file.
 * @param String_Current_File_Name The file to rename.
 * @param String_New_File_Name The new name to give to the file.
 * @return ERROR_CODE_NO_ERROR if the file was correctly renamed,
 * @return ERROR_CODE_BAD_FILENAME if New_File_Name is an empty string,
 * @return ERROR_CODE_FILE_NOT_FOUND if the file was not found,
 * @return ERROR_CODE_FILE_ALREADY_EXISTS if the new name is attributed to an existing file.
 */
int FileRename(char *String_Current_File_Name, char *String_New_File_Name);

/** Delete a file from the file system.
 * @param String_File_Name The file to delete.
 * @return ERROR_CODE_NO_ERROR if the file was correctly deleted,
 * @return ERROR_CODE_FILE_NOT_FOUND if the file was not found.
 */
int FileDelete(char *String_File_Name);

/** Retrieve the size of a file in bytes.
 * @param String_File_Name The file to find size.
 * @return 0 if the file was not found,
 * @return a value different from zero is file size.
 */
unsigned int FileSize(char *String_File_Name);

/** Reset all files (without closing them) to prevent a user program from opening all files and never closing them, resulting in an unusable file system. */
void FileResetFileDescriptors(void);

/** Open a file.
 * @param String_File_Name A pointer to an ASCIIZ string containing the file name.
 * @param Opening_Mode Opening mode of the file : 'r' to read only or 'w' to write only.
 * @param File_Descriptor_Index A pointer on an unsigned int which will receive the file descriptor index if the function succeed.
 * @return ERROR_CODE_BAD_FILE_NAME if File_Name is an empty string,
 * @return ERROR_CODE_FILE_OPENED_YET if the file was previously opened but not closed,
 * @return ERROR_CODE_FILE_NOT_FOUND if the file was opened in read only mode and it was not found,
 * @return ERROR_CODE_UNKNOWN_OPENING_MODE if the opening mode is not 'r' or 'w',
 * @return ERROR_CODE_CANT_OPEN_MORE_FILES if the kernel files quota was exceeded,
 * @return ERROR_CODE_FILES_LIST_FULL if the file was opened in write mode an there is no more room in the Files List,
 * @return ERROR_CODE_BLOCKS_LIST_FULL if the file was opened in write mode an there is no more room in the Blocks List,
 * @return ERROR_CODE_NO_ERROR if the file was correctly opened.
 */
int FileOpen(char *String_File_Name, char Opening_Mode, unsigned int *File_Descriptor_Index);

/** Read data from a previously opened file.
 * @param File_Descriptor_Index The file descriptor identifying the file.
 * @param Pointer_Buffer The buffer to store read data to.
 * @param Bytes_Count How many bytes to read. If the supplied number is larger than the file size, less bytes will be read.
 * @param Pointer_Bytes_Read On output, will contain the real number of read bytes.
 * @return ERROR_CODE_NO_ERROR if one or more bytes were successfully read or if 0 byte was requested to read,
 * @return ERROR_CODE_BAD_FILE_DESCRIPTOR if the supplied file descriptor exceeds the maximum number of files that the kernel can open at the same time,
 * @return ERROR_CODE_FILE_NOT_OPENED if the file is not opened,
 * @return ERROR_CODE_BAD_OPENING_MODE if the file is not opened in read mode.
 */
int FileRead(unsigned int File_Descriptor_Index, void *Pointer_Buffer, unsigned int Bytes_Count, unsigned int *Pointer_Bytes_Read);

/** Write data to an opened file.
 * @param File_Descriptor_Index The file descriptor identifying the file.
 * @param Pointer_Buffer The buffer containing the data to write.
 * @param Bytes_Count How many bytes to write.
 * @return ERROR_CODE_NO_ERROR if all bytes were successfully written,
 * @return ERROR_CODE_BAD_FILE_DESCRIPTOR if the supplied file descriptor exceeds the maximum number of files that the kernel can open at the same time,
 * @return ERROR_CODE_FILE_NOT_OPENED if the file is not opened,
 * @return ERROR_CODE_BAD_OPENING_MODE if the file is not opened in write mode,
 * @return ERROR_CODE_BLOCKS_LIST_FULL if there is no more free blocks in the Blocks List and the data could not be written.
 */
int FileWrite(unsigned int File_Descriptor_Index, void *Pointer_Buffer, unsigned int Bytes_Count);

/** Close a file. 
 * @param File_Descriptor_Index Index of the opened file (do nothing if the file was not opened).
 */
void FileClose(unsigned int File_Descriptor_Index);

#endif
