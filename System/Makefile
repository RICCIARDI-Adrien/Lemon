# System makefile.
# There is no need to take dependencies into account as the whole system must be entirely rebuilded each time.
# Author : Adrien RICCIARDI
include ../Rules.mk

PATH_SOURCES = Sources
PATH_OBJECTS = Objects
PATH_INCLUDES = Includes

INCLUDES = -I$(PATH_INCLUDES)

CCFLAGS += -W -Wall -Wunused $(INCLUDES)
LDFLAGS = -T Linker_Script.ld --strip-all -nostdlib -nostartfile
MBR_FLAGS = -DSECTORS_TO_LOAD_COUNT=64

OBJECTS_CORE = $(PATH_OBJECTS)/Architecture.o $(PATH_OBJECTS)/Debug.o $(PATH_OBJECTS)/Hardware_Functions.o $(PATH_OBJECTS)/Kernel.o $(PATH_OBJECTS)/Standard_Functions.o $(PATH_OBJECTS)/System_Calls.o
OBJECTS_DRIVERS = $(PATH_OBJECTS)/Driver_Hard_Disk_IDE.o $(PATH_OBJECTS)/Driver_Hard_Disk_SATA.o $(PATH_OBJECTS)/Driver_Keyboard.o $(PATH_OBJECTS)/Driver_PCI.o $(PATH_OBJECTS)/Driver_PIC.o $(PATH_OBJECTS)/Driver_Screen.o $(PATH_OBJECTS)/Driver_Timer.o $(PATH_OBJECTS)/Driver_UART.o
OBJECTS_FILE_SYSTEM = $(PATH_OBJECTS)/File.o $(PATH_OBJECTS)/File_System.o
OBJECTS_SHELL = $(PATH_OBJECTS)/Shell.o $(PATH_OBJECTS)/Shell_Command_Copy_File.o $(PATH_OBJECTS)/Shell_Command_Delete_File.o $(PATH_OBJECTS)/Shell_Command_Download.o $(PATH_OBJECTS)/Shell_Command_File_Size.o $(PATH_OBJECTS)/Shell_Command_List.o $(PATH_OBJECTS)/Shell_Command_Rename_File.o
OBJECTS = $(OBJECTS_CORE) $(OBJECTS_DRIVERS) $(OBJECTS_FILE_SYSTEM) $(OBJECTS_SHELL)

all: clean $(PATH_OBJECTS)/MBR.32b $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(PATH_OBJECTS)/Kernel.32b
	######## Build successful ########
	@ls -l $(PATH_OBJECTS)/Kernel.32b | awk '{print "Kernel size : " $$5 + 512 " bytes"}'

clean:
	rm -f $(PATH_OBJECTS)/*

#------------------------------------------------------------------------------------------------------------------------------
# System core
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/MBR.32b: $(PATH_SOURCES)/MBR.asm
	$(AS) $(MBR_FLAGS) -f bin $(PATH_SOURCES)/MBR.asm -o $(PATH_OBJECTS)/MBR.32b

$(PATH_OBJECTS)/Architecture.o: $(PATH_SOURCES)/Architecture.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Architecture.c -o $(PATH_OBJECTS)/Architecture.o

$(PATH_OBJECTS)/Debug.o: $(PATH_SOURCES)/Debug.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Debug.c -o $(PATH_OBJECTS)/Debug.o

$(PATH_OBJECTS)/Hardware_Functions.o: $(PATH_SOURCES)/Hardware_Functions.asm
	$(AS) -f elf $(PATH_SOURCES)/Hardware_Functions.asm -o $(PATH_OBJECTS)/Hardware_Functions.o

$(PATH_OBJECTS)/Kernel.o: $(PATH_SOURCES)/Kernel.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Kernel.c -o $(PATH_OBJECTS)/Kernel.o

$(PATH_OBJECTS)/Standard_Functions.o: $(PATH_SOURCES)/Standard_Functions.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Standard_Functions.c -o $(PATH_OBJECTS)/Standard_Functions.o
	
$(PATH_OBJECTS)/System_Calls.o: $(PATH_SOURCES)/System_Calls.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/System_Calls.c -o $(PATH_OBJECTS)/System_Calls.o

#------------------------------------------------------------------------------------------------------------------------------
# Drivers
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Driver_Hard_Disk_IDE.o: $(PATH_SOURCES)/Drivers/Driver_Hard_Disk_IDE.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_Hard_Disk_IDE.c -o $(PATH_OBJECTS)/Driver_Hard_Disk_IDE.o

$(PATH_OBJECTS)/Driver_Hard_Disk_SATA.o: $(PATH_SOURCES)/Drivers/Driver_Hard_Disk_SATA.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_Hard_Disk_SATA.c -o $(PATH_OBJECTS)/Driver_Hard_Disk_SATA.o

$(PATH_OBJECTS)/Driver_Keyboard.o: $(PATH_SOURCES)/Drivers/Driver_Keyboard.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_Keyboard.c -o $(PATH_OBJECTS)/Driver_Keyboard.o

$(PATH_OBJECTS)/Driver_PCI.o: $(PATH_SOURCES)/Drivers/Driver_PCI.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_PCI.c -o $(PATH_OBJECTS)/Driver_PCI.o

$(PATH_OBJECTS)/Driver_PIC.o: $(PATH_SOURCES)/Drivers/Driver_PIC.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_PIC.c -o $(PATH_OBJECTS)/Driver_PIC.o

$(PATH_OBJECTS)/Driver_Screen.o: $(PATH_SOURCES)/Drivers/Driver_Screen.c
	$(CC) $(CCFLAGS) -c -O2 $(PATH_SOURCES)/Drivers/Driver_Screen.c -o $(PATH_OBJECTS)/Driver_Screen.o

$(PATH_OBJECTS)/Driver_Timer.o: $(PATH_SOURCES)/Drivers/Driver_Timer.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_Timer.c -o $(PATH_OBJECTS)/Driver_Timer.o

$(PATH_OBJECTS)/Driver_UART.o: $(PATH_SOURCES)/Drivers/Driver_UART.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_UART.c -o $(PATH_OBJECTS)/Driver_UART.o

#------------------------------------------------------------------------------------------------------------------------------
# File system
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/File.o: $(PATH_SOURCES)/File_System/File.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/File_System/File.c -o $(PATH_OBJECTS)/File.o

$(PATH_OBJECTS)/File_System.o: $(PATH_SOURCES)/File_System/File_System.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/File_System/File_System.c -o $(PATH_OBJECTS)/File_System.o

#------------------------------------------------------------------------------------------------------------------------------
# Shell
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Shell.o: $(PATH_SOURCES)/Shell/Shell.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell.c -o $(PATH_OBJECTS)/Shell.o

$(PATH_OBJECTS)/Shell_Command_Copy_File.o: $(PATH_SOURCES)/Shell/Shell_Command_Copy_File.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Copy_File.c -o $(PATH_OBJECTS)/Shell_Command_Copy_File.o

$(PATH_OBJECTS)/Shell_Command_Delete_File.o: $(PATH_SOURCES)/Shell/Shell_Command_Delete_File.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Delete_File.c -o $(PATH_OBJECTS)/Shell_Command_Delete_File.o

$(PATH_OBJECTS)/Shell_Command_Download.o: $(PATH_SOURCES)/Shell/Shell_Command_Download.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Download.c -o $(PATH_OBJECTS)/Shell_Command_Download.o

$(PATH_OBJECTS)/Shell_Command_File_Size.o: $(PATH_SOURCES)/Shell/Shell_Command_File_Size.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_File_Size.c -o $(PATH_OBJECTS)/Shell_Command_File_Size.o

$(PATH_OBJECTS)/Shell_Command_List.o: $(PATH_SOURCES)/Shell/Shell_Command_List.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_List.c -o $(PATH_OBJECTS)/Shell_Command_List.o

$(PATH_OBJECTS)/Shell_Command_Rename_File.o: $(PATH_SOURCES)/Shell/Shell_Command_Rename_File.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Rename_File.c -o $(PATH_OBJECTS)/Shell_Command_Rename_File.o
