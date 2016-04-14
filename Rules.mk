# Default commands
AS = nasm
CC = gcc
LD = ld

# Use the Pentium 1 as default value
ifeq ($(GLOBAL_PROCESSOR_TYPE),)
	GLOBAL_PROCESSOR_TYPE = pentium
endif

# Default gcc flags
# Option -fno-asynchronous-unwind-tables avoid generating the .eh_frame section, which is heavy and useless here
CCFLAGS = -nostdlib -fno-builtin -nostartfiles -finline-functions-called-once -fno-asynchronous-unwind-tables -masm=intel -m32 -march=$(GLOBAL_PROCESSOR_TYPE) -mtune=$(GLOBAL_PROCESSOR_TYPE)