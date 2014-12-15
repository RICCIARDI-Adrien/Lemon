/** @file System_Calls.c
 * @see System_Calls.h for description.
 * @author Adrien RICCIARDI
 */
#include <Configuration.h>
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_Screen.h>
#include <Drivers/Driver_Timer.h>
#include <Drivers/Driver_UART.h>
#include <File_System/File.h>
#include <File_System/File_System.h>
#include <Kernel.h>
#include <System_Calls.h>

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
// Maybe use this in future to avoid multiple casts
/*typedef union
{
	char *String;
	unsigned char *Pointer_Buffer;
	int Signed_Integer;
	unsigned int Unsigned_Integer;
} TParameter;*/

/** A system call handler. */
typedef void (*TSystemCallHandler)(void);

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
// System call parameters
static TSystemCall Call_Code;
static int Integer_1, Integer_2, Return_Value;
static void *Pointer_1, *Pointer_2;

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
//====================================================================================================================
// Kernel / system calls
//====================================================================================================================
static void SystemCallSystemGetParameter(void)
{
	unsigned int *Pointer_Result;
	
	Return_Value = 0;
	Pointer_Result = Pointer_1;
	
	// Handle all parameters
	switch (Integer_1)
	{
		case SYSTEM_CALL_SYSTEM_PARAMETER_ID_MEMORY_TOTAL_SIZE:
			*Pointer_Result = CONFIGURATION_SYSTEM_TOTAL_RAM_SIZE_MEGA_BYTES;
			break;
			
		case SYSTEM_CALL_SYSTEM_PARAMETER_ID_MEMORY_USER_SIZE:
			*Pointer_Result = CONFIGURATION_SYSTEM_TOTAL_RAM_SIZE_MEGA_BYTES - 1;
			break;
			
		case SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_MAXIMUM_OPENED_FILES_COUNT:
			*Pointer_Result = CONFIGURATION_FILE_SYSTEM_MAXIMUM_OPENED_FILES_COUNT;
			break;
			
		case SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_BLOCK_SIZE:
			*Pointer_Result = CONFIGURATION_FILE_SYSTEM_BLOCK_SIZE_BYTES;
			break;
			
		case SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_MAXIMUM_FILE_LIST_ENTRIES_COUNT:
			*Pointer_Result = CONFIGURATION_FILE_SYSTEM_MAXIMUM_FILES_LIST_ENTRIES;
			break;
			
		case SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_MAXIMUM_BLOCK_ALLOCATION_TABLE_ENTRIES_COUNT:
			*Pointer_Result = CONFIGURATION_FILE_SYSTEM_MAXIMUM_BLOCK_ALLOCATION_TABLE_ENTRIES;
			break;
			
		case SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_FREE_FILE_LIST_ENTRIES_COUNT:
			*Pointer_Result = FileSystemGetFreeFileListEntriesCount();
			break;
			
		case SYSTEM_CALL_SYSTEM_PARAMETER_ID_FILE_SYSTEM_FREE_BLOCK_ALLOCATION_TABLE_ENTRIES_COUNT:
			*Pointer_Result = FileSystemGetFreeBlocksCount();
			break;
			
		// Unknown parameter
		default:
			Return_Value = 1;
			break;
	}
}

static void SystemCallTimerReadValue(void)
{
	Return_Value = Timer_Counter;
}

static void SystemCallTimerWait(void)
{
	TimerWait(Integer_1);
}

static void SystemCallUARTInitialize(void)
{
	Return_Value = UARTInitialize(Integer_1, Integer_2);
}

static void SystemCallUARTReadByte(void)
{
	Return_Value = UARTReadByte();
}

static void SystemCallUARTWriteByte(void)
{
	UARTWriteByte(Integer_1);
}

static void SystemCallUARTIsDataReceived(void)
{
	Return_Value = UARTIsDataReceived();
}

//====================================================================================================================
// Screen calls
//====================================================================================================================
static void SystemCallScreenWriteCharacter(void)
{
	ScreenWriteChar((char) Integer_1);
}

static void SystemCallScreenWriteString(void)
{
	ScreenWriteString(Pointer_1);
}

static void SystemCallScreenGetColor(void)
{
	Return_Value = ScreenGetColor();
}

static void SystemCallScreenSetColor(void)
{
	ScreenSetColor(Integer_1);
}

static void SystemCallScreenGetCursorPosition(void)
{
	ScreenGetCursorPosition(Pointer_1, Pointer_2);
}

static void SystemCallScreenSetCursorPosition(void)
{
	ScreenSetCursorPosition(Integer_1, Integer_2);
}

static void SystemCallScreenDisplayBuffer(void)
{
	ScreenDisplayBuffer(Pointer_1);
}

//====================================================================================================================
// Keyboard calls
//====================================================================================================================
static void SystemCallKeyboardReadCharacter(void)
{
	asm("sti");
	Return_Value = KeyboardReadChar();
}

static void SystemCallKeyboardIsKeyAvailable(void)
{
	Return_Value = KeyboardIsKeyAvailable();
}

//====================================================================================================================
// File system calls
//====================================================================================================================
static void SystemCallFileExists(void)
{
	Return_Value = FileExists(Pointer_1);
}

static void SystemCallFileSize(void)
{
	Return_Value = FileSize(Pointer_1);
}

static void SystemCallFileListNext(void)
{
	FileListNext(Pointer_1);
}

static void SystemCallFileDelete(void)
{
	Return_Value = FileDelete(Pointer_1);
}

static void SystemCallFileRename(void)
{
	Return_Value = FileRename(Pointer_1, Pointer_2);
}

static void SystemCallFileOpen(void)
{
	Return_Value = FileOpen(Pointer_1, (char) Integer_1, Pointer_2);
}

static void SystemCallFileRead(void)
{
	Return_Value = FileRead((unsigned int) Integer_1, Pointer_1, (unsigned int) Integer_2, Pointer_2); 
}

static void SystemCallFileWrite(void)
{
	Return_Value = FileWrite((unsigned int) Integer_1, Pointer_1, (unsigned int) Integer_2);
}

static void SystemCallFileClose(void)
{
	FileClose(Integer_1);
}

//====================================================================================================================
// Jump table
//====================================================================================================================
TSystemCallHandler System_Calls_Handlers[] =
{
	KernelStartShell, // SYSTEM_CALL_SYSTEM_EXIT_PROGRAM
	SystemCallSystemGetParameter, // SYSTEM_CALL_SYSTEM_GET_PARAMETER
	SystemCallTimerReadValue, // SYSTEM_CALL_TIMER_READ_VALUE
	SystemCallTimerWait, // SYSTEM_CALL_TIMER_WAIT
	SystemCallUARTInitialize, // SYSTEM_CALL_UART_INITIALIZE
	SystemCallUARTReadByte, // SYSTEM_CALL_UART_READ_BYTE
	SystemCallUARTWriteByte, // SYSTEM_CALL_UART_WRITE_BYTE
	SystemCallUARTIsDataReceived, // SYSTEM_CALL_UART_IS_DATA_RECEIVED
	ScreenClear, // SYSTEM_CALL_SCREEN_CLEAR
	SystemCallScreenWriteCharacter, // SYSTEM_CALL_SCREEN_WRITE_CHARACTER
	SystemCallScreenWriteString, // SYSTEM_CALL_SCREEN_WRITE_STRING
	SystemCallScreenGetColor, // SYSTEM_CALL_SCREEN_GET_COLOR
	SystemCallScreenSetColor, // SYSTEM_CALL_SCREEN_SET_COLOR
	SystemCallScreenGetCursorPosition, // SYSTEM_CALL_SCREEN_GET_CURSOR_POSITION
	SystemCallScreenSetCursorPosition, // SYSTEM_CALL_SCREEN_SET_CURSOR_POSITION
	SystemCallScreenDisplayBuffer, // SYSTEM_CALL_SCREEN_DISPLAY_BUFFER
	SystemCallKeyboardReadCharacter, // SYSTEM_CALL_KEYBOARD_READ_CHARACTER
	SystemCallKeyboardIsKeyAvailable, // SYSTEM_CALL_KEYBOARD_IS_KEY_AVAILABLE
	SystemCallFileExists, // SYSTEM_CALL_FILE_EXISTS
	FileListInitialize, // SYSTEM_CALL_FILE_LIST_INITIALIZE
	SystemCallFileListNext, // SYSTEM_CALL_FILE_LIST_NEXT
	SystemCallFileSize, // SYSTEM_CALL_FILE_SIZE
	SystemCallFileDelete, // SYSTEM_CALL_FILE_DELETE
	SystemCallFileRename, // SYSTEM_CALL_FILE_RENAME
	SystemCallFileOpen, // SYSTEM_CALL_FILE_OPEN
	SystemCallFileRead, // SYSTEM_CALL_FILE_READ
	SystemCallFileWrite, // SYSTEM_CALL_FILE_WRITE
	SystemCallFileClose, // SYSTEM_CALL_FILE_CLOSE
};

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
int SystemCalls(void)
{
	// Get call parameters
	asm
	(
		"mov [%0], eax\n"
		"mov [%1], ebx\n"
		"mov [%2], ecx\n"
		"mov [%3], edx\n"
		"mov [%4], esi"
		: // No output parameters
		: "m" (Call_Code), "m" (Integer_1), "m" (Integer_2), "m" (Pointer_1), "m" (Pointer_2)
		: "eax", "ebx", "ecx", "edx", "esi"
	);
	
	// Is the system call implemented ?
	if (Call_Code >= SYSTEM_CALLS_COUNT) KernelUnknownSystemCallErrorHandler(); // This function never returns
	
	// Adjust string pointers according to user segment base into GDT
	Pointer_1 += KERNEL_USER_SPACE_ADDRESS;
	Pointer_2 += KERNEL_USER_SPACE_ADDRESS;
	
	// It is possible for a user space program to provide a pointer that, when added with the user space offset, will overlap in the kernel memory area. Generate a GPF if this is the case
	if (((unsigned int) Pointer_1 < KERNEL_USER_SPACE_ADDRESS) || ((unsigned int) Pointer_2 < KERNEL_USER_SPACE_ADDRESS)) asm("int 13"); // This interrupt will never return and will abort the current system call
	
	// Execute requested call
	System_Calls_Handlers[Call_Code]();
	return Return_Value;
}