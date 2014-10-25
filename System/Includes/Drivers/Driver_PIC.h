/** @file Driver_PIC.h
 * Configure the Programmable Interrupt Controllers.
 * @author Adrien RICCIARDI
 * @version 1.0 : 26/04/2014
 */
#ifndef H_DRIVER_PIC_H
#define H_DRIVER_PIC_H

//-------------------------------------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------------------------------------
/** Initialize the two PC PICs to generate the wanted interrupts. */
void PICInitialize(void);

#endif