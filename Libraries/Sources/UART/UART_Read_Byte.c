/** @file UART_Read_Byte.c
 * Read a byte from the serial port. This is a blocking function.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/12/2012
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned char UARTReadByte(void)
{
	return SystemCall(SYSTEM_CALL_UART_READ_BYTE, 0, 0, NULL, NULL);
}