# Main makefile
# Author : Adrien RICCIARDI
SDK_PATH = ../Lemon_SDK

# Make sure the configuration file is specified
ifeq ($(CONFIGURATION),)
error_missing_configuration_file:
	$(error No configuration file specified. Use 'make CONFIGURATION=<path/to/configuration/file>' to build the system)
endif

# Initialize configuration variables from a build configuration file
include $(CONFIGURATION)

#--------------------------------------------------------------------------------------------------
# Set default configuration variables value if variables are not defined
#--------------------------------------------------------------------------------------------------
# Default tools
GLOBAL_TOOL_ASSEMBLER ?= nasm
GLOBAL_TOOL_COMPILER ?= gcc
GLOBAL_TOOL_LINKER ?= ld
GLOBAL_TOOL_ISO_GENERATOR ?= genisoimage

# Configuration variables
GLOBAL_SYSTEM_LANGUAGE ?= french
GLOBAL_PROCESSOR_TYPE ?= pentium
SYSTEM_IS_DEBUG_ENABLED ?= 0
SYSTEM_HARD_DISK_LOGICAL_BLOCK_ADDRESSING_MODE ?= 28
SYSTEM_HARD_DISK_DRIVER ?= ide
SYSTEM_ETHERNET_CONTROLLER_DRIVER ?= none
SYSTEM_RAM_SIZE ?= 16

# Make the build options available as a C string to be used by the system "version" command
STRING_BUILD_CONFIGURATION_VARIABLES = "\"GLOBAL_SYSTEM_LANGUAGE=$(GLOBAL_SYSTEM_LANGUAGE)\nGLOBAL_PROCESSOR_TYPE=$(GLOBAL_PROCESSOR_TYPE)\nSYSTEM_IS_DEBUG_ENABLED=$(SYSTEM_IS_DEBUG_ENABLED)\nSYSTEM_HARD_DISK_LOGICAL_BLOCK_ADDRESSING_MODE=$(SYSTEM_HARD_DISK_LOGICAL_BLOCK_ADDRESSING_MODE)\nSYSTEM_HARD_DISK_DRIVER=$(SYSTEM_HARD_DISK_DRIVER)\nSYSTEM_ETHERNET_CONTROLLER_DRIVER=$(SYSTEM_ETHERNET_CONTROLLER_DRIVER)\nSYSTEM_RAM_SIZE=$(SYSTEM_RAM_SIZE)\n\""

export GLOBAL_TOOL_ASSEMBLER
export GLOBAL_TOOL_COMPILER
export GLOBAL_TOOL_LINKER
export GLOBAL_TOOL_ISO_GENERATOR
export GLOBAL_PROCESSOR_TYPE
export SYSTEM_IS_DEBUG_ENABLED
export SYSTEM_HARD_DISK_LOGICAL_BLOCK_ADDRESSING_MODE
export SYSTEM_HARD_DISK_DRIVER
export SYSTEM_ETHERNET_CONTROLLER_DRIVER
export SYSTEM_RAM_SIZE
export STRING_BUILD_CONFIGURATION_VARIABLES
export SYSTEM_INSTALLER_FILES_LIST

# Choose the whole system and applications language
ifeq ($(GLOBAL_SYSTEM_LANGUAGE),english)
	LANGUAGE_DEFINE = -DCONFIGURATION_LANGUAGE_ENGLISH
else ifeq ($(GLOBAL_SYSTEM_LANGUAGE),italian)
	LANGUAGE_DEFINE = -DCONFIGURATION_LANGUAGE_ITALIAN
endif

# Default gcc flags
# Option -fno-asynchronous-unwind-tables avoid generating the .eh_frame section, which is heavy and useless here
# Option -fno-pic is needed since gcc 6.2 to avoid gcc generate Position Independant Code like a classic ELF file (this won't work as Lemon uses static binaries)
CCFLAGS = -fno-pic -nostdlib -fno-builtin -nostartfiles -finline-functions-called-once -fno-asynchronous-unwind-tables -masm=intel -m32 -Werror -march=$(GLOBAL_PROCESSOR_TYPE) -mtune=$(GLOBAL_PROCESSOR_TYPE) $(LANGUAGE_DEFINE)
export CCFLAGS

#--------------------------------------------------------------------------------------------------
# Private functions
#--------------------------------------------------------------------------------------------------
define DisplayTitle
	@printf "\033[32m################################################################################\n"
	@printf "## $1\n"
	@printf "################################################################################\033[0m\n"
endef

#--------------------------------------------------------------------------------------------------
# Rules
#--------------------------------------------------------------------------------------------------
# Applications must be built before system to allow them to be embedded in a RAM disk
all: clean libraries applications system installer

clean:
	@cd System && $(MAKE) CONFIGURATION_BUILD_INSTALLER=1 clean
	@cd Libraries && $(MAKE) clean
	@cd Applications && $(MAKE) clean
	@cd System && $(MAKE) clean

help:
	@$(call DisplayTitle,Configuration variables)
	@echo "Global variables affect the whole system (the operating system and all applications)."
	@echo "System variables affect only the operating system, not the applications."
	@echo
	@echo "- GLOBAL_TOOL_ASSEMBLER : which x86 assembler binary to use (useful to specify a cross-compilation toolchain binary)"
	@echo "    + nasm (default value)"
	@echo "- GLOBAL_TOOL_COMPILER : which C compiler binary to use (useful to specify a cross-compilation toolchain binary)"
	@echo "    + gcc (default value)"
	@echo "- GLOBAL_TOOL_LINKER : which object linker binary to use (useful to specify a cross-compilation toolchain binary)"
	@echo "    + ld (default value)"
	@echo "- GLOBAL_TOOL_ISO_GENERATOR : which CDROM ISO image binary to use (useful to specify a cross-compilation toolchain binary)"
	@echo "    + genisoimage (default value)"
	@echo "- GLOBAL_SYSTEM_LANGUAGE : system and applications language"
	@echo "    + english"
	@echo "    + french (default value)"
	@echo "    + italian"
	@echo "- GLOBAL_PROCESSOR_TYPE (see gcc's man page for x86 -march option for all available values) : the target processor"
	@echo "    + i486"
	@echo "    + pentium (default value)"
	@echo "    + pentium4"
	@echo "    + core2"
	@echo "    + nehalem"
	@echo "- SYSTEM_IS_DEBUG_ENABLED : display kernel debugging messages"
	@echo "    + 0 (default value)"
	@echo "    + 1"
	@echo "- SYSTEM_HARD_DISK_LOGICAL_BLOCK_ADDRESSING_MODE : the hard disk driver LBA mode"
	@echo "    + 28 (default value, used for hard disk size < 128GB)"
	@echo "    + 48 (mandatory for hard disk size > 128GB)"
	@echo "- SYSTEM_HARD_DISK_DRIVER : the hard disk driver to use"
	@echo "    + ide (default value)"
	@echo "    + ram"
	@echo "    + sata"
	@echo "- SYSTEM_ETHERNET_CONTROLLER_DRIVER : the driver to use for the network controller"
	@echo "    + none (default value, disabling ethernet controller)"
	@echo "    + 82540em (Intel PRO/1000 Network Internet Controller, also works for 82541PI)"
	@echo "    + rtl8111 (Realtek 8111, 8168, 8169)"
	@echo "- SYSTEM_RAM_SIZE : how many RAM can be used by the whole system (in mega bytes), use an integer value from 2 to 4096"
	@echo "    + 2 (minimum value)"
	@echo "    + 16 (default value)"
	@echo "    + 4096 (maximum value)"
	@echo

sdk:
	@printf "Preparing SDK directory...\n"
	@rm -rf $(SDK_PATH)
	@mkdir $(SDK_PATH)
	@printf "Copying the whole Applications directory...\n"
	@cp -r Applications $(SDK_PATH)
	@# Delete serial server binary to force the target computer to recompile it (and thus to avoid some incompatibilities)
	@rm -f $(SDK_PATH)/Applications/Tools/Serial_Port_Server.out
	@# Copy the shared rules needed to compile the applications
	@cp Global_Rules.mk $(SDK_PATH)
	@# Clean the Applications .32b and .o files
	@make -C $(SDK_PATH)/Applications clean > /dev/null
	@printf "Copying user relevant documentation...\n"
	@mkdir $(SDK_PATH)/Documentation
	@cp -r Documentation/Documentation_Libraries $(SDK_PATH)/Documentation
	@cp -r Documentation/User_Manual $(SDK_PATH)/Documentation
	@cp Documentation/Manual.html $(SDK_PATH)/Documentation
	@printf "Copying Libraries binaries and includes directories...\n"
	@mkdir $(SDK_PATH)/Libraries
	@cp -r Libraries/Binaries $(SDK_PATH)/Libraries
	@cp -r Libraries/Includes $(SDK_PATH)/Libraries
	@# Copy Error_Codes.h and System_Calls.h to Libraries includes directory
	@cp System/Includes/Error_Codes.h $(SDK_PATH)/Libraries/Includes
	@cp System/Includes/System_Calls.h $(SDK_PATH)/Libraries/Includes
	@printf "Copying installer CD image...\n"
	@cp Installer/Binaries/Lemon_Installer_CD_Image.iso $(SDK_PATH)
	@printf "\033[32mSDK successfully built.\033[0m\n"

# Copy the installation image to the floppy disk
floppy:
	@if [ ! -e Lemon_Installer_Floppy_Image.img ]; then printf "\033[31mThe installer floppy image has not been generated. Run 'make' before calling 'make floppy'.\033[0m\n"; false; fi
	@if [ $(shell id -u) != "0" ]; then printf "\033[31mYou must be root to execute this command.\033[0m\n"; false; fi
	@printf "Writing data to floppy...\n"
	@dd if=Installer/Binaries/Lemon_Installer_Floppy_Image.img of=/dev/fd0
	@printf "Reading back data...\n"
	@dd if=/dev/fd0 of=/tmp/Lemon_Floppy_Image.img
	@printf "Verifying data...\n"
	@# The diff command will return 1 if the images are different, making the rule fail
	@diff Installer/Binaries/Lemon_Installer_Floppy_Image.img /tmp/Lemon_Floppy_Image.img
	@printf "\033[32mFloppy image OK.\033[0m\n"

# Burn the installation ISO to a CD
cd:
	@if [ ! -e Lemon_Installer_CD_Image.iso ]; then printf "\033[31mThe installer ISO image has not been generated. Run 'make' before calling 'make cd'.\033[0m\n"; false; fi
	@if [ $(shell id -u) != "0" ]; then printf "\033[31mYou must be root to execute this command.\033[0m\n"; false; fi
	@-umount /media/ar/CDROM
	@wodim dev=/dev/sr0 blank=fast Lemon_Installer_CD_Image.iso gracetime=2

applications:
	@$(call DisplayTitle,Compiling applications)
	@cd Applications && $(MAKE)

# Access to a specific application rules (compile, clean and download)
application/%:
	cd Applications && $(MAKE) $*

installer:
	@$(call DisplayTitle,Creating installation image)
	@cd System && $(MAKE) CONFIGURATION_BUILD_INSTALLER=1

libraries:
	@$(call DisplayTitle,Compiling libraries)
	@cd Libraries && $(MAKE)

system:
	@$(call DisplayTitle,Compiling system)
	@cd System && $(MAKE) CONFIGURATION_BUILD_INSTALLER=0
