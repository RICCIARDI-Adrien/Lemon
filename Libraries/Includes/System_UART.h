/** @file System_UART.h
 * Read from and write to the PC serial port.
 * @author Adrien RICCIARDI
 */
#ifndef H_SYSTEM_UART_H
#define H_SYSTEM_UART_H

//-------------------------------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------------------------------
/** Disable parity when initializing UART. */
#define SYSTEM_UART_PARITY_NONE 0x00
/** Set odd parity (parity must be enabled to use this flag). */
#define SYSTEM_UART_PARITY_ODD 0x08
/** Set even parity (parity must be enabled to use this flag). */
#define SYSTEM_UART_PARITY_EVEN 0x18

/** Baud rate at 1200 bit/s. */
#define SYSTEM_UART_BAUD_RATE_1200 96
/** Baud rate at 2400 bit/s. */
#define SYSTEM_UART_BAUD_RATE_2400 48
/** Baud rate at 4800 bit/s. */
#define SYSTEM_UART_BAUD_RATE_4800 24
/** Baud rate at 9600 bit/s. */
#define SYSTEM_UART_BAUD_RATE_9600 12
/** Baud rate at 19200 bit/s. */
#define SYSTEM_UART_BAUD_RATE_19200 6
/** Baud rate at 38400 bit/s. */
#define SYSTEM_UART_BAUD_RATE_38400 3
/** Baud rate at 57600 bit/s. */
#define SYSTEM_UART_BAUD_RATE_57600 2
/** Baud rate at 115200 bit/s. */
#define SYSTEM_UART_BAUD_RATE_115200 1

// TODO : enums ?

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the UART with the specified frame format and baud rate.
 * @param Frame_Format The format of the RS-232 frame. Use a constant from SYSTEM_UART_PARITY_NONE, SYSTEM_UART_PARITY_EVEN or SYSTEM_UART_PARITY_ODD to configure serial port.
 * @param Data_Rate_Bauds The data rate from a SYSTEM_UART_BAUD_RATE_XXX constant.
 * @return ERROR_CODE_NO_ERROR if the UART was correctly initialized,
 * @return ERROR_CODE_BAD_UART_PARAMETERS if the provided parameters did not come from UART_XXX contants.
 */
int SystemUARTInitialize(unsigned char Frame_Format, unsigned short Data_Rate_Bauds);

/** Receive a byte from the serial port.
 * @return The received byte.
 * @note This is a blocking function.
 */
unsigned char SystemUARTReadByte(void);

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
