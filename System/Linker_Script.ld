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
		_bss_start = .; /* Allow a program startup code to clear the section starting from this address */
		*(.bss)
		. = ALIGN(4);
		_bss_end = .;
	}
}
