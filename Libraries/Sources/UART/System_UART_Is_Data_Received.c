/** @file System_UART_Is_Data_Received.c
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int SystemUARTIsDataReceived(void)
{
	return SystemCall(SYSTEM_CALL_UART_IS_DATA_RECEIVED, 0, 0, NULL, NULL);
}
