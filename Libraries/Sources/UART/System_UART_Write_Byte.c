/** @file System_UART_Write_Byte.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void SystemUARTWriteByte(unsigned char Byte)
{
	LibrariesSystemCall(SYSTEM_CALL_UART_WRITE_BYTE, Byte, 0, NULL, NULL);
}
