/** @file Kernel.c
 * @see Kernel.h for description.
 * @author Adrien RICCIARDI
 */
#include <Architecture.h>
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_PIC.h>
#include <Drivers/Driver_Screen.h>
#include <Drivers/Driver_Timer.h>
#include <File_System/File.h>
#include <File_System/File_System.h>
#include <Hardware_Functions.h>
#include <Kernel.h>
#include <Shell/Shell.h>
#include <Standard_Functions.h>
#include <Strings.h>
#include <System_Calls.h>

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Reset the kernel stack. */
#define KERNEL_RESET_STACK() \
	asm \
	( \
		"mov esp, %0" \
		: /* No output */ \
		: "i" (KERNEL_STACK_ADDRESS) \
		: "esp" \
	)
	
/** Enable interrupts. */
#define KERNEL_ENABLE_INTERRUPTS() asm("sti")

//-------------------------------------------------------------------------------------------------
// Kernel entry point
//-------------------------------------------------------------------------------------------------
void __attribute__((section(".init"))) KernelEntryPoint(void)
{
	// Initialize Intel memory protection mechanisms
	ArchitectureInitializeMemoryProtection();
	
	// Configure the interrupt controller
	PICInitialize();
	
	// Enable access to the whole memory
	KeyboardEnableA20Gate();
	
	// Others peripherals configuration
	outb(0x3F2, 0x0C); // Floppy disk (for now it only disables the motor)
	KeyboardInitialize();
	TimerInitialize();
	
	// Load file system (can't do that in the Installer program)
	#ifndef INSTALLER
		if (!FileSystemInit())
		{
			// Show error message
			ScreenSetColor(SCREEN_COLOR_RED); // Color must be set before in order to clear screen with this value
			ScreenClear();
			ScreenWriteString(STRING_KERNEL_CONSOLE_FILE_SYSTEM_ERROR);
			
			// Wait for Enter key to reboot
			KERNEL_ENABLE_INTERRUPTS();
			while (KeyboardReadCharacter() != '\n');
			KeyboardRebootSystem();
		}
	#endif
	
	// Show the welcoming message once at the system boot
	ScreenSetColor(SCREEN_COLOR_LIGHT_BLUE);
	ScreenClear();
	ScreenWriteString(STRING_SHELL_WELCOME);
	
	// Jump to shell
	KernelStartShell();
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void KernelStartShell(void)
{
	KERNEL_RESET_STACK();
	KERNEL_ENABLE_INTERRUPTS();
	Shell();
}

void KernelStartProgram(void)
{
	// Reset files opened by the previous program
	FileResetFileDescriptors();
	
	ArchitectureSwitchToUserSpace();
}

// This error is not triggered by the MMU, so it is not an interrupt handler
void KernelUnknownSystemCallErrorHandler(void)
{
	// Show error message
	ScreenSetColor(SCREEN_COLOR_RED);
	ScreenClear();
	ScreenWriteString(STRING_KERNEL_CONSOLE_UNKNWOWN_SYSTEM_CALL_ERROR);
	
	// Wait for the user to hit the Enter key
	KERNEL_ENABLE_INTERRUPTS();
	while (KeyboardReadCharacter() != '\n');
	
	// Restart the shell
	KernelStartShell();
}

//-------------------------------------------------------------------------------------------------
// Interrupt handlers
//-------------------------------------------------------------------------------------------------
/** This handler is called when a div or idiv instructions tries to divide by zero. */
void KernelDivisionErrorExceptionInterruptHandler(void) // Can't be static as it must be linked with assembly frame
{
	// Show error message
	ScreenSetColor(SCREEN_COLOR_RED);
	ScreenClear();
	ScreenWriteString(STRING_KERNEL_CONSOLE_DIVISION_ERROR_EXCEPTION);
	
	// Wait for the user to hit the Enter key
	KERNEL_ENABLE_INTERRUPTS();
	while (KeyboardReadCharacter() != '\n');
	
	// Restart the shell
	KernelStartShell();
}

/** This handler is called by the processor when general protection fault happens. */
void KernelGeneralProtectionFaultExceptionInterruptHandler(void) // Can't be static as it must be linked with assembly frame
{
	// Show error message
	ScreenSetColor(SCREEN_COLOR_RED);
	ScreenClear();
	ScreenWriteString(STRING_KERNEL_CONSOLE_GENERAL_PROTECTION_FAULT_EXCEPTION);
	
	// Wait for the user to hit the Enter key
	KERNEL_ENABLE_INTERRUPTS();
	while (KeyboardReadCharacter() != '\n');
	
	// Restart the shell
	KernelStartShell();
}

/** A kernel stack exception occurred, this should never happen. */
void KernelStackExceptionInterruptHandler(void) // Can't be static as it must be linked with assembly frame
{
	// Show error message
	ScreenSetColor(SCREEN_COLOR_RED);
	ScreenClear();
	ScreenWriteString(STRING_KERNEL_CONSOLE_STACK_EXCEPTION);
	
	// Wait for the user to hit the Enter key
	KERNEL_ENABLE_INTERRUPTS();
	while (KeyboardReadCharacter() != '\n');
	
	// This is the safest way
	KeyboardRebootSystem();
}