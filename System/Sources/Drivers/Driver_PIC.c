/** @file Driver_PIC.c
 * @see Driver_PIC.h for description.
 * @author Adrien RICCIARDI
 */
#include <Architecture.h> // To have outb() function
#include <Drivers/Driver_PIC.h>
#include <Hardware_Functions.h>

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void PICInitialize(void)
{
	// ICW1, double cascaded PIC mode
	outb(0x20, 0x11); // Master
	outb(0xA0, 0x11); // Slave
	ArchitectureIODelay();
	
	// ICW2
	outb(0x21, 32); // Starting address of IRQ 0 to 7 handlers
	outb(0xA1, 40); // Starting address of IRQ 8 to 15 handlers
	ArchitectureIODelay();
	
	// ICW3
	outb(0x21, 0x04); // The slave PIC is connected on the master's IRQ 2
	outb(0xA1, 0x02);
	ArchitectureIODelay();
	
	// ICW4
	outb(0x21, 0x01); // Disable "fully nested" mode, disable buffering
	outb(0xA1, 0x01);
	ArchitectureIODelay();

	// Enable interrupts handling
	outb(0x20, 0);
	outb(0xA1, 0);
}