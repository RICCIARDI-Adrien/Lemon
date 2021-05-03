# System makefile.
# There is no need to take dependencies into account as the whole system must be entirely rebuilded each time.
# Author : Adrien RICCIARDI
PATH_BINARIES = ..
PATH_SOURCES = Sources
PATH_OBJECTS = Objects
PATH_INCLUDES = Includes
PATH_TOOLS = Tools

CCFLAGS += -I$(PATH_INCLUDES) -DCONFIGURATION_BUILD_INSTALLER=$(CONFIGURATION_BUILD_INSTALLER)
LDFLAGS = --strip-all -nostdlib -T Linker_Script.ld
ifeq ($(CONFIGURATION_BUILD_INSTALLER),1)
	MBR_FLAGS = -DSECTORS_TO_LOAD_COUNT=512 -DCONFIGURATION_BUILD_INSTALLER=1
else
	MBR_FLAGS = -DSECTORS_TO_LOAD_COUNT=64
endif

OBJECTS_CORE = $(PATH_OBJECTS)/Architecture.o $(PATH_OBJECTS)/Debug.o $(PATH_OBJECTS)/Hardware_Functions.o $(PATH_OBJECTS)/Kernel.o $(PATH_OBJECTS)/Standard_Functions.o $(PATH_OBJECTS)/System_Calls.o
OBJECTS_DRIVERS += $(PATH_OBJECTS)/Driver_Keyboard.o $(PATH_OBJECTS)/Driver_PIC.o $(PATH_OBJECTS)/Driver_RTC.o $(PATH_OBJECTS)/Driver_Screen.o $(PATH_OBJECTS)/Driver_Timer.o $(PATH_OBJECTS)/Driver_UART.o
OBJECTS_FILE_SYSTEM = $(PATH_OBJECTS)/File.o $(PATH_OBJECTS)/File_System.o
OBJECTS_SHELL_INSTALLER = $(PATH_OBJECTS)/Shell_Installer.o $(PATH_OBJECTS)/Shell_Installer_Partition_Menu.o
OBJECTS_SHELL_SYSTEM = $(PATH_OBJECTS)/Shell.o $(PATH_OBJECTS)/Shell_Command_Copy_File.o $(PATH_OBJECTS)/Shell_Command_Delete_File.o $(PATH_OBJECTS)/Shell_Command_Download.o $(PATH_OBJECTS)/Shell_Command_File_Size.o $(PATH_OBJECTS)/Shell_Command_List.o $(PATH_OBJECTS)/Shell_Command_Rename_File.o

#------------------------------------------------------------------------------------------------------------------------------
# User configuration
#------------------------------------------------------------------------------------------------------------------------------
# Choose which ethernet controller driver to compile
ifeq ($(findstring CONFIGURATION_SYSTEM_ETHERNET_CONTROLLER_DRIVER_82540EM=y,$(KCONFIG_VARIABLES)),CONFIGURATION_SYSTEM_ETHERNET_CONTROLLER_DRIVER_82540EM=y)
    OBJECTS_DRIVERS += $(PATH_OBJECTS)/Driver_Ethernet_Intel_82540EM.o
    DEPENDENCIES_INCLUDE_PCI = 1
else ifeq ($(findstring CONFIGURATION_SYSTEM_ETHERNET_CONTROLLER_DRIVER_RTL8111=y,$(KCONFIG_VARIABLES)),CONFIGURATION_SYSTEM_ETHERNET_CONTROLLER_DRIVER_RTL8111=y)
    OBJECTS_DRIVERS += $(PATH_OBJECTS)/Driver_Ethernet_Realtek_8111.o
    DEPENDENCIES_INCLUDE_PCI = 1
endif

# Choose which hard disk driver to compile
ifeq ($(findstring CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_SATA=y,$(KCONFIG_VARIABLES)),CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_SATA=y)
	OBJECTS_DRIVERS += $(PATH_OBJECTS)/Driver_Hard_Disk_SATA.o
	DEPENDENCIES_INCLUDE_PCI = 1
	CCFLAGS += -DCONFIGURATION_BUILD_RAM_DISK=0
else ifeq ($(findstring CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_RAM=y,$(KCONFIG_VARIABLES)),CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_RAM=y)
	OBJECTS_DRIVERS += $(PATH_OBJECTS)/Driver_Hard_Disk_RAM_Disk.o
	CCFLAGS += -DCONFIGURATION_BUILD_RAM_DISK=1
else ifeq ($(findstring CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_IDE=y,$(KCONFIG_VARIABLES)),CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_IDE=y)
	OBJECTS_DRIVERS += $(PATH_OBJECTS)/Driver_Hard_Disk_IDE.o
	CCFLAGS += -DCONFIGURATION_BUILD_RAM_DISK=0
else
    $(error No hard disk driver is selected. This is not allowed for now, please fix configuration file)
endif

# Compile PCI driver if needed
ifeq ($(DEPENDENCIES_INCLUDE_PCI),1)
	OBJECTS_DRIVERS += $(PATH_OBJECTS)/Driver_PCI.o
endif

#------------------------------------------------------------------------------------------------------------------------------
# Select files to build
#------------------------------------------------------------------------------------------------------------------------------
ifeq ($(CONFIGURATION_BUILD_INSTALLER),1)
    # Select the system shell instead of the installer one as it is a RAM disk build
    ifeq ($(findstring CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_RAM=y,$(KCONFIG_VARIABLES)),CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_RAM=y)
	OBJECTS_SHELL = $(OBJECTS_SHELL_SYSTEM)
    # This is a regular system build, use the installer shell
    else
	OBJECTS_SHELL = $(OBJECTS_SHELL_INSTALLER)
    endif
else
    # Do not compile anything when building the system with RAM disk support, the installer will compile the needed stuff (only the MBR will be compiled, but this is fast)
    ifeq ($(findstring CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_RAM=y,$(KCONFIG_VARIABLES)),CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_RAM=y)
	OBJECTS_CORE =
	OBJECTS_DRIVERS =
	OBJECTS_FILE_SYSTEM =
	OBJECTS_SHELL =
    # This is a regular system build (with a physical hard drive driver), select the system shell
    else
	OBJECTS_SHELL = $(OBJECTS_SHELL_SYSTEM)
    endif
endif
OBJECTS = $(OBJECTS_CORE) $(OBJECTS_DRIVERS) $(OBJECTS_FILE_SYSTEM) $(OBJECTS_SHELL)

#------------------------------------------------------------------------------------------------------------------------------
# Build rules
#------------------------------------------------------------------------------------------------------------------------------
all: clean $(PATH_OBJECTS)/MBR.bin
ifeq ($(CONFIGURATION_BUILD_INSTALLER),1)
    all: convert_files
endif
all: $(OBJECTS)
all:
ifeq ($(CONFIGURATION_BUILD_INSTALLER),1)
	######## Linking image ########
	$(GLOBAL_TOOL_LINKER) $(LDFLAGS) $(OBJECTS) -o $(PATH_OBJECTS)/Kernel.bin
	######## Creating floppy image ########
	cat $(PATH_OBJECTS)/MBR.bin $(PATH_OBJECTS)/Kernel.bin > $(PATH_OBJECTS)/Raw_Image.bin
	dd if=/dev/zero of=$(PATH_BINARIES)/Lemon_Installer_Floppy_Image.img bs=512 count=2880 status=none
	dd if=$(PATH_OBJECTS)/Raw_Image.bin of=$(PATH_BINARIES)/Lemon_Installer_Floppy_Image.img conv=notrunc status=none
	@# Get rid of useless files
	rm -f $(PATH_OBJECTS)/Raw_Image.bin
	######## Creating CDROM image ########
	mkdir -p $(PATH_OBJECTS)/ISO_Image_Content
	cp $(PATH_BINARIES)/Lemon_Installer_Floppy_Image.img $(PATH_OBJECTS)/ISO_Image_Content
	$(GLOBAL_TOOL_ISO_GENERATOR) -G $(PATH_BINARIES)/Lemon_Installer_Floppy_Image.img -r -b Lemon_Installer_Floppy_Image.img -c boot.catalog -o Lemon_Installer_CD_Image.iso $(PATH_OBJECTS)/ISO_Image_Content
	mv Lemon_Installer_CD_Image.iso $(PATH_BINARIES)
	######## Creating USB stick image ########
	cp $(PATH_BINARIES)/Lemon_Installer_Floppy_Image.img $(PATH_BINARIES)/Lemon_Installer_USB_Stick_Image.img
	######## Build successful ########
	@ls -l $(PATH_OBJECTS)/Kernel.bin | awk '{print "Installer size : " $$5 + 512 " bytes"}'
else
    ifneq ($(findstring CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_RAM=y,$(KCONFIG_VARIABLES)),CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_RAM=y)
	######## Linking kernel ########
	$(GLOBAL_TOOL_LINKER) $(LDFLAGS) $(OBJECTS) -o $(PATH_OBJECTS)/Kernel.bin
	######## Build successful ########
	@ls -l $(PATH_OBJECTS)/Kernel.bin | awk '{print "Kernel size : " $$5 + 512 " bytes"}'
    endif
endif

convert_files:
	######## Embed applications data into image ########
	@# Create fake MBR and kernel binaries for the RAM disk installer build to embed them on the image (this way installer code is left untouched)
ifeq ($(findstring CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_RAM=y,$(KCONFIG_VARIABLES)),CONFIGURATION_SYSTEM_HARD_DISK_DRIVER_RAM=y)
	@touch $(PATH_OBJECTS)/System_MBR.bin
	@touch $(PATH_OBJECTS)/System_Kernel.bin
endif
	$(PATH_TOOLS)/Files_To_Header_Converter.sh $(patsubst CONFIGURATION_INSTALLER_FILES_LIST=%,%,$(filter CONFIGURATION_INSTALLER_FILES_LIST=%,$(KCONFIG_VARIABLES)))

clean:
	rm -rf $(PATH_OBJECTS)/ISO_Image_Content
ifeq ($(CONFIGURATION_BUILD_INSTALLER),1)
	@# Delete only object files, keeping system .bin files which will be embedded into the installer image
	rm -f $(PATH_OBJECTS)/*.o
	@# Rename system MBR and kernel binaries to preserve them from installer compilation
	@if (test -e $(PATH_OBJECTS)/MBR.bin); then mv $(PATH_OBJECTS)/MBR.bin $(PATH_OBJECTS)/System_MBR.bin; fi
	@if (test -e $(PATH_OBJECTS)/Kernel.bin); then mv $(PATH_OBJECTS)/Kernel.bin $(PATH_OBJECTS)/System_Kernel.bin; fi
	@# Clean generated images
	rm -f $(PATH_BINARIES)/Lemon_Installer_CD_Image.iso $(PATH_BINARIES)/Lemon_Installer_Floppy_Image.img $(PATH_BINARIES)/Lemon_Installer_USB_Stick_Image.img
else
	@# When building the system, delete all previously generated build files
	rm -f $(PATH_OBJECTS)/*
	@# Remove the automatically generated installer files
	rm -f $(PATH_INCLUDES)/Installer_Embedded_Files_Data.h
endif

#------------------------------------------------------------------------------------------------------------------------------
# System core
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/MBR.bin: $(PATH_SOURCES)/MBR.asm
	$(GLOBAL_TOOL_ASSEMBLER) $(MBR_FLAGS) -f bin $(PATH_SOURCES)/MBR.asm -o $(PATH_OBJECTS)/MBR.bin

$(PATH_OBJECTS)/Architecture.o: $(PATH_SOURCES)/Architecture.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Architecture.c -o $(PATH_OBJECTS)/Architecture.o

$(PATH_OBJECTS)/Debug.o: $(PATH_SOURCES)/Debug.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Debug.c -o $(PATH_OBJECTS)/Debug.o

$(PATH_OBJECTS)/Hardware_Functions.o: $(PATH_SOURCES)/Hardware_Functions.asm
	$(GLOBAL_TOOL_ASSEMBLER) -f elf $(PATH_SOURCES)/Hardware_Functions.asm -o $(PATH_OBJECTS)/Hardware_Functions.o

$(PATH_OBJECTS)/Kernel.o: $(PATH_SOURCES)/Kernel.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Kernel.c -o $(PATH_OBJECTS)/Kernel.o

$(PATH_OBJECTS)/Standard_Functions.o: $(PATH_SOURCES)/Standard_Functions.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Standard_Functions.c -o $(PATH_OBJECTS)/Standard_Functions.o
	
$(PATH_OBJECTS)/System_Calls.o: $(PATH_SOURCES)/System_Calls.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/System_Calls.c -o $(PATH_OBJECTS)/System_Calls.o

#------------------------------------------------------------------------------------------------------------------------------
# Drivers
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Driver_Ethernet_Intel_82540EM.o: $(PATH_SOURCES)/Drivers/Driver_Ethernet_Intel_82540EM.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_Ethernet_Intel_82540EM.c -o $(PATH_OBJECTS)/Driver_Ethernet_Intel_82540EM.o

$(PATH_OBJECTS)/Driver_Ethernet_Realtek_8111.o: $(PATH_SOURCES)/Drivers/Driver_Ethernet_Realtek_8111.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_Ethernet_Realtek_8111.c -o $(PATH_OBJECTS)/Driver_Ethernet_Realtek_8111.o

$(PATH_OBJECTS)/Driver_Hard_Disk_IDE.o: $(PATH_SOURCES)/Drivers/Driver_Hard_Disk_IDE.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_Hard_Disk_IDE.c -o $(PATH_OBJECTS)/Driver_Hard_Disk_IDE.o

$(PATH_OBJECTS)/Driver_Hard_Disk_RAM_Disk.o: $(PATH_SOURCES)/Drivers/Driver_Hard_Disk_RAM_Disk.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_Hard_Disk_RAM_Disk.c -o $(PATH_OBJECTS)/Driver_Hard_Disk_RAM_Disk.o

$(PATH_OBJECTS)/Driver_Hard_Disk_SATA.o: $(PATH_SOURCES)/Drivers/Driver_Hard_Disk_SATA.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_Hard_Disk_SATA.c -o $(PATH_OBJECTS)/Driver_Hard_Disk_SATA.o

$(PATH_OBJECTS)/Driver_Keyboard.o: $(PATH_SOURCES)/Drivers/Driver_Keyboard.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_Keyboard.c -o $(PATH_OBJECTS)/Driver_Keyboard.o

$(PATH_OBJECTS)/Driver_PCI.o: $(PATH_SOURCES)/Drivers/Driver_PCI.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_PCI.c -o $(PATH_OBJECTS)/Driver_PCI.o

$(PATH_OBJECTS)/Driver_PIC.o: $(PATH_SOURCES)/Drivers/Driver_PIC.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_PIC.c -o $(PATH_OBJECTS)/Driver_PIC.o

$(PATH_OBJECTS)/Driver_RTC.o: $(PATH_SOURCES)/Drivers/Driver_RTC.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_RTC.c -o $(PATH_OBJECTS)/Driver_RTC.o

$(PATH_OBJECTS)/Driver_Screen.o: $(PATH_SOURCES)/Drivers/Driver_Screen.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c -O2 $(PATH_SOURCES)/Drivers/Driver_Screen.c -o $(PATH_OBJECTS)/Driver_Screen.o

$(PATH_OBJECTS)/Driver_Timer.o: $(PATH_SOURCES)/Drivers/Driver_Timer.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_Timer.c -o $(PATH_OBJECTS)/Driver_Timer.o

$(PATH_OBJECTS)/Driver_UART.o: $(PATH_SOURCES)/Drivers/Driver_UART.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Drivers/Driver_UART.c -o $(PATH_OBJECTS)/Driver_UART.o

#------------------------------------------------------------------------------------------------------------------------------
# File system
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/File.o: $(PATH_SOURCES)/File_System/File.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/File_System/File.c -o $(PATH_OBJECTS)/File.o

$(PATH_OBJECTS)/File_System.o: $(PATH_SOURCES)/File_System/File_System.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/File_System/File_System.c -o $(PATH_OBJECTS)/File_System.o

#------------------------------------------------------------------------------------------------------------------------------
# Shell
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Shell.o: $(PATH_SOURCES)/Shell/Shell.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell.c -o $(PATH_OBJECTS)/Shell.o

$(PATH_OBJECTS)/Shell_Command_Copy_File.o: $(PATH_SOURCES)/Shell/Shell_Command_Copy_File.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Copy_File.c -o $(PATH_OBJECTS)/Shell_Command_Copy_File.o

$(PATH_OBJECTS)/Shell_Command_Delete_File.o: $(PATH_SOURCES)/Shell/Shell_Command_Delete_File.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Delete_File.c -o $(PATH_OBJECTS)/Shell_Command_Delete_File.o

$(PATH_OBJECTS)/Shell_Command_Download.o: $(PATH_SOURCES)/Shell/Shell_Command_Download.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Download.c -o $(PATH_OBJECTS)/Shell_Command_Download.o

$(PATH_OBJECTS)/Shell_Command_File_Size.o: $(PATH_SOURCES)/Shell/Shell_Command_File_Size.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_File_Size.c -o $(PATH_OBJECTS)/Shell_Command_File_Size.o

$(PATH_OBJECTS)/Shell_Command_List.o: $(PATH_SOURCES)/Shell/Shell_Command_List.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_List.c -o $(PATH_OBJECTS)/Shell_Command_List.o

$(PATH_OBJECTS)/Shell_Command_Rename_File.o: $(PATH_SOURCES)/Shell/Shell_Command_Rename_File.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Rename_File.c -o $(PATH_OBJECTS)/Shell_Command_Rename_File.o

$(PATH_OBJECTS)/Shell_Installer.o: $(PATH_SOURCES)/Shell/Shell_Installer.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Installer.c -o $(PATH_OBJECTS)/Shell_Installer.o

$(PATH_OBJECTS)/Shell_Installer_Partition_Menu.o: $(PATH_SOURCES)/Shell/Shell_Installer_Partition_Menu.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Installer_Partition_Menu.c -o $(PATH_OBJECTS)/Shell_Installer_Partition_Menu.o
