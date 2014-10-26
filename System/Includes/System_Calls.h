/** @file System_Calls.h
 * System calls handlers.
 * @author Adrien RICCIARDI
 * @version 1.0 : 21/04/2012
 * @version 1.1 : 12/09/2012
 * @version 1.2 : 13/12/2013, made system calls names clearer, reorganized them and optimized code.
 * @version 1.3 : 07/06/2014, used an enum to list the system calls (and removed never used system calls groups).
 */
#ifndef H_SYSTEM_CALLS_H
#define H_SYSTEM_CALLS_H

//-------------------------------------------------------------------------------------------------
// Types
//-------------------------------------------------------------------------------------------------
/** All the available system calls. */
typedef enum
{
	// System related
	/** Exit from an application and return to the system.
	 * @param eax = 0
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_SYSTEM_EXIT_PROGRAM,

	/** Get the amount of RAM memory available for user.
	 * To find out the total RAM installed on the system add 1 MB (1048576) to the result of this system call.
	 * @param eax = 1
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Amount of memory available for user in bytes.
	 */
	SYSTEM_CALL_SYSTEM_GET_USER_MEMORY_SIZE,

	// Timer
	/** Read the hardware timer 0 counter.
	 * @param eax = 2
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return The counter value (incremented by one every 50 ms since the start of the system).
	 */
	SYSTEM_CALL_TIMER_READ_VALUE,

	/** Pause system and running application for the specified periods of 50 ms.
	 * @param eax = 3
	 * @param ebx = How many periods of 50 ms to wait.
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_TIMER_WAIT,

	// UART
	/** Initialize the Universal Asynchronous Receiver Transmitter (serial port) 0.
	 * @param eax = 4
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
	 * @param eax = 5
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return The read byte.
	 * @warning This function is blocking.
	 */
	SYSTEM_CALL_UART_READ_BYTE,

	/** Write a byte to the UART.
	 * @param eax = 6
	 * @param ebx = The byte to write.
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_UART_WRITE_BYTE,

	/** Check if the UART received a byte.
	 * @param eax = 7
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return 1 if a byte was received or 0 if not.
	 */
	SYSTEM_CALL_UART_IS_DATA_RECEIVED,

	// Screen
	/** Clear the kernel console screen.
	 * @param eax = 0x10
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_SCREEN_CLEAR,

	/** Write a character to the kernel console screen.
	 * @param eax = 0x11
	 * @param ebx = ASCII code of the character.
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_SCREEN_WRITE_CHARACTER,

	/** Write an ASCIIZ string of characters to the kernel console screen.
	 * @param eax = 0x12
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = Pointer on the string.
	 * @param esi = don't care
	 * @return Nothing.
	 * @note The terminating zero character is ignored.
	 */
	SYSTEM_CALL_SCREEN_WRITE_STRING,

	/** Get the current screen color.
	 * @param eax = 0x13
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return The console color.
	 */
	SYSTEM_CALL_SCREEN_GET_COLOR,

	/** Set the screen color.
 	 * @param eax = 0x14
	 * @param ebx = Color code.
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_SCREEN_SET_COLOR,

	/** Get the cursor position.
	 * @param eax = 0x15
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = X pointer.
	 * @param esi = Y pointer.
	 * @return Nothing.
	 */
	SYSTEM_CALL_SCREEN_GET_CURSOR_POSITION,

	/** Set the cursor position. Do nothing if the location is out of range.
	 * @param eax = 0x16
	 * @param ebx = Row value.
	 * @param ecx = Column value.
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_SCREEN_SET_CURSOR_POSITION,

	/** Display a buffer representing the raw video memory data on the screen.
	 * @param eax = 0x17
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = Buffer pointer.
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_SCREEN_DISPLAY_BUFFER,

	// Keyboard
	/** Wait until the user hits a key and return the corresponding character code.
	 * @param eax = 0x20
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return ASCII or special key code.
	 */
	SYSTEM_CALL_KEYBOARD_READ_CHARACTER,

	/** Tell if a key is available in the keyboard buffer or not.
	 * @param eax = 0x21
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return 0 if no key is available or 1 if a key is present.
	 */
	SYSTEM_CALL_KEYBOARD_IS_KEY_AVAILABLE,

	// File system
	/** Check if a file exists or not in the file system.
	 * @param eax = 0x30
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = don't care
	 * @param esi = don't care
	 * @return 0 if the file was not found,
	 * @return 1 if the file exists.
	 */
	SYSTEM_CALL_FILE_EXISTS,

	/** Start a file listing.
	 * @param eax = 0x31
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = Pointer to a string containing the file name.
	 * @param esi = don't care
	 * @return Nothing.
	 */
	SYSTEM_CALL_FILE_LIST_INITIALIZE,

	/** Get the next file name in a file listing.
	 * @param eax = 0x32
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = Pointer on a FILE_NAME_LENGTH + 1 wide string which will receive the file name.
	 * @param esi = don't care
	 * @return Nothing.
	 * @note The listing is finished when the string becomes empty.
	 */
	SYSTEM_CALL_FILE_LIST_NEXT,

	/** Get the size in bytes of a file.
	 * @param eax = 0x33
	 * @param ebx = don't care
	 * @param ecx = don't care
	 * @param edx = Pointer to a string containing the file name.
	 * @param esi = don't care
	 * @return File's size in bytes.
	 */
	SYSTEM_CALL_FILE_SIZE,

	/** Delete a file from the file system.
	 * @param eax = 0x34
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
	 * @param eax = 0x34
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
	 * @param eax = 0x36
	 * @param ebx = A byte representing the opening mode ('r' or 'w').
	 * @param ecx = don't care
	 * @param edx = Pointer to a string holding the file name.
	 * @param esi = Pointer on a double word (32 bits) in which file descriptor's value will be stored.
	 * @return ERROR_CODE_NO_ERROR if the file was correctly opened,
	 * @return ERROR_CODE_BAD_FILENAME if File_Name is an empty string,
	 * @return ERROR_CODE_FILE_NOT_FOUND if the file was not found,
	 * @return ERROR_CODE_FILE_OPENED_YET if the file was previously opened by not closed,
	 * @return ERROR_CODE_FL_FULL if the file was opened in write mode and there was no more room in FL,
	 * @return ERROR_CODE_BAT_FULL if the file was opened in write mode and there was no more room in BAT,
	 * @return ERROR_CODE_UNKNOWN_OPENING_MODE if the opening mode byte is not 'r' or 'w',
	 * @return ERROR_CODE_CANT_OPEN_MORE_FILES if there is no more free file descriptor.
	 */
	SYSTEM_CALL_FILE_OPEN,

	/** Read data from an opened file.
	 * @param eax = 0x37
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

	/** Write a whole to an opened file.
	 * @param eax = 0x38
	 * @param ebx = File descriptor index.
	 * @param ecx = How many bytes to write.
	 * @param edx = Pointer to a buffer which holds the data to write.
	 * @param esi = don't care
	 * @return ERROR_CODE_NO_ERROR if all bytes were successfully written,
	 * @return ERROR_CODE_BAD_FILE_DESCRIPTOR if the supplied file descriptor exceeds the maximum number of files that the kernel can open at the same time,
	 * @return ERROR_CODE_FILE_NOT_OPENED if the file is not opened,
	 * @return ERROR_CODE_BAD_OPENING_MODE if the file is not opened in write mode,
	 * @return ERROR_CODE_BAT_FULL if there no more free blocks in BAT and the data could not be written.
	 */
	SYSTEM_CALL_FILE_WRITE,

	/** Close a previously opened file. If the file was not opened, do nothing.
	 * @param eax = 0x39
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
