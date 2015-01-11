/** @file Shell_Command_Download.c
 * Download a file from the UART and save it to disk.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/12/2012
 * @version 1.1 : 29/12/2012, added to kernel code.
 */
#include <Configuration.h> // Needed by CONFIGURATION_FILE_NAME_LENGTH
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_Screen.h>
#include <Drivers/Driver_UART.h>
#include <Error_Codes.h>
#include <File_System/File.h>
#include <Kernel.h> // Needed for kernel constants
#include <Shell/Shell_Commands.h>
#include <Standard_Functions.h> // Needed by itoa()
#include <Strings.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Value to send to indicate that the shell is ready to download a file. */
#define CODE_DOWNLOAD_REQUEST 'R'
/** Value received when the server starts to send the file. */
#define CODE_DOWNLOAD_START 'S'
/** Value to send to continue the download. */
#define CODE_DOWNLOAD_CONTINUE 'C'
/** Value to send to abort the download. */
#define CODE_DOWNLOAD_ABORT 'A'

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ShellCommandDownload(void)
{
	char Buffer[CONFIGURATION_FILE_NAME_LENGTH + 1];
	unsigned int Download_Bytes_Count = 0, i;
	unsigned char *Pointer_Downloaded_Data;
	
	// Configure the UART every time (to avoid problem if an other application modifies UART settings)
	UARTInitialize(UART_DISABLE_PARITY, UART_BAUD_RATE_115200);
	ScreenWriteString(STRING_SHELL_DOWNLOAD_WAITING_FOR_SERVER);
			
	// Send special code to tell server that we are ready for download
	while (1)
	{
		UARTWriteByte(CODE_DOWNLOAD_REQUEST);
		
		// Did the user pressed escape key ?
		if (KeyboardIsKeyAvailable())
		{
			if (KeyboardReadChar() == KEYBOARD_KEY_CODE_ESCAPE) return;
		}
		
		// Did the server started the transmission ?
		if (UARTIsDataReceived())
		{
			if (UARTReadByte() == CODE_DOWNLOAD_START) break;
		}
	}
	
	ScreenWriteString(STRING_SHELL_DOWNLOAD_DOWNLOADING_FILE);
			
	// Get the size of the file to download (in bytes)
	Pointer_Downloaded_Data = (unsigned char *) &Download_Bytes_Count;
	for (i = 0; i < 4; i++) // Here with gcc 4.4.5-8 (debian 6.0.6) Pointer_Downloaded_Data[i] doesn't point on the good address, why ? Bad kernel segment start address ?
	{
		*Pointer_Downloaded_Data = UARTReadByte();
		Pointer_Downloaded_Data++;
	}
	ScreenWriteString(STRING_SHELL_DOWNLOAD_SHOW_FILE_SIZE_1);
	ScreenWriteString(itoa(Download_Bytes_Count));
	ScreenWriteString(STRING_SHELL_DOWNLOAD_SHOW_FILE_SIZE_2);
	
	// Abort the download according to file size
	ScreenSetColor(SCREEN_COLOR_RED);
	// There is nothing to download
	if (Download_Bytes_Count == 0)
	{
		ScreenWriteString(STRING_SHELL_DOWNLOAD_FILE_SIZE_NULL);
		UARTWriteByte(CODE_DOWNLOAD_ABORT); // Stop transfer
		return;
	}
	// The file size is larger than the available RAM
	if (Download_Bytes_Count > KERNEL_USER_SPACE_SIZE)
	{
		ScreenWriteString(STRING_SHELL_DOWNLOAD_FILE_SIZE_TOO_BIG);
		UARTWriteByte(CODE_DOWNLOAD_ABORT); // Stop transfer
		return;
	}
	
	// Accept the transfer
	UARTWriteByte(CODE_DOWNLOAD_CONTINUE);
	
	// Copy bytes in the memory
	Pointer_Downloaded_Data = (unsigned char *) KERNEL_USER_SPACE_ADDRESS;
	for (i = 0; i < Download_Bytes_Count; i++)
	{
		*Pointer_Downloaded_Data = UARTReadByte();
		Pointer_Downloaded_Data++;
	}

	// Show download completed message
	ScreenSetColor(SCREEN_COLOR_GREEN);
	ScreenWriteString(STRING_SHELL_DOWNLOAD_DOWNLOADING_COMPLETED);
	ScreenSetColor(SCREEN_COLOR_BLUE);
	
	// Ask user on what he wants to do with the file
	do
	{
		ScreenWriteString(STRING_SHELL_DOWNLOAD_FINAL_QUESTION);
		KeyboardReadString(Buffer, 1);
	} while ((Buffer[0] != 's') && (Buffer[0] != 'a'));
			
	// Save the file on the hard disk
	if (Buffer[0] == 's')
	{
		ScreenWriteString(STRING_SHELL_DOWNLOAD_ASK_FILE_NAME);
		KeyboardReadString(Buffer, CONFIGURATION_FILE_NAME_LENGTH + 1);
		
		switch (FileCreate(Buffer, (unsigned char *) KERNEL_USER_SPACE_ADDRESS, Download_Bytes_Count))
		{
			case  ERROR_CODE_NO_ERROR:
				ScreenSetColor(SCREEN_COLOR_GREEN);
				ScreenWriteString(STRING_SHELL_DOWNLOAD_FILE_SAVE_COMPLETED);
				break;
				
			case ERROR_CODE_BAD_FILE_NAME:
				ScreenSetColor(SCREEN_COLOR_RED);
				ScreenWriteString(STRING_SHELL_DOWNLOAD_BAD_FILE_NAME);
				break;
				
			default:
				ScreenSetColor(SCREEN_COLOR_RED);
				ScreenWriteString(STRING_SHELL_DOWNLOAD_FILE_SAVE_FAILED);
				break;
		}
	}
}