# Main makefile
# Author : Adrien RICCIARDI
SDK_PATH = ../Lemon_SDK

# Retrieve Kconfig-generated variables and import the ones that are set in a makefile variables list
KCONFIG_VARIABLES = $(shell if [ -e .config ]; then cat .config | sed '/\#/d' | sed '/^$$/d' | sed 's/CONFIG_/CONFIGURATION_/g'; fi)

# Determine host machine processors count
HOST_PROCESSORS_COUNT = $(shell cat /proc/cpuinfo | grep processor | wc -l)

#--------------------------------------------------------------------------------------------------
# Set default configuration variables value if variables are not defined
#--------------------------------------------------------------------------------------------------
# Default tools
GLOBAL_TOOL_ASSEMBLER ?= nasm
GLOBAL_TOOL_COMPILER ?= gcc
GLOBAL_TOOL_LINKER ?= ld
GLOBAL_TOOL_ISO_GENERATOR ?= genisoimage

# Configuration variables
GLOBAL_PROCESSOR_CODE_NAME = $(patsubst CONFIGURATION_GLOBAL_PROCESSOR_CODE_NAME="%",%,$(filter CONFIGURATION_GLOBAL_PROCESSOR_CODE_NAME=%,$(KCONFIG_VARIABLES)))
STRING_GIT_COMMIT_HASH = $(shell git log --format=%H -1)

export GLOBAL_TOOL_ASSEMBLER
export GLOBAL_TOOL_COMPILER
export GLOBAL_TOOL_LINKER
export GLOBAL_TOOL_ISO_GENERATOR
export KCONFIG_VARIABLES

# Default gcc flags
# Option -fno-asynchronous-unwind-tables avoid generating the .eh_frame section, which is heavy and useless here
# Option -fno-pic is needed since gcc 6.2 to avoid gcc generate Position Independant Code like a classic ELF file (this won't work as Lemon uses static binaries)
# Option -Wno-address-of-packed-member is needed since gcc 9.2, otherwise multiple "taking address of packed member of ‘struct <anonymous>’ may result in an unaligned pointer value" warnings will be triggered
CCFLAGS = -W -Wall -fno-pic -nostdlib -fno-builtin -nostartfiles -finline-functions-called-once -fno-asynchronous-unwind-tables -masm=intel -m32 -Werror -Wno-address-of-packed-member -march=$(GLOBAL_PROCESSOR_CODE_NAME) -mtune=$(GLOBAL_PROCESSOR_CODE_NAME) -DSTRING_GIT_COMMIT_HASH="\"$(STRING_GIT_COMMIT_HASH)\"" $(subst CONFIGURATION_,-DCONFIGURATION_,$(KCONFIG_VARIABLES))
export CCFLAGS

#--------------------------------------------------------------------------------------------------
# Private functions
#--------------------------------------------------------------------------------------------------
define DisplayTitle
	@printf "\033[32m################################################################################\n"
	@printf "## $1\n"
	@printf "################################################################################\033[0m\n"
endef

define GenerateConfigurationTemplate =
.PHONY: Configuration/$(1)
Configuration/$(1):
	cp Configurations/$(1) $(PWD)/.config
endef

#--------------------------------------------------------------------------------------------------
# Rules
#--------------------------------------------------------------------------------------------------
.PHONY: all cd check_configuration clean floppy menuconfig sdk qemu qemu-install

# Applications must be built before system to allow them to be embedded in a RAM disk
all: check_configuration clean libraries applications
	@# Build system
	@$(call DisplayTitle,Compiling system)
	@cd System && $(MAKE) CONFIGURATION_BUILD_INSTALLER=0
	@# Build installation image
	@$(call DisplayTitle,Creating installation image)
	@cd System && $(MAKE) CONFIGURATION_BUILD_INSTALLER=1

# Check for configuration file presence
check_configuration:
ifeq ($(KCONFIG_VARIABLES),)
	@printf "\033[31mNo configuration file present. Please run 'make menuconfig' or TODO for default configurations.\033[0m\n"
	@false
endif

clean:
	@cd System && $(MAKE) CONFIGURATION_BUILD_INSTALLER=1 clean
	@cd Libraries && $(MAKE) clean
	@cd Applications && $(MAKE) clean
	@cd System && $(MAKE) clean

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
	@dd if=Lemon_Installer_Floppy_Image.img of=/dev/fd0
	@printf "Reading back data...\n"
	@dd if=/dev/fd0 of=/tmp/Lemon_Floppy_Image.img
	@printf "Verifying data...\n"
	@# The diff command will return 1 if the images are different, making the rule fail
	@diff Lemon_Installer_Floppy_Image.img /tmp/Lemon_Floppy_Image.img
	@printf "\033[32mFloppy image OK.\033[0m\n"

# Burn the installation ISO to a CD
cd:
	@if [ ! -e Lemon_Installer_CD_Image.iso ]; then printf "\033[31mThe installer ISO image has not been generated. Run 'make' before calling 'make cd'.\033[0m\n"; false; fi
	@if [ $(shell id -u) != "0" ]; then printf "\033[31mYou must be root to execute this command.\033[0m\n"; false; fi
	@-umount /media/ar/CDROM
	@wodim dev=/dev/sr0 blank=fast Lemon_Installer_CD_Image.iso gracetime=2

applications: check_configuration
	@$(call DisplayTitle,Compiling applications)
	@cd Applications && $(MAKE)

# Access to a specific application rules (compile, clean and download)
application/%: check_configuration
	cd Applications && $(MAKE) $*

libraries: check_configuration
	@$(call DisplayTitle,Compiling libraries)
	@cd Libraries && $(MAKE) -j $(HOST_PROCESSORS_COUNT)

# Create hard disk image if not present
QEMU_Hard_Disk.img:
	dd if=/dev/zero of=QEMU_Hard_Disk.img bs=1M count=512

qemu: QEMU_Hard_Disk.img
	@# Emulated PC configuration : 16 MB of RAM, Intel 82540EM PCI network card, IDE hard disk
	qemu-system-i386 -m 16M -device e1000 -name Lemon -drive file=QEMU_Hard_Disk.img,media=disk,format=raw $(QEMU_OPTIONS)

qemu-install: QEMU_OPTIONS += -cdrom Lemon_Installer_CD_Image.iso -boot order=d
qemu-install: qemu

# Install "kconfig-frontends" Debian package to get "kconfig-mconf" program
menuconfig:
	kconfig-mconf Kconfig

# Generate all configuration rules
CONFIGURATIONS_LIST = $(notdir $(shell find Configurations -name "*.config"))
$(foreach CONFIGURATION_NAME,$(CONFIGURATIONS_LIST),$(eval $(call GenerateConfigurationTemplate,$(CONFIGURATION_NAME))))
