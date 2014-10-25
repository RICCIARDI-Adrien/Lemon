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
		*(.bss)
	}
}
