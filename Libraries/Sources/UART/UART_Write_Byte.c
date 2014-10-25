/** @file UART_Write_Byte.c
 * Write a byte to the serial port.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/12/2012
 */
#include <System.h>
#include <UART.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void UARTWriteByte(unsigned char Byte)
{
	SystemCall(SYSTEM_CALL_UART_WRITE_BYTE, Byte, 0, NULL, NULL);
}