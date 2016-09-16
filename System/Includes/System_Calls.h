/** @file System_Calls.h
 * System calls handlers.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_CALLS_H
#define H_SYSTEM_CALLS_H

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** All the available system parameters. */
typedef enum
{
	SYSTEM_CALL_SYSTEM_PARAMETER_ID_MEMORY_TOTAL_SIZE, //! How many RAM the system can address (in mega bytes).
	SYSTEM_CALL_SYSTEM_PARAMETER_ID_MEMORY_USER_SIZE, //! How many RAM a user program can access (in mega bytes).
	SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_MAXIMUM_OPENED_FILES_COUNT, //! How many files can be opened in the same time.
	SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_BLOCK_SIZE, //! A block size in bytes.
	SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES_COUNT, //! Maximum number of entries in the file system Files List (i.e. the maximum number of files that can be stored in the file system).
	SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_MAXIMUM_BLOCKS_LIST_ENTRIES_COUNT, //! Maximum number of file system blocks.
	SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_FREE_FILES_LIST_ENTRIES_COUNT, //! How many Files List entries are available.
	SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_FREE_BLOCKS_LIST_ENTRIES_COUNT, //! How many Blocks List entries are available.
	SYSTEM_CALL_SYSTEM_PARAMETER_IDS_COUNT //! The total number of parameters.
} TSystemCallSystemParameterID;

/** All the available system calls. */
typedef enum
{
	// System related
	/** Exit from an application and return to the system.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_SYSTEM_EXIT_PROGRAM,

	/** Get a specific system parameter.
	 * @param ebx = The parameter ID (see TSystemCallSystemParameterID).
	 * @param ecx = don't care
	 * @param edx = Pointer on parameter output data (type may change according to requested parameter).
	 * @param esi = don't care
	 * @return 0 if the parameter was successfully retrieved,
	 * @return 1 if the parameter does not exists.
	 */
	SYSTEM_CALL_SYSTEM_GET_PARAMETER,

	// Timer
	/** Read the hardware timer 0 counter.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return The counter value (incremented by one every 50 ms since the start of the system). This value must be casted to unsigned int.
	 */
	SYSTEM_CALL_TIMER_READ_VALUE,

	/** Pause system and running application for the specified periods of 50 ms.
	 * @param ebx = How many periods of 50 ms to wait.
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_TIMER_WAIT,

	// UART
	/** Initialize the Universal Asynchronous Receiver Transmitter (serial port) 0.
	 * @param ebx = Parity mode.
	 * @param ecx = Baud rate in bit/s.
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return ERROR_CODE_NO_ERROR if the UART was correctly initialized,
	 * @return ERROR_CODE_BAD_UART_PARAMETERS if bad parameters were provided.
	 * @see Driver_UART.h for precisions about parity mode and baud rate constants.
	 */
	SYSTEM_CALL_UART_INITIALIZE,

	/** Read a byte from the UART.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return The read byte.
	 * @warning This function is blocking.
	 */
	SYSTEM_CALL_UART_READ_BYTE,

	/** Write a byte to the UART.
	 * @param ebx = The byte to write.
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_UART_WRITE_BYTE,

	/** Check if the UART received a byte.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return 1 if a byte was received or 0 if not.
	 */
	SYSTEM_CALL_UART_IS_DATA_RECEIVED,

	// Screen
	/** Clear the kernel console screen.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_SCREEN_CLEAR,

	/** Write a character to the kernel console screen.
	 * @param ebx = ASCII code of the character.
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_SCREEN_WRITE_CHARACTER,

	/** Write an ASCIIZ string of characters to the kernel console screen.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = Pointer on the string.
	 * @param esi = don't care
	 * @return Nothing.
	 * @note The terminating zero character is ignored.
	 */
	SYSTEM_CALL_SCREEN_WRITE_STRING,

	/** Get the current screen color.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return The console color.
	 */
	SYSTEM_CALL_SCREEN_GET_COLOR,

	/** Set the screen color.
	 * @param ebx = Color code.
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_SCREEN_SET_COLOR,

	/** Get the cursor position.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = X pointer.
	 * @param esi = Y pointer.
	 * @return Nothing.
	 */
	SYSTEM_CALL_SCREEN_GET_CURSOR_POSITION,

	/** Set the cursor position. Do nothing if the location is out of range.
	 * @param ebx = Row value.
	 * @param ecx = Column value.
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_SCREEN_SET_CURSOR_POSITION,

	/** Display a buffer representing the raw video memory data on the screen.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = Buffer pointer.
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_SCREEN_DISPLAY_BUFFER,

	// Keyboard
	/** Wait until the user hits a key and return the corresponding character code.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return ASCII or special key code.
	 */
	SYSTEM_CALL_KEYBOARD_READ_CHARACTER,

	/** Tell if a key is available in the keyboard buffer or not.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return 0 if no key is available or 1 if a key is present.
	 */
	SYSTEM_CALL_KEYBOARD_IS_KEY_AVAILABLE,

	/** Wait for an ethernet frame to be received.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = On output, contain the frame size in bytes.
	 * @param esi = Output buffer address.
	 */
	SYSTEM_CALL_ETHERNET_RECEIVE_PACKET,

	/** Send an ethernet frame.
	 * @param ebx = Frame length in bytes.
	 * @param ecx = don't care
	 * @param edx = Buffer address.
	 * @param esi = don't care
	 */
	SYSTEM_CALL_ETHERNET_SEND_PACKET,

	// File system
	/** Check if a file exists or not in the file system.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return 0 if the file was not found,
	 * @return 1 if the file exists.
	 */
	SYSTEM_CALL_FILE_EXISTS,

	/** Start a file listing.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = Pointer to a string containing the file name.
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_FILE_LIST_INITIALIZE,

	/** Get the next file name in a file listing.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = Pointer on a FILE_NAME_LENGTH + 1 wide string which will receive the file name.
	 * @param esi = don't care
	 * @return Nothing.
	 * @note The listing is finished when the string becomes empty.
	 */
	SYSTEM_CALL_FILE_LIST_NEXT,

	/** Get the size in bytes of a file.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = Pointer to a string containing the file name.
	 * @param esi = don't care
	 * @return File's size in bytes.
	 */
	SYSTEM_CALL_FILE_SIZE,

	/** Delete a file from the file system.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = Pointer to a string containing the file name.
	 * @param esi = don't care
	 * @return ERROR_CODE_NO_ERROR if the file was successfully deleted,
	 * @return ERROR_CODE_BAD_FILENAME if the file name's string was empty,
	 * @return ERROR_CODE_FILE_NOT_FOUND if the file to delete was not found.
	 */
	SYSTEM_CALL_FILE_DELETE,

	/** Rename an existing file.
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = Pointer to a string containing the current file name.
	 * @param esi = Pointer to a string containing the new file name.
	 * @return ERROR_CODE_NO_ERROR if the file was correctly renamed,
	 * @return ERROR_CODE_BAD_FILENAME if Current_File_Name or New_File_Name is an empty string,
	 * @return ERROR_CODE_FILE_NOT_FOUND if the file was not found,
	 * @return ERROR_CODE_FILE_ALREADY_EXISTS if the new name is attributed to an existing file.
	 */
	SYSTEM_CALL_FILE_RENAME,

	/** Open a file.
	 * @param ebx = A byte representing the opening mode ('r' or 'w').
	 * @param ecx = don't care
	 * @param edx = Pointer to a string holding the file name.
	 * @param esi = Pointer on a double word (32 bits) in which file descriptor's value will be stored.
	 * @return ERROR_CODE_NO_ERROR if the file was correctly opened,
	 * @return ERROR_CODE_BAD_FILENAME if File_Name is an empty string,
	 * @return ERROR_CODE_FILE_NOT_FOUND if the file was not found,
	 * @return ERROR_CODE_FILE_OPENED_YET if the file was previously opened by not closed,
	 * @return ERROR_CODE_FILES_LIST_FULL if the file was opened in write mode and there was no more room in Files List,
	 * @return ERROR_CODE_BLOCKS_LIST_FULL if the file was opened in write mode and there was no more room in Blocks List,
	 * @return ERROR_CODE_UNKNOWN_OPENING_MODE if the opening mode byte is not 'r' or 'w',
	 * @return ERROR_CODE_CANT_OPEN_MORE_FILES if there is no more free file descriptor.
	 */
	SYSTEM_CALL_FILE_OPEN,

	/** Read data from an opened file.
	 * @param ebx = File descriptor index.
	 * @param ecx = How many bytes to read.
	 * @param edx = Pointer to a buffer which will be filled with file data. The buffer must have enough room to fit data.
	 * @param esi = Pointer to an unsigned integer which will contain the real number of read bytes.
	 * @return ERROR_CODE_NO_ERROR if one or more bytes were successfully read or if 0 byte was requested to read,
	 * @return ERROR_CODE_BAD_FILE_DESCRIPTOR if the supplied file descriptor exceeds the maximum number of files that the kernel can open at the same time,
	 * @return ERROR_CODE_FILE_NOT_OPENED if the file is not opened,
	 * @return ERROR_CODE_BAD_OPENING_MODE if the file is not opened in read mode.
	 */
	SYSTEM_CALL_FILE_READ,

	/** Write data to an opened file.
	 * @param ebx = File descriptor index.
	 * @param ecx = How many bytes to write.
	 * @param edx = Pointer to a buffer which holds the data to write.
	 * @param esi = don't care
	 * @return ERROR_CODE_NO_ERROR if all bytes were successfully written,
	 * @return ERROR_CODE_BAD_FILE_DESCRIPTOR if the supplied file descriptor exceeds the maximum number of files that the kernel can open at the same time,
	 * @return ERROR_CODE_FILE_NOT_OPENED if the file is not opened,
	 * @return ERROR_CODE_BAD_OPENING_MODE if the file is not opened in write mode,
	 * @return ERROR_CODE_BLOCKS_LIST_FULL if there is no more free blocks in the Blocks List and the data could not be written.
	 */
	SYSTEM_CALL_FILE_WRITE,

	/** Close a previously opened file. If the file was not opened, do nothing.
	 * @param ebx = File descriptor index.
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_FILE_CLOSE,

	/** How many system calls are available. */
	SYSTEM_CALLS_COUNT
} TSystemCall;

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** System calls interrupt handler.
 * @return The system call specific value.
 * @note System calls parameters are stored in the processor registers.\n
 * Register eax is the system call number.\n
 * Register ebx is the first integer parameter.\n
 * Register ecx is the second integer parameter.\n
 * Register edx is the first pointer parameter.\n
 * Register esi is the second pointer parameter.\n
 * System calls are called with interrupt 60h.
 */
int SystemCalls(void);

#endif
