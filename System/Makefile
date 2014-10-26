# System makefile.
# There is no need to take dependencies into account as the whole system must be entirely rebuilded every time.
# Author : Adrien RICCIARDI
CC = gcc
LD = ld
AS = nasm
ISO_GENERATOR = genisoimage

SOURCES_PATH = Sources
BINARIES_PATH = Binaries
OBJECTS_PATH = Objects
INCLUDES_PATH = Includes

INCLUDES = -I$(INCLUDES_PATH)

CCFLAGS += -masm=intel -m32 -fno-builtin -finline-functions-called-once -nostdlib -nostartfiles -Wunused -Wall -W $(INCLUDES)
LDFLAGS = -T Linker_Script.ld --strip-all -nostdlib -nostartfile
MBR_FLAGS = -DSECTORS_TO_LOAD_COUNT=64

OBJECTS_CORE = $(OBJECTS_PATH)/Architecture.o $(OBJECTS_PATH)/Hardware_Functions.o $(OBJECTS_PATH)/Kernel.o $(OBJECTS_PATH)/Standard_Functions.o $(OBJECTS_PATH)/System_Calls.o
OBJECTS_DRIVERS = $(OBJECTS_PATH)/Driver_Keyboard.o $(OBJECTS_PATH)/Driver_Hard_Disk.o $(OBJECTS_PATH)/Driver_UART.o $(OBJECTS_PATH)/Driver_Timer.o $(OBJECTS_PATH)/Driver_Screen.o $(OBJECTS_PATH)/Driver_PIC.o
OBJECTS_FILE_SYSTEM = $(OBJECTS_PATH)/File_System.o $(OBJECTS_PATH)/File.o
OBJECTS_SHELL = $(OBJECTS_PATH)/Shell.o $(OBJECTS_PATH)/Shell_Command_List.o $(OBJECTS_PATH)/Shell_Command_Download.o $(OBJECTS_PATH)/Shell_Command_Delete_File.o $(OBJECTS_PATH)/Shell_Command_Rename_File.o $(OBJECTS_PATH)/Shell_Command_File_Size.o $(OBJECTS_PATH)/Shell_Command_Copy_File.o
OBJECTS = $(OBJECTS_CORE) $(OBJECTS_DRIVERS) $(OBJECTS_FILE_SYSTEM) $(OBJECTS_SHELL)

all: clean $(OBJECTS_PATH)/MBR.32b $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(OBJECTS_PATH)/Kernel.32b
	######## Create floppy image ########
	@cat $(OBJECTS_PATH)/MBR.32b $(OBJECTS_PATH)/Kernel.32b > $(BINARIES_PATH)/Raw_Image.bin
	@dd if=/dev/zero of=$(BINARIES_PATH)/Lemon_System_Floppy_Image.img bs=512 count=2880 status=noxfer
	@dd if=$(BINARIES_PATH)/Raw_Image.bin of=$(BINARIES_PATH)/Lemon_System_Floppy_Image.img conv=notrunc status=noxfer
	@# Clean binaries folder of useless files
	@rm $(BINARIES_PATH)/*.bin
	######## Create CDROM image ########
	@if (test -e $(BINARIES_PATH)/Lemon_System_CD_Image.iso); then rm $(BINARIES_PATH)/Lemon_System_CD_Image.iso; fi
	$(ISO_GENERATOR) -r -b Lemon_System_Floppy_Image.img -c boot.catalog -o Lemon_System_CD_Image.iso $(BINARIES_PATH)
	@mv Lemon_System_CD_Image.iso $(BINARIES_PATH)
	#============ Build successful ============#
	@echo -n "Kernel size (bytes) : "
	@ls -l $(OBJECTS_PATH)/Kernel.32b | awk '{print $$5}'

#------------------------------------------------------------------------------------------------------------------------------
# System core
#------------------------------------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/MBR.32b: $(SOURCES_PATH)/MBR.asm
	$(AS) $(MBR_FLAGS) -f bin $(SOURCES_PATH)/MBR.asm -o $(OBJECTS_PATH)/MBR.32b

$(OBJECTS_PATH)/Architecture.o: $(SOURCES_PATH)/Architecture.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Architecture.c -o $(OBJECTS_PATH)/Architecture.o

$(OBJECTS_PATH)/Hardware_Functions.o: $(SOURCES_PATH)/Hardware_Functions.asm
	$(AS) -f elf $(SOURCES_PATH)/Hardware_Functions.asm -o $(OBJECTS_PATH)/Hardware_Functions.o

$(OBJECTS_PATH)/Kernel.o: $(SOURCES_PATH)/Kernel.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Kernel.c -o $(OBJECTS_PATH)/Kernel.o

$(OBJECTS_PATH)/System_Calls.o: $(SOURCES_PATH)/System_Calls.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/System_Calls.c -o $(OBJECTS_PATH)/System_Calls.o

$(OBJECTS_PATH)/Standard_Functions.o: $(SOURCES_PATH)/Standard_Functions.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Standard_Functions.c -o $(OBJECTS_PATH)/Standard_Functions.o

#------------------------------------------------------------------------------------------------------------------------------
# Drivers
#------------------------------------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/Driver_Keyboard.o: $(SOURCES_PATH)/Drivers/Driver_Keyboard.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Drivers/Driver_Keyboard.c -o $(OBJECTS_PATH)/Driver_Keyboard.o
	
$(OBJECTS_PATH)/Driver_Hard_Disk.o: $(SOURCES_PATH)/Drivers/Driver_Hard_Disk.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Drivers/Driver_Hard_Disk.c -o $(OBJECTS_PATH)/Driver_Hard_Disk.o

$(OBJECTS_PATH)/Driver_UART.o: $(SOURCES_PATH)/Drivers/Driver_UART.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Drivers/Driver_UART.c -o $(OBJECTS_PATH)/Driver_UART.o

$(OBJECTS_PATH)/Driver_Timer.o: $(SOURCES_PATH)/Drivers/Driver_Timer.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Drivers/Driver_Timer.c -o $(OBJECTS_PATH)/Driver_Timer.o

$(OBJECTS_PATH)/Driver_Screen.o: $(SOURCES_PATH)/Drivers/Driver_Screen.c
	$(CC) $(CCFLAGS) -c -O2 $(SOURCES_PATH)/Drivers/Driver_Screen.c -o $(OBJECTS_PATH)/Driver_Screen.o

$(OBJECTS_PATH)/Driver_PIC.o: $(SOURCES_PATH)/Drivers/Driver_PIC.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Drivers/Driver_PIC.c -o $(OBJECTS_PATH)/Driver_PIC.o

#------------------------------------------------------------------------------------------------------------------------------
# File system
#------------------------------------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/File_System.o: $(SOURCES_PATH)/File_System/File_System.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/File_System/File_System.c -o $(OBJECTS_PATH)/File_System.o

$(OBJECTS_PATH)/File.o: $(SOURCES_PATH)/File_System/File.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/File_System/File.c -o $(OBJECTS_PATH)/File.o

#------------------------------------------------------------------------------------------------------------------------------
# Shell
#------------------------------------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/Shell.o: $(SOURCES_PATH)/Shell/Shell.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Shell/Shell.c -o $(OBJECTS_PATH)/Shell.o

$(OBJECTS_PATH)/Shell_Command_List.o: $(SOURCES_PATH)/Shell/Shell_Command_List.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Shell/Shell_Command_List.c -o $(OBJECTS_PATH)/Shell_Command_List.o

$(OBJECTS_PATH)/Shell_Command_Download.o: $(SOURCES_PATH)/Shell/Shell_Command_Download.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Shell/Shell_Command_Download.c -o $(OBJECTS_PATH)/Shell_Command_Download.o

$(OBJECTS_PATH)/Shell_Command_Delete_File.o: $(SOURCES_PATH)/Shell/Shell_Command_Delete_File.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Shell/Shell_Command_Delete_File.c -o $(OBJECTS_PATH)/Shell_Command_Delete_File.o

$(OBJECTS_PATH)/Shell_Command_Rename_File.o: $(SOURCES_PATH)/Shell/Shell_Command_Rename_File.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Shell/Shell_Command_Rename_File.c -o $(OBJECTS_PATH)/Shell_Command_Rename_File.o

$(OBJECTS_PATH)/Shell_Command_File_Size.o: $(SOURCES_PATH)/Shell/Shell_Command_File_Size.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Shell/Shell_Command_File_Size.c -o $(OBJECTS_PATH)/Shell_Command_File_Size.o

$(OBJECTS_PATH)/Shell_Command_Copy_File.o: $(SOURCES_PATH)/Shell/Shell_Command_Copy_File.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Shell/Shell_Command_Copy_File.c -o $(OBJECTS_PATH)/Shell_Command_Copy_File.o

clean:
	rm -f $(BINARIES_PATH)/* $(OBJECTS_PATH)/*
