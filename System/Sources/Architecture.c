/** @file Architecture.c
 * @see Architecture.h for description.
 * @author Adrien RICCIARDI
 */
#include <Architecture.h>
#include <Configuration.h>
#include <Drivers/Driver_Keyboard.h> // To have the keyboard interrupt handler
#include <Drivers/Driver_PIC.h> // To have the PIC acknowledge interrupt macro
#include <Drivers/Driver_Timer.h> // To have the timer interrupt handler
#include <Kernel.h>
#include <Standard_Functions.h> // To have memset()
#include <System_Calls.h> // To have the system calls interrupt handler

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Code exec-read, accessed, protection level 0. */
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_KERNEL_CODE 0x9B
/** Data read-write, accessed, protection level 0. */
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_KERNEL_DATA 0x93
/** Data read-write, expand down, protection level 0. */
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_KERNEL_STACK 0x97
/** Code exec-read, accessed, protection level 3. */
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_USER_CODE 0xFB
/** Data read-write, accessed, protection level 3. */
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_USER_DATA 0xF3
/** Segment is present, protection level 3. */
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_TASK_STATE_SEGMENT 0xE9

// Segment indexes in GDT
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_CODE 1
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_DATA 2
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_STACK 3
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_CODE 4
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_DATA 5
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_TASK_STATE_SEGMENT 6

/** Save all user registers onto kernel stack and switch to kernel data segment. */
#define ARCHITECTURE_SAVE_USER_REGISTERS() \
	asm \
	( \
		"pushad\n" \
		"push ds\n" \
		"push es\n" \
		"push fs\n" \
		"push gs\n" \
		"mov ax, %0\n" /* Point to kernel data segment descriptor */ \
		"mov ds, ax\n" \
		"mov es, ax\n" \
		: /* No output parameter */ \
		: "i" (ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_DATA << 3) \
		: "eax" \
	)
									 
/** Restore all previously saved user registers to the kernel stack. */
#define ARCHITECTURE_RESTORE_USER_REGISTERS() \
	asm \
	( \
		"pop gs\n" \
		"pop fs\n" \
		"pop es\n" \
		"pop ds\n" \
		"popad" \
	)

/** Switch to the kernel data segment, so the kernel can access its own data. */
#define ARCHITECTURE_SWITCH_TO_KERNEL_DATA_SEGMENT() \
	asm \
	( \
		"mov ax, %0\n" /* Point to user segment descriptor */ \
		"mov ds, ax\n" \
		"mov es, ax\n" \
		: /* No output parameter */ \
		: "i" (ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_DATA * 8) \
		: "eax" \
	)

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A GDT segment descriptor. */
struct TSegmentDescriptor
{
	unsigned short Limit_Low, Base_Address_Low;
	unsigned char Base_Address_Middle, Flags;
	unsigned char Limit_High: 4;
	unsigned char Other: 4;
	unsigned char Base_Address_High;
} __attribute__((packed));

/** The GDTR content. */
struct TGlobalDescriptorTableRegister
{
	unsigned short Table_Size;
	struct TSegmentDescriptor *Pointer_Table;
} __attribute__((packed));

/** An IDT segment descriptor. */
struct TInterruptDescriptor
{
	unsigned short Handler_Address_Low, Segment_Selector, Flags, Handler_Address_High;
} __attribute__((packed));

/** The IDTR content. */
struct TInterruptDescriptorTableRegister
{
	unsigned short Table_Size;
	struct TInterruptDescriptor *Pointer_Table;
} __attribute__((packed));

/** A TSS descriptor. */
struct TTaskStateSegment
{
	unsigned short Previous_Task, Reserved_0;
	unsigned int ESP0;
	unsigned short SS0, Reserved_1;
	unsigned int ESP1;
	unsigned short SS1, Reserved_2;
	unsigned int ESP2;
	unsigned short SS2, Reserved_3;
	unsigned int CR3;
	unsigned int EIP;
	unsigned int EFLAGS;
	unsigned int EAX;
	unsigned int ECX;
	unsigned int EDX;
	unsigned int EBX;
	unsigned int ESP;
	unsigned int EBP;
	unsigned int ESI;
	unsigned int EDI;
	unsigned short ES, Reserved_5;
	unsigned short CS, Reserved_6;
	unsigned short SS, Reserved_7;
	unsigned short DS, Reserved_8;
	unsigned short FS, Reserved_9;
	unsigned short GS, Reserved_10;
	unsigned short LDT_Segment_Selector, Reserved_11;
	unsigned short Debug_Flag, IO_Map_Base_Address;
} __attribute__((packed));

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The GDT. */
static volatile struct TSegmentDescriptor Global_Descriptor_Table[256] __attribute__((aligned(8))); // Should give better performances by aligning on cache boundary
/** The GDTR content. */
static volatile struct TGlobalDescriptorTableRegister Global_Descriptor_Table_Register;

/** The IDT. */
static volatile struct TInterruptDescriptor Interrupt_Descriptor_Table[256] __attribute__((aligned(8))); // Should give better performances by aligning on cache boundary
/** The IDTR content. */
static volatile struct TInterruptDescriptorTableRegister Interrupt_Descriptor_Table_Register;

/** The TSS involved in context switching. */
static volatile struct TTaskStateSegment Kernel_Task_State_Segment;

//-------------------------------------------------------------------------------------------------
// Function prototypes
//-------------------------------------------------------------------------------------------------
// The following prototypes are mandatory because the corresponding assembly labels can't be seen by the C compiler
// Moreover, this functions can't be static as they must be linked with the assembly code
void ArchitectureInterruptLauncherDivisionErrorException(void);
void ArchitectureInterruptLauncherGeneralProtectionFaultException(void);
void ArchitectureInterruptLauncherStackException(void);
void ArchitectureInterruptLauncherTimer(void);
void ArchitectureInterruptLauncherKeyboard(void);
void ArchitectureInterruptLauncherSystemCalls(void);
void ArchitectureInterruptExit(void);

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Add a segment descriptor to the Global Descriptor Table.
 * @param Index The segment index in the GDT.
 * @param Base_Address The segment base address in bytes.
 * @param Limit The segment limit in multiples of 4096.
 * @param Flags Segment Type, Descriptor Privilege Level and other segment-specific flags.
 * @param Other Granularity flag and other segment-specific flags.
 */
static void ArchitectureMemoryProtectionAddSegmentDescriptor(unsigned int Index, unsigned int Base_Address, unsigned int Limit, unsigned char Flags, unsigned char Other)
{
	// Base address
	Global_Descriptor_Table[Index].Base_Address_Low = Base_Address;
	Global_Descriptor_Table[Index].Base_Address_Middle = Base_Address >> 16;
	Global_Descriptor_Table[Index].Base_Address_High = Base_Address >> 24;
	// Limit
	Global_Descriptor_Table[Index].Limit_Low = Limit;
	Global_Descriptor_Table[Index].Limit_High = Limit >> 16;
	// Flags
	Global_Descriptor_Table[Index].Flags = Flags;
	// Other bits
	Global_Descriptor_Table[Index].Other = Other;
}

/** Add an interrupt descriptor to the Interrupt Descriptor Table.
 * @param Index The segment index in the IDT.
 * @param Is_User_Accessible Tell if an userspace program can directly call this interrupt handler or not.
 * @param Pointer_Function_Address The interrupt launcher address.
 */
static void ArchitectureMemoryProtectionAddInterruptDescriptor(unsigned int Index, int Is_User_Accessible, void (*Pointer_Function_Address))
{
	// Register handler function's address
	Interrupt_Descriptor_Table[Index].Handler_Address_Low = (unsigned int) Pointer_Function_Address;
	Interrupt_Descriptor_Table[Index].Handler_Address_High = (unsigned int) Pointer_Function_Address >> 16;
	// Descriptor type 
	if (Is_User_Accessible) Interrupt_Descriptor_Table[Index].Flags = 0xEE00; // 1110 1110 0000 0000b (security level 3)
	else Interrupt_Descriptor_Table[Index].Flags = 0x8E00; // 1000 1110 0000 0000b (security level 0)
	// Segment descriptor's number in the GDT
	Interrupt_Descriptor_Table[Index].Segment_Selector = ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_CODE << 3;
}

/** Initialize the GDT. */
static inline __attribute__((always_inline)) void ArchitectureInitializeGlobalDescriptorTable(void)
{
	// Create the dummy descriptor (descriptor 0 filled with 0 to show that the table is valid)
	memset((void *) Global_Descriptor_Table, 0, sizeof(struct TSegmentDescriptor)); // Explicit cast to avoid warning due to pointer volatile attribute
	
	// Add system segments
	ArchitectureMemoryProtectionAddSegmentDescriptor(ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_CODE, 0, 0xFFFFF, ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_KERNEL_CODE, 0x0C);
	ArchitectureMemoryProtectionAddSegmentDescriptor(ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_DATA, 0, 0xFFFFF, ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_KERNEL_DATA, 0x0C);
	ArchitectureMemoryProtectionAddSegmentDescriptor(ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_STACK, 0, 0, ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_KERNEL_STACK, 0x0C);
	
	// Add user's task segments
	ArchitectureMemoryProtectionAddSegmentDescriptor(ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_CODE, KERNEL_USER_SPACE_ADDRESS, KERNEL_USER_SPACE_SIZE >> 12, ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_USER_CODE, 0x0C); // The limit size must be expressed in 4096-byte pages, 0x0C is for 32-bit instructions and to enable 4KB granularity
	ArchitectureMemoryProtectionAddSegmentDescriptor(ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_DATA, KERNEL_USER_SPACE_ADDRESS, KERNEL_USER_SPACE_SIZE >> 12, ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_USER_DATA, 0x0C);
	
	// Add kernel TSS descriptor
	ArchitectureMemoryProtectionAddSegmentDescriptor(ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_TASK_STATE_SEGMENT, (unsigned int) &Kernel_Task_State_Segment, sizeof(struct TTaskStateSegment) - 1, ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_TASK_STATE_SEGMENT, 0);
	
	// Load the GDT in memory
	Global_Descriptor_Table_Register.Table_Size = sizeof(Global_Descriptor_Table) - 1; // As told by Intel datasheet
	Global_Descriptor_Table_Register.Pointer_Table = (struct TSegmentDescriptor *) Global_Descriptor_Table;  // Explicit cast to avoid warning due to pointer volatile attribute
	asm("lgdt [Global_Descriptor_Table_Register]");
	
	// Reset kernel data segment selectors
	asm
	(
		"mov ax, %0\n"
		"mov ds, ax\n"
		"mov es, ax\n"
		"mov fs, ax\n" // fs and gs MUST be initialized or the Pentium (and above) MMUs will generate a General Protection Fault
		"mov gs, ax\n"
		"jmp %1, Reset_CS\n"
	"Reset_CS:"
		: // No output parameter
		: "i" (ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_DATA << 3), "i" (ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_CODE << 3)
		: "ax"
	);
}

/** Initialize the IDT. */
static inline __attribute__((always_inline)) void ArchitectureInitializeInterruptDescriptorTable(void)
{
	int i;
	
	// Fill all interrupt handlers with the NULL interrupt
	for (i = 0; i < 256; i++) ArchitectureMemoryProtectionAddInterruptDescriptor(i, 0, ArchitectureInterruptExit);
	
	// Add descriptors
	ArchitectureMemoryProtectionAddInterruptDescriptor(0, 0, ArchitectureInterruptLauncherDivisionErrorException);
	ArchitectureMemoryProtectionAddInterruptDescriptor(12, 0, ArchitectureInterruptLauncherStackException);
	ArchitectureMemoryProtectionAddInterruptDescriptor(13, 0, ArchitectureInterruptLauncherGeneralProtectionFaultException);
	ArchitectureMemoryProtectionAddInterruptDescriptor(32, 0, ArchitectureInterruptLauncherTimer);
	ArchitectureMemoryProtectionAddInterruptDescriptor(33, 0, ArchitectureInterruptLauncherKeyboard);
	ArchitectureMemoryProtectionAddInterruptDescriptor(0x60, 1, ArchitectureInterruptLauncherSystemCalls); // Must be accessible from user mode
	
	// Load the IDT in the memory
	Interrupt_Descriptor_Table_Register.Table_Size = sizeof(Interrupt_Descriptor_Table) - 1; // As told by Intel datasheet
	Interrupt_Descriptor_Table_Register.Pointer_Table = (struct TInterruptDescriptor *) Interrupt_Descriptor_Table; // Explicit cast to avoid warning due to pointer volatile attribute
	asm("lidt [Interrupt_Descriptor_Table_Register]");
}

/** Initialize the kernel Task State Segment, needed by hardware protection when switching from protection levels. */
static inline __attribute__((always_inline)) void ArchitectureInitializeTaskStateSegment(void)
{
	Kernel_Task_State_Segment.Debug_Flag = 0; // Not in debug mode
	Kernel_Task_State_Segment.IO_Map_Base_Address = 0;
	Kernel_Task_State_Segment.ESP0 = KERNEL_STACK_ADDRESS;
	Kernel_Task_State_Segment.SS0 = ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_STACK << 3; // Corresponding segment descriptor offset in GDT

	// Load TSS into memory
	asm
	(
		"mov ax, %0\n"
		"ltr ax"
		: // No output parameters
		: "i" (ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_TASK_STATE_SEGMENT << 3) // Segment Selector Index = ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_TASK_STATE_SEGMENT ; Table Indicator = 0 (segment is in GDT) ; Requested Privilege Level = 0
		: "ax"
	);
}

/** Contain all interrupt handler launchers (a valid scope must be provided in order to use gcc custom assembly). */
static void __attribute__((used)) ArchitectureInterruptLaunchers(void)
{
	// Kernel system calls
	asm
	(
	"ArchitectureInterruptLauncherSystemCalls:\n"
		// Save segment descriptors manually, we can't use the macro because we would have to restore eax which is the interrupt result
		"push ds\n"
		"push es\n"
		"push edi\n" // This is not a system call parameter, so it must be preserved
		
		// Switch to kernel data segment
		"mov edi, eax\n" // Keep eax content faster than pushing it on the stack
		"mov ax, %0\n"
		"mov ds, ax\n"
		"mov es, ax\n" // Needed by Intel string instructions or a GPF exception will be generated when returning from user mode
		"mov eax, edi\n"
		"call SystemCalls\n" // The handler is contained in the System_Calls.c file

		// Restore descriptor segments
		"pop edi\n"
		"pop es\n"
		"pop ds\n"
		"iret"
		: // No output
		: "i" (ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_DATA << 3)
		: "eax", "edi"
	);
	
	// Error handlers
	// Handle Division Error exception
	asm("ArchitectureInterruptLauncherDivisionErrorException:");
	ARCHITECTURE_SWITCH_TO_KERNEL_DATA_SEGMENT();
	asm("call KernelDivisionErrorExceptionInterruptHandler");
	
	// Handle Stack exception
	asm("ArchitectureInterruptLauncherStackException:");
	ARCHITECTURE_SWITCH_TO_KERNEL_DATA_SEGMENT();
	asm("call KernelStackExceptionInterruptHandler");

	// Handle General Protection Fault exception
	asm("ArchitectureInterruptLauncherGeneralProtectionFaultException:");
	ARCHITECTURE_SWITCH_TO_KERNEL_DATA_SEGMENT();
	asm("call KernelGeneralProtectionFaultExceptionInterruptHandler");

	// Devices handlers
	// Timer
	asm("ArchitectureInterruptLauncherTimer:");
	ARCHITECTURE_SAVE_USER_REGISTERS();
	asm("call TimerInterruptHandler"); // The handler is contained in the Drivers/Driver_Timer.c file
	ARCHITECTURE_RESTORE_USER_REGISTERS();
	asm("jmp ArchitectureInterruptExit");

	// Keyboard
	asm("ArchitectureInterruptLauncherKeyboard:");
	ARCHITECTURE_SAVE_USER_REGISTERS();
	asm("call KeyboardInterruptHandler"); // The handler is contained in the Drivers/Driver_Keyboard.c file
	ARCHITECTURE_RESTORE_USER_REGISTERS();
	asm("jmp ArchitectureInterruptExit");
	
	// Default interrupt handler (do nothing)
	asm("ArchitectureInterruptExit:");
	PIC_ACKNOWLEDGE_INTERRUPT();
	asm("iret");
}

//-------------------------------------------------------------------------------------------------
// Public functions
//-------------------------------------------------------------------------------------------------
void ArchitectureInitializeMemoryProtection(void)
{
	ArchitectureInitializeGlobalDescriptorTable();
	ArchitectureInitializeInterruptDescriptorTable();
	ArchitectureInitializeTaskStateSegment();
}

void ArchitectureSwitchToUserSpace(void)
{
	ARCHITECTURE_INTERRUPTS_DISABLE();
	
	// Switch from kernel space to user space using a fake interrupt return
	asm
	(
		"push %0\n" // User stack segment descriptor offset in GDT (same than data segment) OR'ed with ring 3 Requested Privilege Level
		"push %1\n" // Initial stack pointer value (top of user space, so we need to substract kernel segments to total RAM amount)
	
		// Change rights for user space by modifying CPU flags
		"pushf\n" // Get flags
		"pop eax\n"
		"or eax, 0x200\n" // Enable Interrupt Flag bit
		"and eax, 0xFFFF8FFF\n" // Disable Nested Flag bit to disable hardware task switching, disable IOPL bits to disable I/O instructions execution
		"push eax\n" // Push new flags
	
		"push %2\n" // User code segment offset in GDT OR'ed with ring 3 Requested Privilege Level
		"push %3\n" // Instruction pointer
		"mov ax, %4\n" // Initialize Data Segment descriptor with ring 3 Requested Privilege Level
		"mov ds, ax\n"
		"mov es, ax\n" // Needed by Intel string instructions
		"iret" // Go to user space (automatically re-enable interrupts)
		: // No output parameters
		: "i" ((ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_DATA << 3) | 3), "i" (KERNEL_USER_SPACE_SIZE), "i" ((ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_CODE << 3) | 3), "i" (KERNEL_PROGRAM_ENTRY_POINT_USER_ADDRESS), "i" ((ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_DATA << 3) | 3)
		: "eax"
	);
}

void ArchitectureIODelay(void)
{
	asm
	(
		"nop\n"
		"nop\n"
		"nop\n"
	);
}