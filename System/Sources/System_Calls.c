/** @file System_Calls.c
 * @see System_Calls.h for description.
 * @author Adrien RICCIARDI
 */
#include <Drivers/Driver_Keyboard.h>
#include <Drivers/Driver_Screen.h>
#include <Drivers/Driver_Timer.h>
#include <Drivers/Driver_UART.h>
#include <File_System/File.h>
#include <Kernel.h>
#include <System_Calls.h>

//-------------------------------------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------------------------------------
// System call parameters
static TSystemCall Call_Code;
static int Integer_1, Integer_2, Return_Value;
static void *Pointer_1, *Pointer_2;

//====================================================================================================================
// Kernel / system calls
//====================================================================================================================
void SystemCallSystemGetUserMemorySize(void)
{
	Return_Value = KERNEL_USER_SPACE_SIZE;
}

void SystemCallTimerReadValue(void)
{
	Return_Value = Timer_Counter;
}

void SystemCallTimerWait(void)
{
	TimerWait(Integer_1);
}

void SystemCallUARTInit(void)
{
	Return_Value = UARTInit(Integer_1, Integer_2);
}

void SystemCallUARTReadByte(void)
{
	Return_Value = UARTReadByte();
}

void SystemCallUARTWriteByte(void)
{
	UARTWriteByte(Integer_1);
}

void SystemCallUARTIsDataReceived(void)
{
	Return_Value = UARTIsDataReceived();
}

//====================================================================================================================
// Screen calls
//====================================================================================================================
void SystemCallScreenWriteCharacter(void)
{
	ScreenWriteChar((char) Integer_1);
}

void SystemCallScreenWriteString(void)
{
	ScreenWriteString(Pointer_1);
}

void SystemCallScreenGetColor(void)
{
	Return_Value = ScreenGetColor();
}

void SystemCallScreenSetColor(void)
{
	ScreenSetColor(Integer_1);
}

void SystemCallScreenGetCursorPosition(void)
{
	ScreenGetCursorPosition(Pointer_1, Pointer_2);
}

void SystemCallScreenSetCursorPosition(void)
{
	ScreenSetCursorPosition(Integer_1, Integer_2);
}

void SystemCallScreenDisplayBuffer(void)
{
	ScreenDisplayBuffer(Pointer_1);
}

//====================================================================================================================
// Keyboard calls
//====================================================================================================================
void SystemCallKeyboardReadCharacter(void)
{
	asm("sti");
	Return_Value = KeyboardReadChar();
}

void SystemCallKeyboardIsKeyAvailable(void)
{
	Return_Value = KeyboardIsKeyAvailable();
}

//====================================================================================================================
// File system calls
//====================================================================================================================
void SystemCallFileExists(void)
{
	Return_Value = FileExists(Pointer_1);
}

void SystemCallFileSize(void)
{
	Return_Value = FileSize(Pointer_1);
}

void SystemCallFileListNext(void)
{
	FileListNext(Pointer_1);
}

void SystemCallFileDelete(void)
{
	Return_Value = FileDelete(Pointer_1);
}

void SystemCallFileRename(void)
{
	Return_Value = FileRename(Pointer_1, Pointer_2);
}

void SystemCallFileOpen(void)
{
	Return_Value = FileOpen(Pointer_1, (char) Integer_1, Pointer_2);
}

void SystemCallFileRead(void)
{
	Return_Value = FileRead((unsigned int) Integer_1, Pointer_1, (unsigned int) Integer_2, Pointer_2); 
}

void SystemCallFileWrite(void)
{
	Return_Value = FileWrite((unsigned int) Integer_1, Pointer_1, (unsigned int) Integer_2);
}

void SystemCallFileClose(void)
{
	FileClose(Integer_1);
}

//====================================================================================================================
// Jump table
//====================================================================================================================
TSystemCallHandler System_Calls_Handlers[] =
{
	KernelStartShell, // SYSTEM_CALL_SYSTEM_EXIT_PROGRAM
	SystemCallSystemGetUserMemorySize, // SYSTEM_CALL_SYSTEM_GET_USER_MEMORY_SIZE
	SystemCallTimerReadValue, // SYSTEM_CALL_TIMER_READ_VALUE
	SystemCallTimerWait, // SYSTEM_CALL_TIMER_WAIT
	SystemCallUARTInit, // SYSTEM_CALL_UART_INIT
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
	if (Call_Code >= SYSTEM_CALLS_COUNT) return -1; // Arbitrary value
	
	// Adjust string pointers according to user segment base into GDT
	Pointer_1 += KERNEL_USER_SPACE_ADDRESS;
	Pointer_2 += KERNEL_USER_SPACE_ADDRESS;
		
	// Execute requested call
	System_Calls_Handlers[Call_Code]();
	return Return_Value;
}