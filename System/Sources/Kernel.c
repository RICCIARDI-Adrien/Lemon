/** @file Kernel.c
 * @see Kernel.h for description.
 * @author Adrien RICCIARDI
 */
#include <Architecture.h>
#include <Drivers/Driver_Ethernet.h>
#include <Drivers/Driver_Hard_Disk.h>
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_PIC.h>
#include <Drivers/Driver_Screen.h>
#include <Drivers/Driver_Timer.h>
#include <Error_Codes.h>
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
		: "i" (CONFIGURATION_KERNEL_STACK_ADDRESS) \
		: "esp" \
	)

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
// These symbols come from the linker script. Their address must be used, not their value
/** The BSS (variables that value will default to zero) section start (4-byte aligned). */
extern unsigned int *_bss_start;
/** The BSS section end (4-byte aligned). */
extern unsigned int *_bss_end;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Wait for the user to hit the Enter key. */
static inline __attribute__((always_inline)) void KernelWaitForEnterKey(void)
{
	ARCHITECTURE_INTERRUPTS_ENABLE();
	while (KeyboardReadCharacter() != '\n');
}

//-------------------------------------------------------------------------------------------------
// Kernel entry point
//-------------------------------------------------------------------------------------------------
/** The MBR directly calls this function. */
void __attribute__((section(".init"))) KernelEntryPoint(void)
{
	unsigned int *Pointer_Dword;
	int Result;
	#if !CONFIGURATION_BUILD_INSTALLER
		unsigned int Partition_Starting_Sector;
	#endif
	
	// Clear the BSS section as the compiler expects
	for (Pointer_Dword = (unsigned int *) &_bss_start; Pointer_Dword < (unsigned int *) &_bss_end; Pointer_Dword++) *Pointer_Dword = 0;
	
	// Initialize Intel memory protection mechanisms
	ArchitectureInitializeMemoryProtection();
	
	// Configure the interrupt controller
	PICInitialize();
	
	// Enable access to the whole memory
	KeyboardEnableA20Gate();
	
	// Others standard peripherals configuration
	outb(0x3F2, 0x0C); // Floppy disk (for now it only disables the motor)
	KeyboardInitialize();
	TimerInitialize();
	
	// Clear the screen with the default background color
	ScreenSetColor(SCREEN_COLOR_BLUE);
	ScreenClear();
	
	// Enable the interrupts sooner in debug mode to allow KeyboardReadCharacter() to work (be careful if the not yet initialized devices use interrupts)
	#if CONFIGURATION_IS_DEBUG_ENABLED
		ARCHITECTURE_INTERRUPTS_ENABLE();
	#endif
	
	// Initialize the compilation-selected hard disk driver
	Result = HardDiskInitialize();
	if (Result != 0)
	{
		ScreenSetColor(SCREEN_COLOR_RED);
		
		switch (Result)
		{
			case 1:
				ScreenWriteString(STRING_KERNEL_ERROR_HARD_DISK_NOT_LBA_COMPATIBLE);
				break;
				
			case 2:
				ScreenWriteString(STRING_KERNEL_ERROR_SATA_HARD_DISK_NOT_FOUND);
				break;
		}
			
		// Wait for Enter key to reboot
		KernelWaitForEnterKey();
		KeyboardRebootSystem();
	}
	
	#if (!CONFIGURATION_BUILD_INSTALLER) && CONFIGURATION_IS_ETHERNET_CONTROLLER_ENABLED
		Result = EthernetInitialize();
		if (Result != 0)
		{
			ScreenSetColor(SCREEN_COLOR_RED);
			
			switch (Result)
			{
				case 1:
					ScreenWriteString(STRING_KERNEL_ERROR_ETHERNET_CONTROLLER_NOT_FOUND);
					break;
					
				case 2:
					ScreenWriteString(STRING_KERNEL_ERROR_ETHERNET_CONTROLLER_BAD_DRIVER);
					break;
					
				default:
					break;
			}
			
			// Wait for Enter key to reboot
			KernelWaitForEnterKey();
			KeyboardRebootSystem();
		}
	#endif
	
	// Load file system (can't do that in the Installer program)
	#if !CONFIGURATION_BUILD_INSTALLER
		// Retrieve the partition starting sector from the partition table located in the MBR
		Partition_Starting_Sector = *((unsigned int *) (CONFIGURATION_SYSTEM_MBR_LOAD_ADDRESS + 446 + 8)); // The partition table is located at offset 446, and the first partition starting LBA sector is located at offset 8 of the beginning of the partition table
		
		if (!FileSystemInitialize(Partition_Starting_Sector + CONFIGURATION_FILE_SYSTEM_STARTING_SECTOR_OFFSET))
		{
			// Show error message
			ScreenSetColor(SCREEN_COLOR_RED); // Color must be set before in order to clear screen with this value
			ScreenWriteString(STRING_KERNEL_ERROR_INVALID_FILE_SYSTEM);
			
			// Wait for Enter key to reboot
			KernelWaitForEnterKey();
			KeyboardRebootSystem();
		}
	#endif
	
	// Show the welcoming message once at the system boot
	ScreenSetColor(SCREEN_COLOR_LIGHT_BLUE);
	ScreenClear(); // Clear eventual debug messages
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
	ARCHITECTURE_INTERRUPTS_ENABLE();
	Shell();
}

TErrorCode KernelStartProgram(void)
{
	// Reset files opened by the previous program
	FileResetFileDescriptors();
	
	// Reset normal console color
	ScreenSetColor(SCREEN_COLOR_BLUE);
	
	ArchitectureSwitchToUserSpace();
	
	// Should never be reached
	return ERROR_CODE_NO_ERROR;
}

// This error is not triggered by the MMU, so it is not an interrupt handler
void KernelUnknownSystemCallErrorHandler(void)
{
	// Show error message
	ScreenSetColor(SCREEN_COLOR_RED);
	ScreenClear();
	ScreenWriteString(STRING_KERNEL_ERROR_UNKNOWN_SYSTEM_CALL);
	
	KernelWaitForEnterKey();
	
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
	ScreenWriteString(STRING_KERNEL_EXCEPTION_DIVISION_BY_ZERO);
	
	KernelWaitForEnterKey();
	
	// Restart the shell
	KernelStartShell();
}

/** This handler is called by the processor when general protection fault happens. */
void KernelGeneralProtectionFaultExceptionInterruptHandler(void) // Can't be static as it must be linked with assembly frame
{
	// Show error message
	ScreenSetColor(SCREEN_COLOR_RED);
	ScreenClear();
	ScreenWriteString(STRING_KERNEL_EXCEPTION_GENERAL_PROTECTION_FAULT);
	
	KernelWaitForEnterKey();
	
	// Restart the shell
	KernelStartShell();
}

/** A kernel stack exception occurred, this should never happen. */
void KernelStackExceptionInterruptHandler(void) // Can't be static as it must be linked with assembly frame
{
	// Show error message
	ScreenSetColor(SCREEN_COLOR_RED);
	ScreenClear();
	ScreenWriteString(STRING_KERNEL_EXCEPTION_STACK_OVERFLOW);
	
	KernelWaitForEnterKey();
	
	// This is the safest way
	KeyboardRebootSystem();
}
