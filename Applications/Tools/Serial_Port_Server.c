/** @file Serial_Port_Server.c
 * Tool to transfer files to the operating system. Transfer can be made via a VirtualBox pipe or a real RS-232 serial port (USB serial ports are also supported).
 * Compile using : gcc Serial_Port_Server.c -o Serial_Port_Server.out
 * @author Adrien RICCIARDI
 */
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <termios.h>
#include <unistd.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** Value received when the client wants to download a file. */
#define PROTOCOL_COMMAND_REQUEST_DOWNLOAD 'R'
/** Value sent when the server starts sending the file. */
#define PROTOCOL_COMMAND_START_DOWNLOAD 'S'
/** Value received when the client wants the next data block. */
#define PROTOCOL_COMMAND_CONTINUE_DOWNLOAD 'C'
/** Value received when the clients interrupts the download. */
#define PROTOCOL_COMMAND_ABORT_DOWNLOAD 'A'

/** A block size in bytes (how many bytes are sent through the serial port before waiting for an acknowledge from the client). */
#define PROTOCOL_BLOCK_SIZE 4096

/** UART baud rate (using termios constants). */
#define BAUD_RATE B115200

/** The maximum length of a file name (this is the same value than the system has). */
#define SYSTEM_FILE_NAME_LENGTH 12

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Get the file size in bytes.
 * @param String_File_Name Name of the file.
 * @return The file size in bytes.
 */
static long GetFileSize(char *String_File_Name)
{
	FILE *File_Temp;
	long Size;
	
	// Open the file and set the position at the file end
	File_Temp = fopen(String_File_Name, "a");
	if (File_Temp == NULL) return -1;
	
	Size = ftell(File_Temp);
	fclose(File_Temp);
	return Size;
}

/** Configure the serial port for reading and writing (automatically detect if it is a real UART or a Virtual Box socket).
 * @param String_Serial_Port_File_Name The serial port to configure.
 * @return -1 if an error happened,
 * @return A positive value on success corresponding to the serial port file descriptor.
 */
static int ConfigureSerialPort(char *String_Serial_Port_File_Name)
{
	struct stat Status;
	struct termios COM_Parameters;
	struct sockaddr_un Socket_Parameters;
	int File_Descriptor;
	
	// Retrieve informations about the serial port file
	if (stat(String_Serial_Port_File_Name, &Status) != 0)
	{
		printf("Error : can't determinate the serial port file type (%s).\n", strerror(errno));
		return -1;
	}
	
	// Configure the appropriate communication device
	if (S_ISCHR(Status.st_mode)) // This is a character device, so it is a real UART
	{
		// Initialize COM port
		File_Descriptor = open(String_Serial_Port_File_Name, O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
		if (File_Descriptor == -1)
		{
			printf("Error : can't access to serial port (%s).\n", strerror(errno));
			return -1;
		}

		COM_Parameters.c_iflag = IGNPAR;
		COM_Parameters.c_oflag = 0;
		COM_Parameters.c_cflag = CS8 | CREAD | CLOCAL;
		COM_Parameters.c_lflag = 0;
		
		// Set baud rate
		if (cfsetispeed(&COM_Parameters, BAUD_RATE) != 0)
		{
			printf("Error : can't set serial port input baud rate (%s).\n", strerror(errno));
			close(File_Descriptor);
			return -1;
		}
		if (cfsetospeed(&COM_Parameters, BAUD_RATE) != 0)
		{
			printf("Error : can't set serial port output baud rate (%s).\n", strerror(errno));
			close(File_Descriptor);
			return -1;
		}
		
		// Apply new configuration
		tcsetattr(File_Descriptor, TCSANOW, &COM_Parameters);
		
		printf("Serial port %s successfully initialized.\n", String_Serial_Port_File_Name);
	}
	else if (S_ISSOCK(Status.st_mode)) // Virtual Box socket (named "pipe" in the Virtual Box interface)
	{
		// Open socket
		File_Descriptor = socket(PF_LOCAL, SOCK_STREAM, 0);
		if (File_Descriptor < 0)
		{
			printf("Error : can't open Virtual Box pipe (%s).\n", strerror(errno));
			return -1;
		}

		Socket_Parameters.sun_family = AF_LOCAL;
		strncpy(Socket_Parameters.sun_path, String_Serial_Port_File_Name, sizeof(Socket_Parameters.sun_path));

		// Try to connect to the socket
		if (connect(File_Descriptor, (struct sockaddr *) &Socket_Parameters, sizeof(Socket_Parameters)) < 0)
		{
			printf("Error : can't connect to pipe (%s).\n", strerror(errno));
			close(File_Descriptor);
			return -1;
		}
		
		printf("Successfully connected to pipe.\n");
	}
	else
	{
		printf("Error : the serial port file type is not supported.\n");
		return -1;
	}
	
	return File_Descriptor;
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int File_Descriptor_Serial_Port, File_Descriptor_File_To_Send, Return_Value = EXIT_FAILURE;
	char Character, *String_File_Name, *String_Serial_Port_File_Name, *String_File_Name_On_System;
	long File_Size_Bytes;
	ssize_t Read_Bytes_Count;
	static unsigned char Buffer[PROTOCOL_BLOCK_SIZE]; // Avoid storing it on the stack, even if the buffer is not really large
	
	// Check parameters
	if (argc != 4)
	{
		printf("Error : bad parameters.\nUsage : %s Serial_Port_File File_To_Send File_Name_On_System\n", argv[0]);
		return EXIT_FAILURE;
	}
	String_Serial_Port_File_Name = argv[1];
	String_File_Name = argv[2];
	String_File_Name_On_System = argv[3];
	
	// Open the file to send
	File_Descriptor_File_To_Send = open(String_File_Name, O_RDONLY);
	if (File_Descriptor_File_To_Send == -1)
	{
		printf("Error : can't open file '%s' (%s).\n", String_File_Name, strerror(errno));
		return EXIT_FAILURE;
	}
	
	// Configure the communication device
	File_Descriptor_Serial_Port = ConfigureSerialPort(String_Serial_Port_File_Name);
	if (File_Descriptor_Serial_Port == -1) goto Exit; // Error message is displayed by ConfigureSerialPort() yet, no need to print another one
	
	// Show file size
	File_Size_Bytes = GetFileSize(String_File_Name);
	printf("File size : %ld bytes.\n", File_Size_Bytes);
	
	// Wait for client connection
	printf("Waiting for client connection...\n");
	do
	{
		read(File_Descriptor_Serial_Port, &Character, 1);
	} while (Character != PROTOCOL_COMMAND_REQUEST_DOWNLOAD);
	
	// Start transfer
	Character = PROTOCOL_COMMAND_START_DOWNLOAD;
	write(File_Descriptor_Serial_Port, &Character, 1);
	printf("Connection established. Sending file information...\n");
	
	// Send file size (4 bytes) in big endian order
	File_Size_Bytes = htonl(File_Size_Bytes); // This value is not used anymore after this transfer, so it can be safely modified
	write(File_Descriptor_Serial_Port, &File_Size_Bytes, 4);
	
	// Send file name (up to SYSTEM_FILE_NAME_LENGTH characters)
	write(File_Descriptor_Serial_Port, String_File_Name_On_System, SYSTEM_FILE_NAME_LENGTH);
	
	// Wait for client's answer (for instance, he can abort the transfer if the file is too big)
	do
	{
		read(File_Descriptor_Serial_Port, &Character, 1);
	} while ((Character != PROTOCOL_COMMAND_CONTINUE_DOWNLOAD) && (Character != PROTOCOL_COMMAND_ABORT_DOWNLOAD));

	// Stop transfer if it is the client's wish
	if (Character == PROTOCOL_COMMAND_ABORT_DOWNLOAD)
	{
		printf("Client aborted transfer.\n");
		goto Exit;
	}
	
	// Send file data
	printf("Client accepted transfer. Sending file...\n");
	while (1)
	{
		// Read a block of data from the file to send
		Read_Bytes_Count = read(File_Descriptor_File_To_Send, Buffer, PROTOCOL_BLOCK_SIZE);
		if (Read_Bytes_Count == -1)
		{
			printf("Error : failed to read from the file to send (%s).\n", strerror(errno));
			goto Exit;
		}
		// End-of-file has been reached
		if (Read_Bytes_Count == 0) break;
		
		// Send the data block
		if (write(File_Descriptor_Serial_Port, Buffer, Read_Bytes_Count) != Read_Bytes_Count)
		{
			printf("Error : failed to send data block (%s).\n", strerror(errno));
			goto Exit;
		}
		
		// Wait for the client acknowledge
		do
		{
			read(File_Descriptor_Serial_Port, &Character, 1);
		} while (Character != PROTOCOL_COMMAND_CONTINUE_DOWNLOAD);
	}
	
	printf("File successfully sent.\n");
	Return_Value = EXIT_SUCCESS;
	
Exit:
	close(File_Descriptor_File_To_Send);
	close(File_Descriptor_Serial_Port);
	return Return_Value;
}
