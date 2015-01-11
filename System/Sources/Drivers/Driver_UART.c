/** @file Driver_UART.c
 * @see Driver_UART.h for description.
 * @author Adrien RICCIARDI
 */
#include <Drivers/Driver_UART.h>
#include <Error_Codes.h>
#include <Hardware_Functions.h> // Needed for inb() and outb() functions

//-------------------------------------------------------------------------------------------------
// Private constant
//-------------------------------------------------------------------------------------------------
/** COM 1 port. */
#define UART_BASE_PORT 0x3F8

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int UARTInitialize(unsigned char Frame_Format, unsigned short Data_Rate_Bauds)
{
	// Check parameters correctness
	switch (Frame_Format)
	{
		case UART_DISABLE_PARITY:
		case UART_ODD_PARITY:
		case UART_EVEN_PARITY:
			break;
		default:
			return ERROR_CODE_BAD_UART_PARAMETERS;
	}
	
	switch (Data_Rate_Bauds)
	{
		case UART_BAUD_RATE_1200:
		case UART_BAUD_RATE_2400:
		case UART_BAUD_RATE_4800:
		case UART_BAUD_RATE_9600:
		case UART_BAUD_RATE_19200:
		case UART_BAUD_RATE_38400:
		case UART_BAUD_RATE_57600:
		case UART_BAUD_RATE_115200:
			break;
		default:
			return ERROR_CODE_BAD_UART_PARAMETERS;
	}
			
	// Disable interrupts
	asm("cli");
	
	// Rate configuration
	outb(UART_BASE_PORT + 3, 0x80); // Force DLAB bit to '1'
	outb(UART_BASE_PORT, Data_Rate_Bauds); // Frequency divider low byte
	outb(UART_BASE_PORT + 1, Data_Rate_Bauds >> 8); // Frequency divider high byte
	
	// Frame format configuration
	Frame_Format &= 0x7F; // Force DLAB to '0'
	Frame_Format |= 0x03; // Force data size to 8 bits per frame
	outb(UART_BASE_PORT + 3, Frame_Format);
	
	// Disable UART interrupts
	outb(UART_BASE_PORT + 1, 0);
	
	// MODEM configuration
	outb(UART_BASE_PORT + 4, 0);
	
	// Clear reception buffer by reading it
	inb(UART_BASE_PORT);
	
	// Enable interrupts
	asm("sti");
	
	return ERROR_CODE_NO_ERROR;
}

unsigned char UARTReadByte(void)
{
	// Wait for a data to be received
	while (!(inb(UART_BASE_PORT + 5) & 0x01));
	return inb(UART_BASE_PORT);
}

void UARTWriteByte(unsigned char Byte)
{
	// Wait for the line to be free
	while (!(inb(UART_BASE_PORT + 5) & 0x20));
	outb(UART_BASE_PORT, Byte);
}

int UARTIsDataReceived(void)
{
	return inb(UART_BASE_PORT + 5) & 0x01;
}