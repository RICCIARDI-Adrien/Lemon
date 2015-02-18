/** @file Serial_Port_Server.c
 * Tool to transfer files to the operating system. Transfer can be made via a VirtualBox pipe or a real RS-232 serial port (USB serial ports are also supported).
 * Compile using : gcc Serial_Port_Server.c -o Serial_Port_Server.out
 * @author Adrien RICCIARDI
 * @version 1.0 : 10/04/2011
 * @version 1.1 : 18/11/2012, done some basic optimizations.
 * @version 1.2 : 06/01/2013, changed request and start codes to help debugging and increased transfer speed.
 * @version 1.3 : 18/02/2015, added autodetection of the serial port file type (pipe or real device), added the ability to send automatically the file name.
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
#define CODE_DOWNLOAD_REQUEST 'R'
#define CODE_DOWNLOAD_START 'S'
#define CODE_DOWNLOAD_CONTINUE 'C'
#define CODE_DOWNLOAD_ABORT 'A'

#define BAUD_RATE B115200

/** The maximum length of a file name (this is the same value than the system has). */
#define FILE_NAME_LENGTH 12

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

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int COM_Descriptor, Is_Pipe, Return_Value = EXIT_FAILURE;
	struct termios COM_Parameters;
	char Character, *String_File_Name, *String_Serial_Port_File_Name, *String_File_Name_On_System;
	long File_Size_Bytes;
	FILE *File;
	struct sockaddr_un Socket_Parameters;
	struct stat Status;
	
	// Check parameters
	if (argc != 4)
	{
		printf("Error : bad parameters.\nUsage : %s Serial_Port_File File_To_Send File_Name_On_System\n", argv[0]);
		return EXIT_FAILURE;
	}
	String_Serial_Port_File_Name = argv[1];
	String_File_Name = argv[2];
	String_File_Name_On_System = argv[3];
	
	// Retrieve informations about the file
	if (stat(String_Serial_Port_File_Name, &Status) != 0)
	{
		printf("Error : can't determinate the serial port file type (%s).\n", strerror(errno));
		return EXIT_FAILURE;
	}
	// Find the serial port file type
	if (S_ISCHR(Status.st_mode)) Is_Pipe = 0;
	else if (S_ISSOCK(Status.st_mode)) Is_Pipe = 1;
	else
	{
		printf("Error : the serial port file type is not supported.\n");
		return EXIT_FAILURE;
	}
	
	// Open the file to send
	File = fopen(String_File_Name, "r");
	if (File == NULL)
	{
		printf("Error : can't open file '%s' (%s).\n", String_File_Name, strerror(errno));
		return EXIT_FAILURE;
	}
	
	// Configure the appropriate communication device
	// Real serial port
	if (!Is_Pipe)
	{
		// Initialize COM port
		COM_Descriptor = open(String_Serial_Port_File_Name, O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
		if (COM_Descriptor == -1)
		{
			printf("Error : can't access to serial port (%s).\n", strerror(errno));
			goto Exit;
		}

		COM_Parameters.c_iflag = IGNPAR;
		COM_Parameters.c_oflag = 0;
		COM_Parameters.c_cflag = CS8 | CREAD | CLOCAL;
		COM_Parameters.c_lflag = 0;
		
		if (cfsetispeed(&COM_Parameters, BAUD_RATE) != 0)
		{
			printf("Error : can't set serial port input baud rate (%s).\n", strerror(errno));
			goto Exit;
		}
		if (cfsetospeed(&COM_Parameters, BAUD_RATE) != 0)
		{
			printf("Error : can't set serial port output baud rate (%s).\n", strerror(errno));
			goto Exit;
		}
		
		tcsetattr(COM_Descriptor, TCSANOW, &COM_Parameters);
		
		printf("Serial port %s successfully initialized.\n", String_Serial_Port_File_Name);
	}
	// VirtualBox pipe (which is a socket)
	else
	{
		// Open socket
		COM_Descriptor = socket(PF_LOCAL, SOCK_STREAM, 0);
		if (COM_Descriptor < 0)
		{
			printf("Error : can't open VirtualBox's pipe (%s).\n", strerror(errno));
			goto Exit;
		}

		Socket_Parameters.sun_family = AF_LOCAL;
		strncpy(Socket_Parameters.sun_path, String_Serial_Port_File_Name, sizeof(Socket_Parameters.sun_path));

		// Try to connect to virtual box
		if (connect(COM_Descriptor, (struct sockaddr *) &Socket_Parameters, sizeof(Socket_Parameters)) < 0)
		{
			printf("Error : can't connect to pipe (%s).\n", strerror(errno));
			goto Exit;
		}
		
		printf("Successfully connected to pipe.\n");
	}
	
	// Show file size
	File_Size_Bytes = GetFileSize(String_File_Name);
	printf("File size : %ld bytes .\n", File_Size_Bytes);
	
	// Wait for client connection
	printf("Waiting for client connection...\n");
	do
	{
		read(COM_Descriptor, &Character, 1);
	} while (Character != CODE_DOWNLOAD_REQUEST);
	
	// Start transfer
	Character = CODE_DOWNLOAD_START;
	write(COM_Descriptor, &Character, 1);
	printf("Connection established. Sending file size...\n");
	
	// Send file size (4 bytes) in big endian order
	File_Size_Bytes = htonl(File_Size_Bytes); // This value is not used anymore after this transfer, so it can be safely modified
	write(COM_Descriptor, &File_Size_Bytes, 4);
	
	// Send file name (up to FILE_NAME_LENGTH characters)
	write(COM_Descriptor, String_File_Name_On_System, FILE_NAME_LENGTH);
	
	// Wait for client's answer (for instance, he can abort the transfer if the file is too big)
	do
	{
		read(COM_Descriptor, &Character, 1);
	} while ((Character != CODE_DOWNLOAD_CONTINUE) && (Character != CODE_DOWNLOAD_ABORT));

	// Stop transfer if it is the client's wish
	if (Character == CODE_DOWNLOAD_ABORT)
	{
		printf("Client aborted transfer.\n");
		goto Exit;
	}
	
	// Send file data
	printf("Client accepted transfer. Sending file...\n");
	while (!feof(File))
	{
		if (fread(&Character, 1, 1, File) == 1) write(COM_Descriptor, &Character, 1);
	}
	
	printf("File successfully sent.\n");
	Return_Value = EXIT_SUCCESS;
	
Exit:
	fclose(File);
	close(COM_Descriptor);
	return Return_Value;
}
