/** @file Error_Codes.h
 * Contain all the system known error codes.
 * @author Adrien RICCIARDI
 * @version 1.0 : 11/01/2015
 */
#ifndef H_ERROR_CODES_H
#define H_ERROR_CODES_H

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** All the system known errors. */
typedef enum
{
	ERROR_CODE_NO_ERROR, //! No error happened.
	ERROR_CODE_FILES_LIST_FULL, //! The Files List is full.
	ERROR_CODE_BLOCKS_LIST_FULL, //! There is no more free block in the Blocks List.
	ERROR_CODE_BAD_FILE_NAME, //! The provided file name is not valid.
	ERROR_CODE_FILE_NOT_FOUND, //! The requested file doesn't exist in the file system.
	ERROR_CODE_FILE_ALREADY_EXISTS, //! The file is already existing.
	ERROR_CODE_CANT_OPEN_MORE_FILES, //! The maximum opened files count is reached.
	ERROR_CODE_FILE_OPENED_YET, //! The file is opened yet.
	ERROR_CODE_UNKNOWN_OPENING_MODE, //! A file was opened with an invalid mode.
	ERROR_CODE_FILE_NOT_OPENED, //! An attempt to read or write a file which was not previously opened was done.
	ERROR_CODE_BAD_OPENING_MODE, //! A file opened in read mode is accessed in write mode or a file opened in write mode is accessed in read mode.
	ERROR_CODE_BAD_FILE_DESCRIPTOR, //! A bad file descriptor was provided to a file function.
	ERROR_CODE_BAD_UART_PARAMETERS, //! Bad parameters were provided to UART during initialization.
	ERROR_CODE_FILE_LARGER_THAN_RAM, //! There is not enough room in RAM to load the file.
	ERROR_CODE_FILE_NOT_EXECUTABLE //! The file is not an executable program.
} TErrorCode;

#endif