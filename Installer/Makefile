# Installer makefile.
# There is no need to take dependencies into account as the whole system must be entirely rebuilded every time.
# Author : Adrien RICCIARDI
include ../Rules.mk

ISO_GENERATOR = genisoimage

SYSTEM_SOURCES_PATH = ../System/Sources
SYSTEM_INCLUDES_PATH = ../System/Includes
SOURCES_PATH = Sources
OBJECTS_PATH = Objects
BINARIES_PATH = Binaries

OBJECTS_CORE = $(OBJECTS_PATH)/Architecture.o $(OBJECTS_PATH)/Hardware_Functions.o $(OBJECTS_PATH)/Kernel.o $(OBJECTS_PATH)/Standard_Functions.o $(OBJECTS_PATH)/System_Calls.o
OBJECTS_DRIVERS = $(OBJECTS_PATH)/Driver_Keyboard.o $(OBJECTS_PATH)/Driver_Hard_Disk.o $(OBJECTS_PATH)/Driver_UART.o $(OBJECTS_PATH)/Driver_Timer.o $(OBJECTS_PATH)/Driver_Screen.o $(OBJECTS_PATH)/Driver_PIC.o
OBJECTS_FILE_SYSTEM = $(OBJECTS_PATH)/File_System.o $(OBJECTS_PATH)/File.o
OBJECTS_SHELL = $(OBJECTS_PATH)/Shell.o $(OBJECTS_PATH)/Shell_Partition_Menu.o
OBJECTS = $(OBJECTS_CORE) $(OBJECTS_DRIVERS) $(OBJECTS_FILE_SYSTEM) $(OBJECTS_SHELL)

INCLUDES = $(SYSTEM_INCLUDES_PATH)

CCFLAGS += -W -Wall -Wunused -I$(INCLUDES) -DINSTALLER
LDFLAGS = -T ../System/Linker_Script.ld --strip-all -nostdlib -nostartfile
MBR_FLAGS = -DSECTORS_TO_LOAD_COUNT=512 -DINSTALLER

BINARIES_TO_HEADER_CONVERTER = Tools/Binaries_To_Header_Converter.out

all: clean Tools/Binaries_To_Header_Converter.out Convert_Files $(OBJECTS_PATH)/MBR.32b $(OBJECTS)
	######## Linking installer ########
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(OBJECTS_PATH)/Kernel.32b
	######## Creating floppy image ########
	@cat $(OBJECTS_PATH)/MBR.32b $(OBJECTS_PATH)/Kernel.32b > $(BINARIES_PATH)/Raw_Image.bin
	@dd if=/dev/zero of=$(BINARIES_PATH)/Lemon_Installer_Floppy_Image.img bs=512 count=2880 status=noxfer
	@dd if=$(BINARIES_PATH)/Raw_Image.bin of=$(BINARIES_PATH)/Lemon_Installer_Floppy_Image.img conv=notrunc status=noxfer
	@# Clean binaries folder of useless files
	@rm $(BINARIES_PATH)/*.bin
	######## Creating CDROM image ########
	@if (test -e $(BINARIES_PATH)/Lemon_Installer_CD_Image.iso); then rm $(BINARIES_PATH)/Lemon_Installer_CD_Image.iso; fi
	$(ISO_GENERATOR) -G $(BINARIES_PATH)/Lemon_Installer_Floppy_Image.img -hard-disk-boot -r -b Lemon_Installer_Floppy_Image.img -c boot.catalog -o Lemon_Installer_CD_Image.iso $(BINARIES_PATH)
	@mv Lemon_Installer_CD_Image.iso $(BINARIES_PATH)
	######## Build successful ########
	@ls -l $(OBJECTS_PATH)/Kernel.32b | awk '{print "Installer size : " $$5 + 512 " bytes"}'

clean:
	rm -f $(BINARIES_PATH)/* $(OBJECTS_PATH)/* $(SOURCES_PATH)/Shell/Embedded_Files_Data.h $(BINARIES_TO_HEADER_CONVERTER)

#------------------------------------------------------------------------------------------------------------------------------
# Converter tool program
#------------------------------------------------------------------------------------------------------------------------------
$(BINARIES_TO_HEADER_CONVERTER): Tools/Binaries_To_Header_Converter.c
	gcc -W -Wall Tools/Binaries_To_Header_Converter.c -o $(BINARIES_TO_HEADER_CONVERTER)

Convert_Files:
	######## Embed applications data into installer image ########
	@$(BINARIES_TO_HEADER_CONVERTER) Tools/Binaries_To_Header_Converter_List.txt $(SOURCES_PATH)/Shell/Embedded_Files_Data.h

#------------------------------------------------------------------------------------------------------------------------------
# System core
#------------------------------------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/MBR.32b: $(SYSTEM_SOURCES_PATH)/MBR.asm
	$(AS) $(MBR_FLAGS) -f bin $(SYSTEM_SOURCES_PATH)/MBR.asm -o $(OBJECTS_PATH)/MBR.32b

$(OBJECTS_PATH)/Architecture.o: $(SYSTEM_SOURCES_PATH)/Architecture.c
	$(CC) $(CCFLAGS) -c $(SYSTEM_SOURCES_PATH)/Architecture.c -o $(OBJECTS_PATH)/Architecture.o

$(OBJECTS_PATH)/Hardware_Functions.o: $(SYSTEM_SOURCES_PATH)/Hardware_Functions.asm
	$(AS) -f elf $(SYSTEM_SOURCES_PATH)/Hardware_Functions.asm -o $(OBJECTS_PATH)/Hardware_Functions.o

$(OBJECTS_PATH)/Kernel.o: $(SYSTEM_SOURCES_PATH)/Kernel.c
	$(CC) $(CCFLAGS) -c $(SYSTEM_SOURCES_PATH)/Kernel.c -o $(OBJECTS_PATH)/Kernel.o

$(OBJECTS_PATH)/System_Calls.o: $(SYSTEM_SOURCES_PATH)/System_Calls.c
	$(CC) $(CCFLAGS) -c $(SYSTEM_SOURCES_PATH)/System_Calls.c -o $(OBJECTS_PATH)/System_Calls.o

$(OBJECTS_PATH)/Standard_Functions.o: $(SYSTEM_SOURCES_PATH)/Standard_Functions.c
	$(CC) $(CCFLAGS) -c $(SYSTEM_SOURCES_PATH)/Standard_Functions.c -o $(OBJECTS_PATH)/Standard_Functions.o

#------------------------------------------------------------------------------------------------------------------------------
# Drivers
#------------------------------------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/Driver_Keyboard.o: $(SYSTEM_SOURCES_PATH)/Drivers/Driver_Keyboard.c
	$(CC) $(CCFLAGS) -c $(SYSTEM_SOURCES_PATH)/Drivers/Driver_Keyboard.c -o $(OBJECTS_PATH)/Driver_Keyboard.o
	
$(OBJECTS_PATH)/Driver_Hard_Disk.o: $(SYSTEM_SOURCES_PATH)/Drivers/Driver_Hard_Disk.c
	$(CC) $(CCFLAGS) -c $(SYSTEM_SOURCES_PATH)/Drivers/Driver_Hard_Disk.c -o $(OBJECTS_PATH)/Driver_Hard_Disk.o

$(OBJECTS_PATH)/Driver_UART.o: $(SYSTEM_SOURCES_PATH)/Drivers/Driver_UART.c
	$(CC) $(CCFLAGS) -c $(SYSTEM_SOURCES_PATH)/Drivers/Driver_UART.c -o $(OBJECTS_PATH)/Driver_UART.o

$(OBJECTS_PATH)/Driver_Timer.o: $(SYSTEM_SOURCES_PATH)/Drivers/Driver_Timer.c
	$(CC) $(CCFLAGS) -c $(SYSTEM_SOURCES_PATH)/Drivers/Driver_Timer.c -o $(OBJECTS_PATH)/Driver_Timer.o

$(OBJECTS_PATH)/Driver_Screen.o: $(SYSTEM_SOURCES_PATH)/Drivers/Driver_Screen.c
	$(CC) $(CCFLAGS) -c $(SYSTEM_SOURCES_PATH)/Drivers/Driver_Screen.c -o $(OBJECTS_PATH)/Driver_Screen.o

$(OBJECTS_PATH)/Driver_PIC.o: $(SYSTEM_SOURCES_PATH)/Drivers/Driver_PIC.c
	$(CC) $(CCFLAGS) -c $(SYSTEM_SOURCES_PATH)/Drivers/Driver_PIC.c -o $(OBJECTS_PATH)/Driver_PIC.o

#------------------------------------------------------------------------------------------------------------------------------
# File system
#------------------------------------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/File_System.o: $(SYSTEM_SOURCES_PATH)/File_System/File_System.c
	$(CC) $(CCFLAGS) -c $(SYSTEM_SOURCES_PATH)/File_System/File_System.c -o $(OBJECTS_PATH)/File_System.o

$(OBJECTS_PATH)/File.o: $(SYSTEM_SOURCES_PATH)/File_System/File.c
	$(CC) $(CCFLAGS) -c $(SYSTEM_SOURCES_PATH)/File_System/File.c -o $(OBJECTS_PATH)/File.o

#------------------------------------------------------------------------------------------------------------------------------
# Shell
#------------------------------------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/Shell.o: $(SOURCES_PATH)/Shell/Shell.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Shell/Shell.c -o $(OBJECTS_PATH)/Shell.o
	
$(OBJECTS_PATH)/Shell_Partition_Menu.o: $(SOURCES_PATH)/Shell/Shell_Partition_Menu.c
	$(CC) $(CCFLAGS) -c $(SOURCES_PATH)/Shell/Shell_Partition_Menu.c -o $(OBJECTS_PATH)/Shell_Partition_Menu.o
