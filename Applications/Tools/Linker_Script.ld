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
		*(.bss)
	}
	
	/* Allow to know the program end from the binary. */
	_end = .;
}
