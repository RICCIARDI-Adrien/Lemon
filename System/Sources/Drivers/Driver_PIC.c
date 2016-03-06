/** @file Driver_PIC.c
 * @see Driver_PIC.h for description.
 * @author Adrien RICCIARDI
 */
#include <Architecture.h>
#include <Drivers/Driver_PIC.h>
#include <Hardware_Functions.h> // To have outb() function

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** The first PIC Interrupt Command register (only valid for output). */
#define PIC_1_REGISTER_INTERRUPT_COMMAND 0x20
/** The first PIC Interrupt Commands register (only valid for output). */
#define PIC_1_REGISTER_INTERRUPT_COMMANDS 0x21

/** The second PIC Interrupt Command register (only valid for output). */
#define PIC_2_REGISTER_INTERRUPT_COMMAND 0xA0
/** The second PIC Interrupt Commands register (only valid for output). */
#define PIC_2_REGISTER_INTERRUPT_COMMANDS 0xA1

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void PICInitialize(void)
{
	// ICW1, double cascaded PIC mode
	outb(PIC_1_REGISTER_INTERRUPT_COMMAND, 0x11); // Master
	outb(PIC_2_REGISTER_INTERRUPT_COMMAND, 0x11); // Slave
	ArchitectureIODelay();
	
	// ICW2
	outb(PIC_1_REGISTER_INTERRUPT_COMMANDS, 32); // Starting interrupt vector for IRQ 0 to 7 handlers (must be divisible by 8)
	outb(PIC_2_REGISTER_INTERRUPT_COMMANDS, 40); // Starting interrupt vector for IRQ 8 to 15 handlers (must be divisible by 8)
	ArchitectureIODelay();
	
	// ICW3
	outb(PIC_1_REGISTER_INTERRUPT_COMMANDS, 0x04); // The slave PIC is connected to the master's IRQ 2
	outb(PIC_2_REGISTER_INTERRUPT_COMMANDS, 0x02);
	ArchitectureIODelay();
	
	// ICW4
	outb(PIC_1_REGISTER_INTERRUPT_COMMANDS, 0x01); // Disable "fully nested" mode, disable buffering
	outb(PIC_2_REGISTER_INTERRUPT_COMMANDS, 0x01);
	ArchitectureIODelay();

	// Enable all IRQs
	outb(PIC_1_REGISTER_INTERRUPT_COMMANDS, 0);
	outb(PIC_2_REGISTER_INTERRUPT_COMMANDS, 0);
}