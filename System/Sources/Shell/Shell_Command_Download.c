/** @file Shell_Command_Download.c
 * Download a file from the UART and save it to disk.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h> // Needed by CONFIGURATION_FILE_NAME_LENGTH
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_Screen.h>
#include <Drivers/Driver_UART.h>
#include <Error_Codes.h>
#include <File_System/File.h>
#include <File_System/File_System.h>
#include <Shell.h>
#include <Standard_Functions.h> // Needed by itoa()
#include <Strings.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Value to send to indicate that the shell is ready to download a file. */
#define SHELL_COMMAND_DOWNLOAD_PROTOCOL_COMMAND_REQUEST_DOWNLOAD 'R'
/** Value received when the server starts to send the file. */
#define SHELL_COMMAND_DOWNLOAD_PROTOCOL_COMMAND_START_DOWNLOAD 'S'
/** Value to send to continue the download. */
#define SHELL_COMMAND_DOWNLOAD_PROTOCOL_COMMAND_CONTINUE_DOWNLOAD 'C'
/** Value to send to abort the download. */
#define SHELL_COMMAND_DOWNLOAD_PROTOCOL_COMMAND_ABORT_DOWNLOAD 'A'

/** A block size in bytes (how many bytes are read from the UART then written on the disk). */
#define SHELL_COMMAND_DOWNLOAD_PROTOCOL_BLOCK_SIZE 4096

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ShellCommandDownload(void)
{
	char String_File_Name[CONFIGURATION_FILE_NAME_LENGTH + 1];
	unsigned int Total_Download_Bytes_Count = 0, Block_Download_Bytes_Count, i, File_Descriptor;
	unsigned char Buffer[SHELL_COMMAND_DOWNLOAD_PROTOCOL_BLOCK_SIZE]; // Store it on the stack to avoid loosing DATA or BSS memory
	
	// Is there any remaining room in the Files List ? Do not start the download if the file system can't store the file
	if (FileSystemGetFreeFilesListEntriesCount() == 0)
	{
		ScreenSetColor(SCREEN_COLOR_RED);
		ScreenWriteString(STRING_SHELL_DOWNLOAD_NO_MORE_FILE_LIST_ENTRY);
		return;
	}
	
	// Configure the UART every time (to avoid problem if an other application modifies UART settings)
	UARTInitialize(UART_DISABLE_PARITY, UART_BAUD_RATE_115200);
	ScreenWriteString(STRING_SHELL_DOWNLOAD_WAITING_FOR_SERVER);
	
	// Send special code to tell server that we are ready for download
	while (1)
	{
		UARTWriteByte(SHELL_COMMAND_DOWNLOAD_PROTOCOL_COMMAND_REQUEST_DOWNLOAD);
		
		// Did the user pressed escape key ?
		if (KeyboardIsKeyAvailable())
		{
			if (KeyboardReadCharacter() == KEYBOARD_KEY_CODE_ESCAPE) return;
		}
		
		// Did the server started the transmission ?
		if (UARTIsDataReceived())
		{
			if (UARTReadByte() == SHELL_COMMAND_DOWNLOAD_PROTOCOL_COMMAND_START_DOWNLOAD) break;
		}
	}
	
	// Get the size of the file to download (in bytes)
	for (i = 0; i < 4; i++)
	{
		Total_Download_Bytes_Count <<= 8;
		Total_Download_Bytes_Count |= UARTReadByte();
	}
	
	// Get the file name (the number of sent characters is fixed)
	for (i = 0; i < CONFIGURATION_FILE_NAME_LENGTH; i++) String_File_Name[i] = UARTReadByte();
	String_File_Name[CONFIGURATION_FILE_NAME_LENGTH] = 0;
	
	// Display file informations
	ScreenWriteString(STRING_SHELL_DOWNLOAD_SHOW_FILE_INFORMATIONS_1);
	ScreenWriteString(String_File_Name);
	ScreenWriteString(STRING_SHELL_DOWNLOAD_SHOW_FILE_INFORMATIONS_2);
	ScreenWriteString(itoa(Total_Download_Bytes_Count));
	ScreenWriteString(STRING_SHELL_DOWNLOAD_SHOW_FILE_INFORMATIONS_3);
	
	// Abort the download according to file size
	ScreenSetColor(SCREEN_COLOR_RED);
	// There is nothing to download
	if (Total_Download_Bytes_Count == 0)
	{
		ScreenWriteString(STRING_SHELL_DOWNLOAD_FILE_SIZE_NULL);
		UARTWriteByte(SHELL_COMMAND_DOWNLOAD_PROTOCOL_COMMAND_ABORT_DOWNLOAD); // Stop transfer
		return;
	}
	// The file size is larger than the available RAM
	if (Total_Download_Bytes_Count > CONFIGURATION_USER_SPACE_SIZE)
	{
		ScreenWriteString(STRING_SHELL_DOWNLOAD_FILE_SIZE_TOO_BIG);
		UARTWriteByte(SHELL_COMMAND_DOWNLOAD_PROTOCOL_COMMAND_ABORT_DOWNLOAD); // Stop transfer
		return;
	}
	// There is not enough room to store the file on the file system
	if (Total_Download_Bytes_Count > FileSystemGetFreeBlocksCount() * CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES)
	{
		ScreenWriteString(STRING_SHELL_DOWNLOAD_NO_MORE_BLOCK_LIST_ENTRY);
		UARTWriteByte(SHELL_COMMAND_DOWNLOAD_PROTOCOL_COMMAND_ABORT_DOWNLOAD); // Stop transfer
		return;
	}
	
	// Open the destination file
	if (FileOpen(String_File_Name, 'w', &File_Descriptor) != ERROR_CODE_NO_ERROR)
	{
		ScreenSetColor(SCREEN_COLOR_RED);
		ScreenWriteString(STRING_SHELL_DOWNLOAD_FILE_OPENING_FAILED);
		return;
	}
	
	// Accept the transfer
	UARTWriteByte(SHELL_COMMAND_DOWNLOAD_PROTOCOL_COMMAND_CONTINUE_DOWNLOAD);
	
	// Download the file and write it on the file system
	while (Total_Download_Bytes_Count > 0)
	{
		// Compute the amount of bytes to download
		if (Total_Download_Bytes_Count >= SHELL_COMMAND_DOWNLOAD_PROTOCOL_BLOCK_SIZE) Block_Download_Bytes_Count = SHELL_COMMAND_DOWNLOAD_PROTOCOL_BLOCK_SIZE;
		else Block_Download_Bytes_Count = Total_Download_Bytes_Count;
		
		// Download a block of data
		for (i = 0; i < Block_Download_Bytes_Count; i++) Buffer[i] = UARTReadByte();
		
		// Write it to the file
		if (FileWrite(File_Descriptor, Buffer, Block_Download_Bytes_Count) != ERROR_CODE_NO_ERROR)
		{
			FileResetFileDescriptors(); // Do not close the file to avoid saving corrupted data
			ScreenSetColor(SCREEN_COLOR_RED);
			ScreenWriteString(STRING_SHELL_DOWNLOAD_FILE_WRITING_FAILED);
			return;
		}
		
		// Tell the server that it can send the next block of data
		UARTWriteByte(SHELL_COMMAND_DOWNLOAD_PROTOCOL_COMMAND_CONTINUE_DOWNLOAD);
		
		Total_Download_Bytes_Count -= Block_Download_Bytes_Count;
	}
	
	FileClose(File_Descriptor);
	
	// Show download completed message
	ScreenSetColor(SCREEN_COLOR_GREEN);
	ScreenWriteString(STRING_SHELL_DOWNLOAD_DOWNLOADING_COMPLETED);
	ScreenSetColor(SCREEN_COLOR_BLUE);
}