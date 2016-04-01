/** @file Hardware_Functions.h
 * Prototypes for hardware functions written in assembly.
 * @author Adrien RICCIARDI
 */
#ifndef H_HARDWARE_FUNCTIONS_H
#define H_HARDWARE_FUNCTIONS_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Write a byte to the specified port on the system bus.
 * @param Port The port number.
 * @param Data The data to write.
 */
void outb(unsigned short Port, unsigned char Data);

/** Write a word to the specified port on the system bus.
 * @param Port The port number.
 * @param Data The data to write.
 */
void outw(unsigned short Port, unsigned short Data);

/** Write a double-word to the specified port on the system bus.
 * @param Port The port number.
 * @param Data The data to write.
 */
void outd(unsigned short Port, unsigned int Data);

/** Read a byte from the system bus.
 * @param Port The port number to read from.
 * @return The read byte.
 */
unsigned char inb(unsigned short Port);

/** Read a word from the system bus.
 * @param Port The port number to read from.
 * @return The read word.
 */
unsigned short inw(unsigned short Port);

/** Read a double-word from the system bus.
 * @param Port The port number to read from.
 * @return The read double-word.
 */
unsigned int ind(unsigned short Port);

#endif
