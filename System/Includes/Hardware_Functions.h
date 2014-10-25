/** @file Hardware_Functions.h
 * Prototypes for hardware functions written in assembly.
 * @author Adrien RICCIARDI
 * @version 1.0 : 24/07/2011 
 * @version 1.1 : 16/11/2011
 * @version 1.2 : 01/08/2012
 */
#ifndef H_HARDWARE_FUNCTIONS_H
#define H_HARDWARE_FUNCTIONS_H

/** Write a byte to the specified port on the system bus.
 * @param Port The port number.
 * @param Byte The byte to write.
 */
void outb(unsigned short Port, unsigned char Byte);

/** Read a byte from the system bus.
 * @param Port The port number from which read the byte.
 * @return The byte read.
 */
unsigned char inb(unsigned short Port);

#endif
