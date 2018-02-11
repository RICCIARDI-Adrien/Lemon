/** @file Libraries_UART_Read_Byte.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
unsigned char LibrariesUARTReadByte(void)
{
	return LibrariesSystemCall(SYSTEM_CALL_UART_READ_BYTE, 0, 0, NULL, NULL);
}
