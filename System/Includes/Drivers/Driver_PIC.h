/** @file Driver_PIC.h
 * Configure the Programmable Interrupt Controllers.
 * @author Adrien RICCIARDI
 */
#ifndef H_DRIVER_PIC_H
#define H_DRIVER_PIC_H

//-------------------------------------------------------------------------------------------------
// Constants and macros
//-------------------------------------------------------------------------------------------------
/** Acknowledge master and slave PICs interrupt. */
#define PIC_ACKNOWLEDGE_INTERRUPT() \
	asm \
	( \
		"push eax\n" \
		"mov al, 32\n" \
		"out 0xA0, al\n" /* Send End Of Interrupt code to the slave PIC before */ \
		"out 0x20, al\n" /* Send End Of Interrupt code to the master PIC */ \
		"pop eax\n" \
	)

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the two PC PICs to generate the wanted interrupts. */
void PICInitialize(void);

#endif