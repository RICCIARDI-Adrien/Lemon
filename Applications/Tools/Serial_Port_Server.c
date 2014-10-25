/** @file Serial_Port_Server.c
 * Tool to transfer files to the operating system. Transfer can be made via a VirtualBox pipe or a real RS-232 serial port (USB serial ports are also supported).
 * Compile using : gcc Serial_Port_Server.c -o Serial_Port_Server.out
 * @author Adrien RICCIARDI
 * @version 1.0 : 10/04/2011
 * @version 1.1 : 18/11/2012, done some basic optimizations.
 * @version 1.2 : 06/01/2013, changed request and start codes to help debugging and increased transfer speed.
 */
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>

#define CODE_DOWNLOAD_REQUEST 'R'
#define CODE_DOWNLOAD_START 'S'
#define CODE_DOWNLOAD_CONTINUE 'C'
#define CODE_DOWNLOAD_ABORT 'A'

#define BAUD_RATE B115200

/** Get file size in bytes.
 * @param File_Name Name of the file.
 * @return The file size in bytes.
 */
long FileSize(char *File_Name)
{
	FILE *File_Temp;
	long Size;
	
	File_Temp = fopen(File_Name, "a");
	if (File_Temp == NULL) return -1;
	
	Size = ftell(File_Temp);
	fclose(File_Temp);
	return Size;
}

int main(int argc, char *argv[])
{
	int COM_Descriptor, Is_Pipe = 0;
	struct termios COM_Parameters;
	char Character, *File_Name, *Device_File_Name;
	long File_Size_Bytes;
	FILE *File;
	struct sockaddr_un Socket_Parameters;
	
	// Check parameters
	if (argc != 4)
	{
		printf("Error : bad parameters.\nUsage : %s  Device_File  Is_Pipe=0|1  File_To_Send\n", argv[0]);
		return 0;
	}
	Device_File_Name = argv[1];
	Is_Pipe = atoi(argv[2]);
	File_Name = argv[3];
	
	// Open file to send
	File = fopen(File_Name,"r");
	if (File == NULL)
	{
		printf("Error : can't open file '%s'.\n", File_Name);
		return -1;
	}
	
	// Configure communication device
	// Real serial port
	if (!Is_Pipe)
	{
		// Initialize COM port
		COM_Descriptor = open(Device_File_Name, O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO); // COM1 = "/dev/ttyS0"
		if (COM_Descriptor == -1)
		{
			printf("Error : can't access to serial port (%s).\n", strerror(errno));
			return -1;
		}

		COM_Parameters.c_iflag = IGNPAR;
		COM_Parameters.c_oflag = 0;
		COM_Parameters.c_cflag = CS8 | CREAD | CLOCAL;
		COM_Parameters.c_lflag = 0;
		
		if (cfsetispeed(&COM_Parameters, BAUD_RATE) != 0)
		{
			printf("Error : can't set serial port input baud rate (%s).\n", strerror(errno));
			return -1;
		}
		if (cfsetospeed(&COM_Parameters, BAUD_RATE) != 0)
		{
			printf("Error : can't set serial port output baud rate (%s).\n", strerror(errno));
			return -1;
		}
		
		tcsetattr(COM_Descriptor, TCSANOW, &COM_Parameters);
		
		printf("Serial port %s successfully initialized.\n", Device_File_Name);
	}
	// VirtualBox pipe (which is a socket)
	else
	{
		// Open socket
		COM_Descriptor = socket(PF_LOCAL, SOCK_STREAM, 0);
		if (COM_Descriptor < 0)
		{
			printf("Error : can't open VirtualBox's pipe.\n");
			return -1;
		}
       
		Socket_Parameters.sun_family = AF_LOCAL;
		strncpy(Socket_Parameters.sun_path, Device_File_Name, sizeof(Socket_Parameters.sun_path));

		// Try to connect to virtual box
		if (connect(COM_Descriptor, (struct sockaddr *) &Socket_Parameters, sizeof(Socket_Parameters)) < 0)
		{
			printf("Error : can't connect to pipe (%s).\n", strerror(errno));
			return -1;
		}
		
		printf("Successfully connected to pipe.\n");
	}
	
	// Show file size
	File_Size_Bytes = FileSize(File_Name);
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
	
	// Send file size (4 bytes)
	write(COM_Descriptor, &File_Size_Bytes, 4);
	
	// Wait for client's answer (for instance, he can abort the transfer if the file is too big)
	do
	{
		read(COM_Descriptor, &Character, 1);
	} while ((Character != CODE_DOWNLOAD_CONTINUE) && (Character != CODE_DOWNLOAD_ABORT));

	// Stop transfer if it is the client's wish
	if (Character == CODE_DOWNLOAD_ABORT)
	{
		printf("Client aborted transfer.\n");
		fclose(File);
		close(COM_Descriptor);
		return 0;
	}
	
	// Send file data
	printf("Client accepted transfer. Sending file...\n");
	while (!feof(File))
	{
		if (fread(&Character, 1, 1, File) == 1) write(COM_Descriptor, &Character, 1);
	}
	
	fclose(File);
	close(COM_Descriptor);
	printf("File successfully sent.\n");
	return 0;
}
