/** @file UART_Write_Byte.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void UARTWriteByte(unsigned char Byte)
{
	SystemCall(SYSTEM_CALL_UART_WRITE_BYTE, Byte, 0, NULL, NULL);
}