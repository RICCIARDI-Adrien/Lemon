# Default commands
AS = nasm
CC = gcc
LD = ld
ISO_GENERATOR = genisoimage

# Use the Pentium 1 as default value
ifeq ($(GLOBAL_PROCESSOR_TYPE),)
	GLOBAL_PROCESSOR_TYPE = pentium
endif

# Choose the whole system and applications language
ifeq ($(GLOBAL_SYSTEM_LANGUAGE),english)
	LANGUAGE_DEFINE = -DCONFIGURATION_LANGUAGE_ENGLISH
else ifeq ($(GLOBAL_SYSTEM_LANGUAGE),italian)
	LANGUAGE_DEFINE = -DCONFIGURATION_LANGUAGE_ITALIAN
endif

# Default gcc flags
# Option -fno-asynchronous-unwind-tables avoid generating the .eh_frame section, which is heavy and useless here
CCFLAGS = -nostdlib -fno-builtin -nostartfiles -finline-functions-called-once -fno-asynchronous-unwind-tables -masm=intel -m32 -Werror -march=$(GLOBAL_PROCESSOR_TYPE) -mtune=$(GLOBAL_PROCESSOR_TYPE) $(LANGUAGE_DEFINE)