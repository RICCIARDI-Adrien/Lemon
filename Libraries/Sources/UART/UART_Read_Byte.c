/** @file UART_Read_Byte.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned char UARTReadByte(void)
{
	return SystemCall(SYSTEM_CALL_UART_READ_BYTE, 0, 0, NULL, NULL);
}