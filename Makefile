# Main makefile
# Author : Adrien RICCIARDI
SDK_PATH = ../Lemon_SDK

define DisplayTitle
	@printf "\033[32m################################################################################\n"
	@printf "## $1\n"
	@printf "################################################################################\033[0m\n"
endef

all: clean
	@$(call DisplayTitle,Compiling system)
	@cd System && $(MAKE)
	@$(call DisplayTitle,Compiling libraries)
	@cd Libraries && $(MAKE)
	@$(call DisplayTitle,Compiling applications)
	@cd Applications && $(MAKE)
	@$(call DisplayTitle,Creating installation image)
	@cd Installer && $(MAKE)

help:
	@$(call DisplayTitle,Configuration variables)
	@echo "Global variables affect the whole system (the operating system and all applications)."
	@echo "System variables affect only the operating system, not the applications."
	@echo
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
	@echo "- SYSTEM_HARD_DISK_TYPE : choose on which hard disk type the system will be installed"
	@echo "    + ide  (default value)"
	@echo "    + sata"
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
	@if [ ! -e Installer/Binaries/Lemon_Installer_Floppy_Image.img ]; then printf "\033[31mThe installer floppy image has not been generated. Run 'make' before calling 'make floppy'.\033[0m\n"; false; fi
	@printf "Writing data to floppy...\n"
	@sudo dd if=Installer/Binaries/Lemon_Installer_Floppy_Image.img of=/dev/fd0
	@printf "Reading back data...\n"
	@sudo dd if=/dev/fd0 of=/tmp/Lemon_Floppy_Image.img
	@printf "Verifying data...\n"
	@# The diff command will return 1 if the images are different, making the rule fail
	@diff Installer/Binaries/Lemon_Installer_Floppy_Image.img /tmp/Lemon_Floppy_Image.img
	@printf "\033[32mFloppy image OK.\033[0m\n"

clean:
	@cd Installer && $(MAKE) clean
	@cd Libraries && $(MAKE) clean
	@cd Applications && $(MAKE) clean
	@cd System && $(MAKE) clean
