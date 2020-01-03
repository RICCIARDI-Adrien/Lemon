OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)
ENTRY(_start)
PROGRAM_ADDRESS = 0x100;
SECTIONS
{
	. = PROGRAM_ADDRESS;
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
	
	/* Allow to know the program end from the binary */
	_end = .;
	
	/* Remove some undesired sections that only add weight to generated executable */
	/DISCARD/ :
	{
		*(.note*)
	}
}
