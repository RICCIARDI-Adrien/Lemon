# Installer makefile.
# There is no need to take dependencies into account as the whole system must be entirely rebuilded every time.
# Author : Adrien RICCIARDI
include ../Rules.mk

ISO_GENERATOR = genisoimage
BINARIES_TO_HEADER_CONVERTER = Tools/Binaries_To_Header_Converter.out

PATH_SYSTEM_SOURCES = ../System/Sources
PATH_SYSTEM_INCLUDES = ../System/Includes
PATH_SOURCES = Sources
PATH_OBJECTS = Objects
PATH_BINARIES = Binaries

INCLUDES = $(PATH_SYSTEM_INCLUDES)

CCFLAGS += -W -Wall -Wunused -I$(INCLUDES) -DINSTALLER
LDFLAGS = -T ../System/Linker_Script.ld --strip-all -nostdlib -nostartfile
MBR_FLAGS = -DSECTORS_TO_LOAD_COUNT=512 -DINSTALLER

OBJECTS_CORE = $(PATH_OBJECTS)/Architecture.o $(PATH_OBJECTS)/Debug.o $(PATH_OBJECTS)/Hardware_Functions.o $(PATH_OBJECTS)/Kernel.o $(PATH_OBJECTS)/Standard_Functions.o $(PATH_OBJECTS)/System_Calls.o
OBJECTS_DRIVERS = $(PATH_OBJECTS)/Driver_Hard_Disk_IDE.o $(PATH_OBJECTS)/Driver_Hard_Disk_SATA.o $(PATH_OBJECTS)/Driver_Keyboard.o $(PATH_OBJECTS)/Driver_PCI.o $(PATH_OBJECTS)/Driver_PIC.o $(PATH_OBJECTS)/Driver_Screen.o $(PATH_OBJECTS)/Driver_Timer.o $(PATH_OBJECTS)/Driver_UART.o
OBJECTS_FILE_SYSTEM = $(PATH_OBJECTS)/File.o $(PATH_OBJECTS)/File_System.o
OBJECTS_SHELL = $(PATH_OBJECTS)/Shell.o $(PATH_OBJECTS)/Shell_Partition_Menu.o
OBJECTS = $(OBJECTS_CORE) $(OBJECTS_DRIVERS) $(OBJECTS_FILE_SYSTEM) $(OBJECTS_SHELL)


all: clean Tools/Binaries_To_Header_Converter.out Convert_Files $(PATH_OBJECTS)/MBR.32b $(OBJECTS)
	######## Linking installer ########
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(PATH_OBJECTS)/Kernel.32b
	######## Creating floppy image ########
	@cat $(PATH_OBJECTS)/MBR.32b $(PATH_OBJECTS)/Kernel.32b > $(PATH_BINARIES)/Raw_Image.bin
	@dd if=/dev/zero of=$(PATH_BINARIES)/Lemon_Installer_Floppy_Image.img bs=512 count=2880 status=noxfer
	@dd if=$(PATH_BINARIES)/Raw_Image.bin of=$(PATH_BINARIES)/Lemon_Installer_Floppy_Image.img conv=notrunc status=noxfer
	@# Clean binaries folder of useless files
	@rm $(PATH_BINARIES)/*.bin
	######## Creating CDROM image ########
	@if (test -e $(PATH_BINARIES)/Lemon_Installer_CD_Image.iso); then rm $(PATH_BINARIES)/Lemon_Installer_CD_Image.iso; fi
	$(ISO_GENERATOR) -G $(PATH_BINARIES)/Lemon_Installer_Floppy_Image.img -hard-disk-boot -r -b Lemon_Installer_Floppy_Image.img -c boot.catalog -o Lemon_Installer_CD_Image.iso $(PATH_BINARIES)
	@mv Lemon_Installer_CD_Image.iso $(PATH_BINARIES)
	######## Build successful ########
	@ls -l $(PATH_OBJECTS)/Kernel.32b | awk '{print "Installer size : " $$5 + 512 " bytes"}'

clean:
	rm -f $(PATH_BINARIES)/* $(PATH_OBJECTS)/* $(PATH_SOURCES)/Shell/Embedded_Files_Data.h $(BINARIES_TO_HEADER_CONVERTER)

#------------------------------------------------------------------------------------------------------------------------------
# Converter tool program
#------------------------------------------------------------------------------------------------------------------------------
$(BINARIES_TO_HEADER_CONVERTER): Tools/Binaries_To_Header_Converter.c
	gcc -W -Wall Tools/Binaries_To_Header_Converter.c -o $(BINARIES_TO_HEADER_CONVERTER)

Convert_Files:
	######## Embed applications data into installer image ########
	@$(BINARIES_TO_HEADER_CONVERTER) Tools/Binaries_To_Header_Converter_List.txt $(PATH_SOURCES)/Shell/Embedded_Files_Data.h

#------------------------------------------------------------------------------------------------------------------------------
# System core
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/MBR.32b: $(PATH_SYSTEM_SOURCES)/MBR.asm
	$(AS) $(MBR_FLAGS) -f bin $(PATH_SYSTEM_SOURCES)/MBR.asm -o $(PATH_OBJECTS)/MBR.32b

$(PATH_OBJECTS)/Architecture.o: $(PATH_SYSTEM_SOURCES)/Architecture.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/Architecture.c -o $(PATH_OBJECTS)/Architecture.o

$(PATH_OBJECTS)/Debug.o: $(PATH_SYSTEM_SOURCES)/Debug.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/Debug.c -o $(PATH_OBJECTS)/Debug.o

$(PATH_OBJECTS)/Hardware_Functions.o: $(PATH_SYSTEM_SOURCES)/Hardware_Functions.asm
	$(AS) -f elf $(PATH_SYSTEM_SOURCES)/Hardware_Functions.asm -o $(PATH_OBJECTS)/Hardware_Functions.o

$(PATH_OBJECTS)/Kernel.o: $(PATH_SYSTEM_SOURCES)/Kernel.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/Kernel.c -o $(PATH_OBJECTS)/Kernel.o

$(PATH_OBJECTS)/Standard_Functions.o: $(PATH_SYSTEM_SOURCES)/Standard_Functions.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/Standard_Functions.c -o $(PATH_OBJECTS)/Standard_Functions.o

$(PATH_OBJECTS)/System_Calls.o: $(PATH_SYSTEM_SOURCES)/System_Calls.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/System_Calls.c -o $(PATH_OBJECTS)/System_Calls.o

#------------------------------------------------------------------------------------------------------------------------------
# Drivers
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Driver_Hard_Disk_IDE.o: $(PATH_SYSTEM_SOURCES)/Drivers/Driver_Hard_Disk_IDE.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/Drivers/Driver_Hard_Disk_IDE.c -o $(PATH_OBJECTS)/Driver_Hard_Disk_IDE.o

$(PATH_OBJECTS)/Driver_Hard_Disk_SATA.o: $(PATH_SYSTEM_SOURCES)/Drivers/Driver_Hard_Disk_SATA.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/Drivers/Driver_Hard_Disk_SATA.c -o $(PATH_OBJECTS)/Driver_Hard_Disk_SATA.o

$(PATH_OBJECTS)/Driver_Keyboard.o: $(PATH_SYSTEM_SOURCES)/Drivers/Driver_Keyboard.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/Drivers/Driver_Keyboard.c -o $(PATH_OBJECTS)/Driver_Keyboard.o

$(PATH_OBJECTS)/Driver_PCI.o: $(PATH_SYSTEM_SOURCES)/Drivers/Driver_PCI.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/Drivers/Driver_PCI.c -o $(PATH_OBJECTS)/Driver_PCI.o

$(PATH_OBJECTS)/Driver_PIC.o: $(PATH_SYSTEM_SOURCES)/Drivers/Driver_PIC.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/Drivers/Driver_PIC.c -o $(PATH_OBJECTS)/Driver_PIC.o

$(PATH_OBJECTS)/Driver_Screen.o: $(PATH_SYSTEM_SOURCES)/Drivers/Driver_Screen.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/Drivers/Driver_Screen.c -o $(PATH_OBJECTS)/Driver_Screen.o

$(PATH_OBJECTS)/Driver_Timer.o: $(PATH_SYSTEM_SOURCES)/Drivers/Driver_Timer.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/Drivers/Driver_Timer.c -o $(PATH_OBJECTS)/Driver_Timer.o

$(PATH_OBJECTS)/Driver_UART.o: $(PATH_SYSTEM_SOURCES)/Drivers/Driver_UART.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/Drivers/Driver_UART.c -o $(PATH_OBJECTS)/Driver_UART.o

#------------------------------------------------------------------------------------------------------------------------------
# File system
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/File.o: $(PATH_SYSTEM_SOURCES)/File_System/File.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/File_System/File.c -o $(PATH_OBJECTS)/File.o

$(PATH_OBJECTS)/File_System.o: $(PATH_SYSTEM_SOURCES)/File_System/File_System.c
	$(CC) $(CCFLAGS) -c $(PATH_SYSTEM_SOURCES)/File_System/File_System.c -o $(PATH_OBJECTS)/File_System.o

#------------------------------------------------------------------------------------------------------------------------------
# Shell
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Shell.o: $(PATH_SOURCES)/Shell/Shell.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell.c -o $(PATH_OBJECTS)/Shell.o
	
$(PATH_OBJECTS)/Shell_Partition_Menu.o: $(PATH_SOURCES)/Shell/Shell_Partition_Menu.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Partition_Menu.c -o $(PATH_OBJECTS)/Shell_Partition_Menu.o
