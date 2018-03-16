OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)
ENTRY(KernelEntryPoint)
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
	.bss :
	{
		. = ALIGN(4);  /* Make sure that the BSS section size is a multiple of 4 bytes */
		Configuration_Memory_BSS_Start_Address = .; /* Allow a program startup code to clear the section starting from this address */
		*(.bss)
		. = ALIGN(4);
		Configuration_Memory_BSS_End_Address = .;
	}
	. = 0x100000;
	Configuration_Memory_Userspace_Start_Address = .;
}
