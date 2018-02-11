/** @file Libraries_UART_Initialize.c
 * @author Adrien RICCIARDI
 */
#include <Libraries.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int LibrariesUARTInitialize(unsigned char Frame_Format, unsigned short Data_Rate_Bauds)
{
	return LibrariesSystemCall(SYSTEM_CALL_UART_INITIALIZE, Frame_Format, Data_Rate_Bauds, NULL, NULL);
}
