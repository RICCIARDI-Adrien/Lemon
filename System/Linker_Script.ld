OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)
ENTRY(KernelEntryPoint)
FORCE_COMMON_ALLOCATION /* Make sure objects marked "COMMON" are allocated in the specified section */
KERNEL_ADDRESS = 0x10000;
SECTIONS
{
	. = KERNEL_ADDRESS;
	.init :
	{
		*(.init)
	}
	.text :
	{
		*(.text)
	}
	.rodata :
	{
		*(.rodata)
	}
	.data :
	{
		*(.data)
	}
	
	. = 0x100000; /* Bypass low memory reserved for video RAM and BIOS */
	.bss ALIGN(4) : /* Make sure that the BSS section size is a multiple of 4 bytes */
	{
		Configuration_Memory_BSS_Start_Address = .; /* Allow a program startup code to clear the section starting from this address */
		*(.bss)
		Configuration_Memory_BSS_End_Address = .;
		*(COMMON) /* Refer to all uninitialized shared variables, for instance the file system one */
	}
	. = ALIGN(4096); /* Start at the beginning of a MMU page to ease automatic MMU pages count computation */
	Configuration_Memory_User_Space_Start_Address = .;
}
