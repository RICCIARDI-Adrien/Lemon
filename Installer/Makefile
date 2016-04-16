# Installer makefile.
# There is no need to take dependencies into account as the whole system must be entirely rebuilded every time.
# Author : Adrien RICCIARDI
include ../Global_Rules.mk

BINARIES_TO_HEADER_CONVERTER = Tools/Binaries_To_Header_Converter.out

PATH_SOURCES = ../System/Sources
PATH_INCLUDES = ../System/Includes
PATH_OBJECTS = Objects
PATH_BINARIES = Binaries
PATH_INSTALLER_SOURCES = Sources

CCFLAGS += -DINSTALLER
LDFLAGS = -T ../System/Linker_Script.ld
MBR_FLAGS = -DSECTORS_TO_LOAD_COUNT=512 -DINSTALLER

# The common makefile must be included after the paths and flags variables have been defined (because it uses them), and before the object variables are set (because it contains some of them)
include ../System/Makefile_Core.mk

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

#------------------------------------------------------------------------------------------------------------------------------
# Converter tool program
#------------------------------------------------------------------------------------------------------------------------------
$(BINARIES_TO_HEADER_CONVERTER): Tools/Binaries_To_Header_Converter.c
	gcc -W -Wall Tools/Binaries_To_Header_Converter.c -o $(BINARIES_TO_HEADER_CONVERTER)

Convert_Files:
	######## Embed applications data into installer image ########
	@$(BINARIES_TO_HEADER_CONVERTER) Tools/Binaries_To_Header_Converter_List.txt $(PATH_INSTALLER_SOURCES)/Shell/Embedded_Files_Data.h

#------------------------------------------------------------------------------------------------------------------------------
# Shell
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Shell.o: $(PATH_INSTALLER_SOURCES)/Shell/Shell.c
	$(CC) $(CCFLAGS) -c $(PATH_INSTALLER_SOURCES)/Shell/Shell.c -o $(PATH_OBJECTS)/Shell.o
	
$(PATH_OBJECTS)/Shell_Partition_Menu.o: $(PATH_INSTALLER_SOURCES)/Shell/Shell_Partition_Menu.c
	$(CC) $(CCFLAGS) -c $(PATH_INSTALLER_SOURCES)/Shell/Shell_Partition_Menu.c -o $(PATH_OBJECTS)/Shell_Partition_Menu.o
