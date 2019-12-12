/** @file Kernel.c
 * See Kernel.h for description.
 * @author Adrien RICCIARDI
 */
#include <Architecture.h>
#include <Configuration.h>
#include <Debug.h>
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
#include <Shell.h>
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
		: \
	)

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
	unsigned int *Pointer_Double_Word;
	int Result;
	char *String_Error_Message;
	#if (!CONFIGURATION_BUILD_INSTALLER) && (!CONFIGURATION_BUILD_RAM_DISK)
		unsigned int Partition_Starting_Sector;
	#endif
	
	// Enable access to the whole memory before anything else, as BSS section resides after the first 1MB RAM
	KeyboardEnableA20Gate();
	
	// Clear the BSS section as the compiler expects
	for (Pointer_Double_Word = &Configuration_Memory_BSS_Start_Address; Pointer_Double_Word < &Configuration_Memory_BSS_End_Address; Pointer_Double_Word++) *Pointer_Double_Word = 0;
	
	// Initialize Intel memory protection mechanisms
	ArchitectureInitializeMemoryProtection();
	
	// Configure the interrupt controller
	PICInitialize();
	
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
	
	DEBUG_SECTION_START
		DEBUG_DISPLAY_CURRENT_FUNCTION_NAME();
		ScreenWriteString("BSS section start address : 0x");
		DebugWriteHexadecimalInteger((unsigned int) &Configuration_Memory_BSS_Start_Address);
		ScreenWriteString(", BSS section end address : 0x");
		DebugWriteHexadecimalInteger((unsigned int) &Configuration_Memory_BSS_End_Address);
		ScreenWriteString(", user space start address : 0x");
		DebugWriteHexadecimalInteger(CONFIGURATION_USER_SPACE_ADDRESS);
		ScreenWriteString(", BSS section size : ");
		ScreenWriteString(itoa((unsigned int) &Configuration_Memory_BSS_End_Address - (unsigned int) &Configuration_Memory_BSS_Start_Address));
		ScreenWriteString(" bytes, user space size : ");
		ScreenWriteString(itoa(CONFIGURATION_USER_SPACE_SIZE));
		ScreenWriteString(" bytes.\n");
		KeyboardReadCharacter();
	DEBUG_SECTION_END
	
	// Initialize the compilation-selected hard disk driver
	Result = HardDiskInitialize();
	if (Result != 0)
	{
		switch (Result)
		{
			case 1:
				String_Error_Message = STRING_KERNEL_ERROR_HARD_DISK_NOT_LBA_COMPATIBLE;
				break;
				
			case 2:
				String_Error_Message = STRING_KERNEL_ERROR_SATA_HARD_DISK_NOT_FOUND;
				break;
				
			case 3:
				String_Error_Message = STRING_KERNEL_ERROR_FAILED_TO_CREATE_RAM_DISK;
				break;
				
			case 4:
				String_Error_Message = STRING_KERNEL_ERROR_INVALID_FILE_SYSTEM;
				break;
				
			case 5:
				String_Error_Message = STRING_KERNEL_ERROR_FAILED_TO_POPULATE_RAM_DISK;
				break;
		}
		
		// Display the error message
		ScreenSetColor(SCREEN_COLOR_RED);
		ScreenWriteString(String_Error_Message);
		
		// Wait for Enter key to reboot
		KernelWaitForEnterKey();
		KeyboardRebootSystem();
	}
	
	// Allow ethernet support to RAM disk build or to system build, do not include it for a normal installer build
	#if CONFIGURATION_IS_ETHERNET_CONTROLLER_ENABLED && ((!CONFIGURATION_BUILD_INSTALLER) || (CONFIGURATION_BUILD_INSTALLER && CONFIGURATION_BUILD_RAM_DISK))
		Result = EthernetInitialize();
		if (Result != 0)
		{
			switch (Result)
			{
				case 1:
					String_Error_Message = STRING_KERNEL_ERROR_ETHERNET_CONTROLLER_NOT_FOUND;
					break;
					
				case 2:
					String_Error_Message = STRING_KERNEL_ERROR_ETHERNET_CONTROLLER_BAD_DRIVER;
					break;
			}
			
			// Display the error message
			ScreenSetColor(SCREEN_COLOR_RED);
			ScreenWriteString(String_Error_Message);
			
			// Wait for Enter key to reboot
			KernelWaitForEnterKey();
			KeyboardRebootSystem();
		}
	#endif
	
	// Load file system (can't do that in the Installer program nor when a RAM disk is used because the RAM disk already loaded the file system)
	#if (!CONFIGURATION_BUILD_INSTALLER) && (!CONFIGURATION_BUILD_RAM_DISK)
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
	
	// Execute automatic starting program if present
	#if !CONFIGURATION_BUILD_INSTALLER
		Result = ShellLoadAndStartProgram(CONFIGURATION_FILE_STARTED_ON_BOOT_NAME);
		
		// An error occurred when this code is reached
		ScreenSetColor(SCREEN_COLOR_RED);
		switch (Result)
		{
			case ERROR_CODE_FILE_LARGER_THAN_RAM:
				ScreenWriteString(STRING_KERNEL_ERROR_FILE_STARTED_ON_BOOT_LARGER_THAN_RAM);
				break;
				
			case ERROR_CODE_FILE_READING_FAILED:
				ScreenWriteString(STRING_KERNEL_ERROR_FILE_STARTED_ON_BOOT_READ_FAILURE);
				break;
				
			case ERROR_CODE_FILE_NOT_EXECUTABLE:
				ScreenWriteString(STRING_KERNEL_ERROR_FILE_STARTED_ON_BOOT_NOT_EXECUTABLE);
				break;
				
			// Shouldn't be reached
			default:
				break;
		}
	#endif
	
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
