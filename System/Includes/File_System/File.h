/** @file File.h
 * File interface with userspace.
 * @author Adrien RICCIARDI
 * @version 1.0 : 04/04/2013
 */
#ifndef H_FILE_H
#define H_FILE_H

//-------------------------------------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Create a new file in one operation.
 * @param File_Name Name of the file. If the files already exists it is overwritten by the new one.
 * @param Buffer A pointer to a buffer holding the whole file content.
 * @param Buffer_Size_Bytes Size of the buffer in bytes.
 * @return ERROR_CODE_BAD_FILE_NAME if the file name's string is empty,
 * @return ERROR_CODE_BAT_FULL if there is no more room in BAT,
 * @return ERROR_CODE_FL_FULL if there is no more room in FL,
 * @return ERROR_CODE_NO_ERROR if the file was successfully created.
 * @note If the file already exists and the new file is bigger than the previous and there is not enough room on disk, the old file version is preserved.
 */
int FileCreate(char *File_Name, unsigned char *Buffer, unsigned int Buffer_Size_Bytes);

/** Tell if a file exists or not in hard disk's file system.
 * @param File_Name Name of the file.
 * @return 1 if the file exists,
 * @return 0 if the file doesn't exist.
 */
int FileExists(char *File_Name);

/** Begin a file listing.
 * @warning This function must be called one time only before calling FileListNext();
 */
void FileListInitialize(void);

/** Get list file names.
 * @param File_Name A pointer on a buffer which will receive the file name (this buffer must be 13-byte wide).
 * @note The end of the listing is reached when File_Name holds an empty string.
 * @warning The file listing must have been initialized by calling FileListInitialize() before any call to this function.
 */
void FileListNext(char *File_Name);

/** Rename an existing file.
 * @return ERROR_CODE_NO_ERROR if the file was correctly renamed,
 * @return ERROR_CODE_BAD_FILENAME if New_File_Name is an empty string,
 * @return ERROR_CODE_FILE_NOT_FOUND if the file was not found,
 * @return ERROR_CODE_FILE_ALREADY_EXISTS if the new name is attributed to an existing file.
 */
int FileRename(char *Current_File_Name, char *New_File_Name);

/** Delete a file from the file system.
 * @return ERROR_CODE_NO_ERROR if the file was correctly deleted,
 * @return ERROR_CODE_FILE_NOT_FOUND if the file was not found.
 */
int FileDelete(char *File_Name);

/** Retrieve the size of a file in bytes.
 * @return 0 if the file was not found,
 * @return a value different from zero is file size.
 */
unsigned int FileSize(char *File_Name);

/** Load a whole file into memory.
 * @param String_File_Name The name of the file to load.
 * @param Buffer The buffer where to store the loaded file.
 * @param Is_Executable_Check_Enabled Set to true to stop the function if the file to load is not a program. Set to false to load all file types.
 * @return ERROR_CODE_NO_ERROR if the file was correctly loaded,
 * @return ERROR_CODE_FILE_NOT_FOUND if the file was not found,
 * @return ERROR_CODE_FILE_NOT_EXECUTABLE if the executable magic number check is enabled and the file is not a program,
 * @return ERROR_CODE_FILE_LARGER_THAN_RAM if the file size is greater than the available userspace RAM.
 */
int FileLoad(char *String_File_Name, unsigned char *Buffer, int Is_Executable_Check_Enabled);

/** Reset all files (without closing them) to prevent a user program from opening all files and never closing them, resulting in an unusable file system. */
void FileResetFileDescriptors(void);

/** Open a file.
 * @param File_Name A pointer to an ASCIIZ string containing the file name.
 * @param Opening_Mode Opening mode of the file : 'r' to read only or 'w' to write only.
 * @param File_Descriptor_Index A pointer on an unsigned int which will receive the file descriptor index if the function succeed.
 * @return ERROR_CODE_BAD_FILE_NAME if File_Name is an empty string,
 * @return ERROR_CODE_FILE_OPENED_YET if the file was previously opened but not closed,
 * @return ERROR_CODE_FILE_NOT_FOUND if the file was opened in read only mode and it was not found,
 * @return ERROR_CODE_UNKNOWN_OPENING_MODE if the opening mode is not 'r' or 'w',
 * @return ERROR_CODE_CANT_OPEN_MORE_FILES if the kernel files quota was exceeded,
 * @return ERROR_CODE_FL_FULL if the file was opened in write mode an there is no more room in FL,
 * @return ERROR_CODE_BAT_FULL if the file was opened in write mode an there is no more room in BAT,
 * @return ERROR_CODE_NO_ERROR if the file was correctly opened.
 */
int FileOpen(char *File_Name, char Opening_Mode, unsigned int *File_Descriptor_Index);

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
int FileRead(unsigned int File_Descriptor_Index, unsigned char *Pointer_Buffer, unsigned int Bytes_Count, unsigned int *Pointer_Bytes_Read);

/** Write data to an opened file.
 * @param File_Descriptor_Index The file descriptor identifying the file.
 * @param Pointer_Buffer The buffer containing the data to write.
 * @param Bytes_Count How many bytes to write.
 * @return ERROR_CODE_NO_ERROR if all bytes were successfully written,
 * @return ERROR_CODE_BAD_FILE_DESCRIPTOR if the supplied file descriptor exceeds the maximum number of files that the kernel can open at the same time,
 * @return ERROR_CODE_FILE_NOT_OPENED if the file is not opened,
 * @return ERROR_CODE_BAD_OPENING_MODE if the file is not opened in write mode,
 * @return ERROR_CODE_BAT_FULL if there no more free blocks in BAT and the data could not be written.
 */
int FileWrite(unsigned int File_Descriptor_Index, unsigned char *Pointer_Buffer, unsigned int Bytes_Count);

/** Close a file. 
 * @param File_Descriptor_Index Index of the opened file (do nothing if the file was not opened).
 */
void FileClose(unsigned int File_Descriptor_Index);

#endif