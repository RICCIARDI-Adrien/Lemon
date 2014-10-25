/** @file System_Program_Entry_Point.c
 * This code is executed before the program main() function.
 * @author Adrien RICCIARDI
 * @version 1.0 : 11/12/2011
 * @version 1.1 : 27/04/2012
 * @version 1.2 : 25/12/2012, added argc and argv[] support.
 * @version 1.3 : 22/08/2013, optimized SystemCall() code.
 * @version 1.4 : 13/06/2014, added errno initialization.
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
void  __attribute__((section(".init"))) _start(void)
{
	int Return_Value;
	TCommandLineArguments *Pointer_Command_Line_Arguments = (TCommandLineArguments *) 0; // Located at the beginning of the user space
	
	Return_Value = main(Pointer_Command_Line_Arguments->Arguments_Count, Pointer_Command_Line_Arguments->Pointer_Arguments); 
	
	// Return main() return value to the operating system
	SystemCall(SYSTEM_CALL_SYSTEM_EXIT_PROGRAM, Return_Value, 0, NULL, NULL);
}

int SystemCall(TSystemCall Request_Code, int Integer_1, int Integer_2, void *Pointer_1, void *Pointer_2)
{
	asm
	(
		"push ebx\n"
		"push ecx\n"
		"push edx\n"
		"push esi\n"
		"mov eax, %0\n"
		"mov ebx, %1\n"
		"mov ecx, %2\n"
		"mov edx, %3\n"
		"mov esi, %4\n"
		"int 0x60\n"
		"pop esi\n"
		"pop edx\n"
		"pop ecx\n"
		"pop ebx\n"
		// Exit from function to avoid moving eax to memory and memory to eax again as gcc would like to do
		"leave\n"
		"ret"
		: // No output parameter
		: "m" (Request_Code), "m" (Integer_1), "m" (Integer_2), "m" (Pointer_1), "m" (Pointer_2)
		: "eax", "ebx", "ecx", "edx", "esi"
	);
	
	// The assembly code does not return yet, it's just to make gcc happy
	return 0;
}
