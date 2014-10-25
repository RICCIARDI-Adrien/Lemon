/** @file Driver_UART.h
 * Universal Asynchronous Receiver Transmitter (RS-232 serial port) driver.
 * @author Adrien RICCIARDI
 * @version 1.0 : 22/07/2011
 * @version 1.1 : 17/11/2011
 * @version 1.2 : 01/08/2012
 * @version 1.3 : 24/12/2012
 */
#ifndef H_DRIVER_UART_H
#define H_DRIVER_UART_H

//------------------------------------------------------------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------------------------------------------------------------
/** Disable parity when initializing UART. */
#define UART_DISABLE_PARITY 0x00
/** Set odd parity (parity must be enabled to use this flag). */
#define UART_ODD_PARITY 0x08
/** Set even parity (parity must be enabled to use this flag). */
#define UART_EVEN_PARITY 0x18

/** Baud rate at 1200 bit/s. */
#define UART_BAUD_RATE_1200 96
/** Baud rate at 2400 bit/s. */
#define UART_BAUD_RATE_2400 48
/** Baud rate at 4800 bit/s. */
#define UART_BAUD_RATE_4800 24
/** Baud rate at 9600 bit/s. */
#define UART_BAUD_RATE_9600 12
/** Baud rate at 19200 bit/s. */
#define UART_BAUD_RATE_19200 6
/** Baud rate at 38400 bit/s. */
#define UART_BAUD_RATE_38400 3
/** Baud rate at 57600 bit/s. */
#define UART_BAUD_RATE_57600 2
/** Baud rate at 115200 bit/s. */
#define UART_BAUD_RATE_115200 1

//------------------------------------------------------------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------------------------------------------------------------
/** Initialize the UART with the specified frame format and baud rate.
 * @param Frame_Format The format of the RS-232 frame. Use a OR combination of UART_DISABLE_PARITY, UART_EVEN_PARITY and UART_ODD_PARITY to configure serial port.
 * @param Data_Rate_Bauds The data rate from a UART_BAUD_RATE_XX constant.
 * @return ERROR_CODE_NO_ERROR if the UART was correctly initialized,
 * @return ERROR_CODE_BAD_UART_PARAMETERS if the provided parameters did not come from UART_XXX contants.
 */
int UARTInit(unsigned char Frame_Format, unsigned short Data_Rate_Bauds);

/** Receive a byte from the serial port.
 * @return The received byte.
 * @note This is a blocking function.
 */
unsigned char UARTReadByte(void);

/** Send a byte via serial port.
 * @param Byte The byte to send.
 */
void UARTWriteByte(unsigned char Byte);

/** Tell if data was received by the UART.
 * @return 1 if there is data to read, or 0 if no data was received.
 * @note This is a non-blocking function.
 */
int UARTIsDataReceived(void);

#endif