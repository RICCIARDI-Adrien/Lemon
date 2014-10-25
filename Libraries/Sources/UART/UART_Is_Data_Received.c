/** @file UART_Is_Data_Received.c
 * Tell if the serial port received a byte or not. This is a non blocking function.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/12/2012
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int UARTIsDataReceived(void)
{
	return SystemCall(SYSTEM_CALL_UART_IS_DATA_RECEIVED, 0, 0, NULL, NULL);
}