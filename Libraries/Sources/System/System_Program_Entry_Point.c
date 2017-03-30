/** @file System_Program_Entry_Point.c
 * This code is executed before the program main() function.
 * @author Adrien RICCIARDI
 */
#include <System.h>

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** The maximum number of arguments that can be supplied on the command line. */
#define MAXIMUM_ARGUMENTS_COUNT 20

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** Describe the command line format given to the program. */
typedef struct __attribute__((packed))
{
	int Arguments_Count; //! Contain argc value.
	char *Pointer_Arguments[MAXIMUM_ARGUMENTS_COUNT]; //! The *argv[] representation.
	char Arguments_Value[256 - (sizeof(int) + (sizeof(char *) * MAXIMUM_ARGUMENTS_COUNT))]; //! Hold arguments values. Fill the remaining bytes of the 256-byte area before program beginning.
} TCommandLineArguments;

//-------------------------------------------------------------------------------------------------
// Public variables
//-------------------------------------------------------------------------------------------------
/** The following symbols are given by the application linker script. */
extern unsigned int *_bss_start, *_bss_end;

//-------------------------------------------------------------------------------------------------
// Prototypes
//-------------------------------------------------------------------------------------------------
/** Standard main function.
 * @param argc Program's arguments count.
 * @param argv Program's arguments value.
 * @return What the programmer wants.
 */
int main(int argc, char *argv[]);

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
/** Program entry point. */
void __attribute__((section(".init"))) __attribute__((optimize(0))) _start(void) // Do not optimize this function or the call to main() will be discarded
{
	int Return_Value;
	TCommandLineArguments *Pointer_Command_Line_Arguments = (TCommandLineArguments *) 0; // Located at the beginning of the user space
	
	// Clear the BSS section as gcc expects (use assembly to do that quickly)
	asm volatile
	(
		"push eax\n"
		"push ecx\n"
		"push edi\n"
		
		"xor eax, eax\n"
		"mov edi, %0\n"
		"mov ecx, %1\n"
		"repnz stosd\n" // It is possible to clear 4 bytes at a time as the _bss_* addresses are 4-byte aligned
		
		"pop edi\n"
		"pop ecx\n"
		"pop eax\n"
		: // No output parameter
		: "g" (&_bss_start), "g" (&_bss_end - &_bss_start) // Symbol addresses must be used or gcc will try to dereference the symbol, resulting in a wrong value
		: "eax", "ecx", "edi"
	);
	
	Return_Value = main(Pointer_Command_Line_Arguments->Arguments_Count, Pointer_Command_Line_Arguments->Pointer_Arguments); 
	
	// Return main() return value to the operating system
	SystemCall(SYSTEM_CALL_SYSTEM_EXIT_PROGRAM, Return_Value, 0, NULL, NULL);
}
