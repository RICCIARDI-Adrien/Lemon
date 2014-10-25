/** @file UART_Initialize.c
 * Initialize the RS-232 serial port.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/12/2012
 */
#include <System.h>
#include <UART.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int UARTInitialize(unsigned char Frame_Format, unsigned short Data_Rate_Bauds)
{
	return SystemCall(SYSTEM_CALL_UART_INIT, Frame_Format, Data_Rate_Bauds, NULL, NULL);
}