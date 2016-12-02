# Installer makefile.
# There is no need to take dependencies into account as the whole system must be entirely rebuilded every time.
# Author : Adrien RICCIARDI
PATH_SOURCES = ../System/Sources
PATH_INCLUDES = ../System/Includes
PATH_OBJECTS = Objects
PATH_BINARIES = Binaries
PATH_INSTALLER_SOURCES = Sources
PATH_TOOLS = Tools

CCFLAGS += -DCONFIGURATION_BUILD_INSTALLER=1
LDFLAGS = -T ../System/Linker_Script.ld
MBR_FLAGS = -DSECTORS_TO_LOAD_COUNT=512 -DCONFIGURATION_BUILD_INSTALLER=1

# The common makefile must be included after the paths and flags variables have been defined (because it uses them), and before the object variables are set (because it contains some of them)
include ../System/Makefile_Core.mk

OBJECTS_SHELL = $(PATH_OBJECTS)/Shell.o $(PATH_OBJECTS)/Shell_Partition_Menu.o
OBJECTS = $(OBJECTS_CORE) $(OBJECTS_DRIVERS) $(OBJECTS_FILE_SYSTEM) $(OBJECTS_SHELL)

all: clean $(PATH_OBJECTS)/MBR.bin
# Build the installer only if the system won't use a RAM disk
ifneq ($(SYSTEM_HARD_DISK_DRIVER),ram)
    all: convert_files $(OBJECTS)
endif
all:
ifeq ($(SYSTEM_HARD_DISK_DRIVER),ram)
	@# The regular kernel will be directly booted when using a RAM disk
	cp ../System/Objects/Kernel.bin $(PATH_OBJECTS)
else
	######## Linking installer ########
	$(GLOBAL_TOOL_LINKER) $(LDFLAGS) $(OBJECTS) -o $(PATH_OBJECTS)/Kernel.bin
endif
	######## Creating floppy image ########
	@cat $(PATH_OBJECTS)/MBR.bin $(PATH_OBJECTS)/Kernel.bin > $(PATH_BINARIES)/Raw_Image.bin
	@dd if=/dev/zero of=$(PATH_BINARIES)/Lemon_Installer_Floppy_Image.img bs=512 count=2880 status=noxfer
	@dd if=$(PATH_BINARIES)/Raw_Image.bin of=$(PATH_BINARIES)/Lemon_Installer_Floppy_Image.img conv=notrunc status=noxfer
	@# Clean binaries folder of useless files
	@rm $(PATH_BINARIES)/*.bin
	######## Creating CDROM image ########
	@if (test -e $(PATH_BINARIES)/Lemon_Installer_CD_Image.iso); then rm $(PATH_BINARIES)/Lemon_Installer_CD_Image.iso; fi
	$(GLOBAL_TOOL_ISO_GENERATOR) -G $(PATH_BINARIES)/Lemon_Installer_Floppy_Image.img -r -b Lemon_Installer_Floppy_Image.img -c boot.catalog -o Lemon_Installer_CD_Image.iso $(PATH_BINARIES)
	@mv Lemon_Installer_CD_Image.iso $(PATH_BINARIES)
	######## Build successful ########
	@ls -l $(PATH_OBJECTS)/Kernel.bin | awk '{print "Installer size : " $$5 + 512 " bytes"}'

# Make the clean rule always call the clean_installer_specific_files one
clean: clean_installer_specific_files
clean_installer_specific_files:
	@# Clean the tools too to allow the Lemon directory to be copied on another machine (possibly running another architecture), so the old and bad binary won't be used on the new architecture
	rm -f $(BINARIES_TO_HEADER_CONVERTER)
	@# Clean generated images
	rm -f $(PATH_BINARIES)/*
	
convert_files: $(BINARIES_TO_HEADER_CONVERTER)
	######## Embed applications data into image ########
	@$(BINARIES_TO_HEADER_CONVERTER) $(PATH_TOOLS)/Binaries_To_Header_Converter_List.txt $(PATH_INSTALLER_SOURCES)/Shell/Embedded_Files_Data.h

#------------------------------------------------------------------------------------------------------------------------------
# Shell
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Shell.o: $(PATH_INSTALLER_SOURCES)/Shell/Shell.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_INSTALLER_SOURCES)/Shell/Shell.c -o $(PATH_OBJECTS)/Shell.o
	
$(PATH_OBJECTS)/Shell_Partition_Menu.o: $(PATH_INSTALLER_SOURCES)/Shell/Shell_Partition_Menu.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_INSTALLER_SOURCES)/Shell/Shell_Partition_Menu.c -o $(PATH_OBJECTS)/Shell_Partition_Menu.o
