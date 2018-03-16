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
/** The kernel code segment index. */
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_CODE 1
/** The kernel data segment index. */
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_DATA 2
/** The kernel stack segment index. */
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_STACK 3
/** The user space code segment index. */
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_CODE 4
/** The user space data segment index. */
#define ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_DATA 5
/** The task state segment index (used to switch from user space to kernel space and vice versa). */
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
typedef struct __attribute__((packed))
{
	unsigned short Limit_Low; //!< Segment size, bits 15..0 (value is multiplied by 4096 because granularity flag is set).
	unsigned short Base_Address_Low; //!< Segment base address, bits 15..0.
	unsigned char Base_Address_Middle; //!< Segment base address bits 23..16.
	unsigned char Flags; //!< Segment flags (segment type, descriptor type, descriptor privilege level and segment-present flag).
	unsigned char Limit_High: 4; //!< Segment size, bits 19..16.
	unsigned char Other: 4; //!< Some more flags (like granularity flag).
	unsigned char Base_Address_High; //!< Segment base address bits 31..24.
} TArchitectureSegmentDescriptor;

/** The GDTR content. */
typedef struct __attribute__((packed))
{
	unsigned short Table_Size; //!< The Global Descriptor Table size in bytes minus one (as requested by Intel datasheet).
	TArchitectureSegmentDescriptor *Pointer_Table; //!< The Global Descriptor Table base address.
} TArchitectureGlobalDescriptorTableRegister;

/** An IDT segment descriptor. */
typedef struct __attribute__((packed))
{
	unsigned short Handler_Address_Low; //!< Low 16 bits of the handling function address.
	unsigned short Segment_Selector; //!< The index of the code segment that will be used to handle the interrupt. The index value must be shifted by 3 to the left.
	unsigned short Flags; //!< Interrupt configuration (mainly execution level).
	unsigned short Handler_Address_High; //!< High 16 bits of the handling function address.
} TArchitectureInterruptDescriptor;

/** The IDTR content. */
typedef struct __attribute__((packed))
{
	unsigned short Table_Size; //!< The Interrupt Descriptor Table size in bytes minus one (as requested by Intel datasheet).
	TArchitectureInterruptDescriptor *Pointer_Table; //!< The Interrupt Descriptor Table base address.
} TArchitectureInterruptDescriptorTableRegister;

/** A TSS descriptor. */
typedef struct __attribute__((packed))
{
	unsigned short Previous_Task; //!< Previous task TSS's segment descriptor index (not used here as there is only a single task).
	unsigned short Reserved_0; //!< Padding.
	unsigned int ESP0; //!< Task ESP register value for privilege level 0 prior task switching.
	unsigned short SS0; //!< Task SS register value for privilege level 0 prior task switching.
	unsigned short Reserved_1; //!< Padding.
	unsigned int ESP1; //!< Task ESP register value for privilege level 1 prior task switching.
	unsigned short SS1; //!< Task SS register value for privilege level 1 prior task switching.
	unsigned short Reserved_2; //!< Padding.
	unsigned int ESP2; //!< Task ESP register value for privilege level 2 prior task switching.
	unsigned short SS2; //!< Task SS register value for privilege level 1 prior task switching.
	unsigned short Reserved_3; //!< Padding.
	unsigned int CR3; //!< Base physical address of the task page directory (not used here as paging is not needed).
	unsigned int EIP; //!< The instruction the task was executing prior task switching.
	unsigned int EFLAGS; //!< CPU state prior task switching.
	unsigned int EAX; //!< EAX register value prior task switching.
	unsigned int ECX; //!< ECX register value prior task switching.
	unsigned int EDX; //!< EDX register value prior task switching.
	unsigned int EBX; //!< EBX register value prior task switching.
	unsigned int ESP; //!< ESP register value prior task switching.
	unsigned int EBP; //!< EBP register value prior task switching.
	unsigned int ESI; //!< ESI register value prior task switching.
	unsigned int EDI; //!< EDI register value prior task switching.
	unsigned short ES; //!< ES register value prior task switching.
	unsigned short Reserved_5; //!< Padding.
	unsigned short CS; //!< CS register value prior task switching.
	unsigned short Reserved_6; //!< Padding.
	unsigned short SS; //!< SS register value prior task switching.
	unsigned short Reserved_7; //!< Padding.
	unsigned short DS; //!< DS register value prior task switching.
	unsigned short Reserved_8; //!< Padding.
	unsigned short FS; //!< FS register value prior task switching.
	unsigned short Reserved_9; //!< Padding.
	unsigned short GS; //!< GS register value prior task switching.
	unsigned short Reserved_10; //!< Padding.
	unsigned short LDT_Segment_Selector; //!< Task LDT segment selector index (not used here as the GDT is more than enough for the few segments needed by the system).
	unsigned short Reserved_11; //!< Padding.
	unsigned short Debug_Flag; //!< Hold the debug trap flag.
	unsigned short IO_Map_Base_Address; //!< Offset from the TSS base address where to locate the I/O bitmask (not used here as user space IOPL flag value forbids any I/O access).
} TArchitectureTaskStateSegment;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** The GDT. */
static volatile TArchitectureSegmentDescriptor __attribute__((aligned(8))) Architecture_Global_Descriptor_Table[256]; // Should give better performances by aligning on cache boundary
/** The GDTR content. */
static volatile TArchitectureGlobalDescriptorTableRegister Architecture_Global_Descriptor_Table_Register;

/** The IDT. */
static volatile TArchitectureInterruptDescriptor __attribute__((aligned(8))) Architecture_Interrupt_Descriptor_Table[256]; // Should give better performances by aligning on cache boundary
/** The IDTR content. */
static volatile TArchitectureInterruptDescriptorTableRegister Architecture_Interrupt_Descriptor_Table_Register;

/** The TSS involved in context switching. */
static volatile TArchitectureTaskStateSegment Architecture_Kernel_Task_State_Segment;

//-------------------------------------------------------------------------------------------------
// Function prototypes
//-------------------------------------------------------------------------------------------------
// The following prototypes are mandatory because the corresponding assembly labels can't be seen by the C compiler
// Moreover, this functions can't be static as they must be linked with the assembly code
/** Called when an user space application tries to execute a division instruction with a null divisor. */
void ArchitectureInterruptLauncherDivisionErrorException(void);
/** Called when an user space application tries to execute a privileged instruction or to access to a forbidden memory area. */
void ArchitectureInterruptLauncherGeneralProtectionFaultException(void);
/** Called when the kernel stack overflows or underflows. */
void ArchitectureInterruptLauncherStackException(void);
/** Called when the timer triggers an interrupt. */
void ArchitectureInterruptLauncherTimer(void);
/** Called when the keyboard trigger an interrupt. */
void ArchitectureInterruptLauncherKeyboard(void);
/** Called when an user space application performs a system call. */
void ArchitectureInterruptLauncherSystemCalls(void);
/** Notify the interrupts controller that the interrupt has been handled.
 * @warning Use this function only for an interrupt coming from an hardware device.
 */
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
	Architecture_Global_Descriptor_Table[Index].Base_Address_Low = Base_Address;
	Architecture_Global_Descriptor_Table[Index].Base_Address_Middle = Base_Address >> 16;
	Architecture_Global_Descriptor_Table[Index].Base_Address_High = Base_Address >> 24;
	// Limit
	Architecture_Global_Descriptor_Table[Index].Limit_Low = Limit;
	Architecture_Global_Descriptor_Table[Index].Limit_High = Limit >> 16;
	// Flags
	Architecture_Global_Descriptor_Table[Index].Flags = Flags;
	// Other bits
	Architecture_Global_Descriptor_Table[Index].Other = Other;
}

/** Add an interrupt descriptor to the Interrupt Descriptor Table.
 * @param Index The segment index in the IDT.
 * @param Is_User_Accessible Tell if an userspace program can directly call this interrupt handler or not.
 * @param Pointer_Function_Address The interrupt launcher address.
 */
static void ArchitectureMemoryProtectionAddInterruptDescriptor(unsigned int Index, int Is_User_Accessible, void (*Pointer_Function_Address))
{
	// Register handler function's address
	Architecture_Interrupt_Descriptor_Table[Index].Handler_Address_Low = (unsigned int) Pointer_Function_Address;
	Architecture_Interrupt_Descriptor_Table[Index].Handler_Address_High = (unsigned int) Pointer_Function_Address >> 16;
	// Descriptor type 
	if (Is_User_Accessible) Architecture_Interrupt_Descriptor_Table[Index].Flags = 0xEE00; // 1110 1110 0000 0000b (security level 3)
	else Architecture_Interrupt_Descriptor_Table[Index].Flags = 0x8E00; // 1000 1110 0000 0000b (security level 0)
	// Segment descriptor's number in the GDT
	Architecture_Interrupt_Descriptor_Table[Index].Segment_Selector = ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_CODE << 3;
}

/** Initialize the GDT. */
static inline __attribute__((always_inline)) void ArchitectureInitializeGlobalDescriptorTable(void)
{
	unsigned int User_Space_Size_Pages;
	
	// Create the dummy descriptor (descriptor 0 filled with 0 to show that the table is valid)
	memset((void *) Architecture_Global_Descriptor_Table, 0, sizeof(TArchitectureSegmentDescriptor)); // Explicit cast to avoid warning due to pointer volatile attribute
	
	// Add system segments
	ArchitectureMemoryProtectionAddSegmentDescriptor(ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_CODE, 0, 0xFFFFF, ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_KERNEL_CODE, 0x0C);
	ArchitectureMemoryProtectionAddSegmentDescriptor(ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_DATA, 0, 0xFFFFF, ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_KERNEL_DATA, 0x0C);
	ArchitectureMemoryProtectionAddSegmentDescriptor(ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_STACK, 0, 0, ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_KERNEL_STACK, 0x0C);
	
	// Add user task segments
	User_Space_Size_Pages = CONFIGURATION_USER_SPACE_SIZE >> 12; // Compute the userspace size in 4096-byte tables
	if (CONFIGURATION_USER_SPACE_SIZE % 4096 != 0) User_Space_Size_Pages++; // Add one more page if the user space size is not a multiple of the page size
	ArchitectureMemoryProtectionAddSegmentDescriptor(ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_CODE, CONFIGURATION_USER_SPACE_ADDRESS, User_Space_Size_Pages, ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_USER_CODE, 0x0C); // The limit size must be expressed in 4096-byte pages, 0x0C is for 32-bit instructions and to enable 4KB granularity
	ArchitectureMemoryProtectionAddSegmentDescriptor(ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_DATA, CONFIGURATION_USER_SPACE_ADDRESS, User_Space_Size_Pages, ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_USER_DATA, 0x0C);
	
	// Add kernel TSS descriptor
	ArchitectureMemoryProtectionAddSegmentDescriptor(ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_TASK_STATE_SEGMENT, (unsigned int) &Architecture_Kernel_Task_State_Segment, sizeof(TArchitectureTaskStateSegment) - 1, ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_TYPE_TASK_STATE_SEGMENT, 0);
	
	// Load the GDT in memory
	Architecture_Global_Descriptor_Table_Register.Table_Size = sizeof(Architecture_Global_Descriptor_Table) - 1; // As told by Intel datasheet
	Architecture_Global_Descriptor_Table_Register.Pointer_Table = (TArchitectureSegmentDescriptor *) Architecture_Global_Descriptor_Table;  // Explicit cast to avoid warning due to pointer volatile attribute
	asm("lgdt [Architecture_Global_Descriptor_Table_Register]");
	
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
	Architecture_Interrupt_Descriptor_Table_Register.Table_Size = sizeof(Architecture_Interrupt_Descriptor_Table) - 1; // As told by Intel datasheet
	Architecture_Interrupt_Descriptor_Table_Register.Pointer_Table = (TArchitectureInterruptDescriptor *) Architecture_Interrupt_Descriptor_Table; // Explicit cast to avoid warning due to pointer volatile attribute
	asm("lidt [Architecture_Interrupt_Descriptor_Table_Register]");
}

/** Initialize the kernel Task State Segment, needed by hardware protection when switching from protection levels. */
static inline __attribute__((always_inline)) void ArchitectureInitializeTaskStateSegment(void)
{
	Architecture_Kernel_Task_State_Segment.Debug_Flag = 0; // Not in debug mode
	Architecture_Kernel_Task_State_Segment.IO_Map_Base_Address = 0;
	Architecture_Kernel_Task_State_Segment.ESP0 = CONFIGURATION_KERNEL_STACK_ADDRESS;
	Architecture_Kernel_Task_State_Segment.SS0 = ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_KERNEL_STACK << 3; // Corresponding segment descriptor offset in GDT

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
		"mov fs, ax\n"
		"mov gs, ax\n"
		"iret" // Go to user space (automatically re-enable interrupts)
		: // No output parameters
		: "i" ((ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_DATA << 3) | 3), "X" (CONFIGURATION_USER_SPACE_SIZE), "i" ((ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_CODE << 3) | 3), "i" (CONFIGURATION_USER_SPACE_PROGRAM_ENTRY_POINT), "i" ((ARCHITECTURE_MEMORY_PROTECTION_SEGMENT_INDEX_USER_DATA << 3) | 3)
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
