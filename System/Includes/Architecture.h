/** @file Architecture.h
 * Intel IA-32 dependant functions.
 * @author Adrien RICCIARDI
 * @version 1.0 : 12/05/2014
 */
#ifndef H_ARCHITECTURE_H
#define H_ARCHITECTURE_H

//-------------------------------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------------------------------
/** Initialize the GDT, IDT and TSS. */
void ArchitectureInitializeMemoryProtection(void);

/** Switch from kernel space to user space. */
void ArchitectureSwitchToUserSpace(void);

/** Add delay needed by some low speed hardware. */
void ArchitectureIODelay(void);

#endif