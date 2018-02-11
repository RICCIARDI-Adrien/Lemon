/** @file UART_Write_Byte.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void LibrariesUARTWriteByte(unsigned char Byte)
{
	LibrariesSystemCall(SYSTEM_CALL_UART_WRITE_BYTE, Byte, 0, NULL, NULL);
}
