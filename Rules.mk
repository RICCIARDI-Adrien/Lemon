# Default commands
AS = nasm
CC = gcc
LD = ld

# Default gcc flags
CCFLAGS = -nostdlib -fno-builtin -nostartfiles -finline-functions-called-once -fno-asynchronous-unwind-tables -masm=intel -m32 -march=pentium -mtune=pentium