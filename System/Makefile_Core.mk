# Core makefile.
# Contains the common core for the system and the installer. There is no need to take dependencies into account as the whole system must be entirely rebuilded each time.
# Author : Adrien RICCIARDI
CCFLAGS += -W -Wall -Wunused -I$(PATH_INCLUDES) -DCONFIGURATION_SYSTEM_TOTAL_RAM_SIZE_MEGA_BYTES=$(SYSTEM_RAM_SIZE) -DCONFIGURATION_HARD_DISK_LOGICAL_BLOCK_ADDRESSING_MODE=$(SYSTEM_HARD_DISK_LOGICAL_BLOCK_ADDRESSING_MODE)
LDFLAGS += --strip-all -nostdlib -nostartfile

OBJECTS_CORE = $(PATH_OBJECTS)/Architecture.o $(PATH_OBJECTS)/Debug.o $(PATH_OBJECTS)/Hardware_Functions.o $(PATH_OBJECTS)/Kernel.o $(PATH_OBJECTS)/Standard_Functions.o $(PATH_OBJECTS)/System_Calls.o
OBJECTS_DRIVERS = $(PATH_OBJECTS)/Driver_Keyboard.o $(PATH_OBJECTS)/Driver_PIC.o $(PATH_OBJECTS)/Driver_Screen.o $(PATH_OBJECTS)/Driver_Timer.o $(PATH_OBJECTS)/Driver_UART.o
OBJECTS_FILE_SYSTEM = $(PATH_OBJECTS)/File.o $(PATH_OBJECTS)/File_System.o

#------------------------------------------------------------------------------------------------------------------------------
# User configuration
#------------------------------------------------------------------------------------------------------------------------------
ifeq ($(SYSTEM_IS_DEBUG_ENABLED),1)
	CCFLAGS += -DCONFIGURATION_IS_DEBUG_ENABLED=1
else
	CCFLAGS += -DCONFIGURATION_IS_DEBUG_ENABLED=0
endif

# Choose which hard disk driver to compile
ifeq ($(SYSTEM_HARD_DISK_TYPE),sata)
	OBJECTS_DRIVERS += $(PATH_OBJECTS)/Driver_Hard_Disk_SATA.o $(PATH_OBJECTS)/Driver_PCI.o
else
	OBJECTS_DRIVERS += $(PATH_OBJECTS)/Driver_Hard_Disk_IDE.o
endif

#------------------------------------------------------------------------------------------------------------------------------
# Entry point and default rules
#------------------------------------------------------------------------------------------------------------------------------
# Force 'all' to be the default makefile rule
pointer_to_all: check_configuration_variables all

clean:
	rm -f $(PATH_OBJECTS)/*

check_configuration_variables:
	@# Check SYSTEM_RAM_SIZE value
	@if [ $(SYSTEM_RAM_SIZE) -lt 2 ]; then printf "\033[31mError : the SYSTEM_RAM_SIZE variable minimum value is 2.\033[0m\n"; false; fi
	@if [ $(SYSTEM_RAM_SIZE) -gt 4096 ]; then printf "\033[31mError : the SYSTEM_RAM_SIZE variable maximum value is 4096.\033[0m\n"; false; fi

#------------------------------------------------------------------------------------------------------------------------------
# System core
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/MBR.bin: $(PATH_SOURCES)/MBR.asm
	$(AS) $(MBR_FLAGS) -f bin $(PATH_SOURCES)/MBR.asm -o $(PATH_OBJECTS)/MBR.bin

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
