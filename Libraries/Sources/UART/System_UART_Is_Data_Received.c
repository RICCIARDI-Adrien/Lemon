/** @file Libraries_UART_Is_Data_Received.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesUARTIsDataReceived(void)
{
	return LibrariesSystemCall(SYSTEM_CALL_UART_IS_DATA_RECEIVED, 0, 0, NULL, NULL);
}
