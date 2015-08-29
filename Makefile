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

# Choose the whole system and applications language
english: CCFLAGS += -DLEMON_LANGUAGE_ENGLISH
english: all

italian: CCFLAGS += -DLEMON_LANGUAGE_ITALIAN
italian: all

export CCFLAGS

sdk:
	@printf "-> Prepare SDK directory..."
	@rm -rf $(SDK_PATH)
	@mkdir $(SDK_PATH)
	@printf "-> Copy the whole Applications directory..."
	@cp -r Applications $(SDK_PATH)
	@# Delete serial server binary to force the target computer to recompile it (and thus to avoid some incompatibilities)
	@rm $(SDK_PATH)/Applications/Tools/Serial_Port_Server.out
	@# Clean the Applications .32b and .o files
	@make -C $(SDK_PATH)/Applications clean > /dev/null
	@printf "-> Copy user relevant documentation..."
	@mkdir $(SDK_PATH)/Documentation
	@cp -r Documentation/Documentation_Libraries $(SDK_PATH)/Documentation
	@cp -r Documentation/User_Manual $(SDK_PATH)/Documentation
	@cp Documentation/Manual.html $(SDK_PATH)/Documentation
	@printf "-> Copy Libraries binaries and includes directories..."
	@mkdir $(SDK_PATH)/Libraries
	@cp -r Libraries/Binaries $(SDK_PATH)/Libraries
	@cp -r Libraries/Includes $(SDK_PATH)/Libraries
	@# Copy Error_Codes.h and System_Calls.h to Librairies includes directory
	@cp System/Includes/Error_Codes.h $(SDK_PATH)/Libraries/Includes
	@cp System/Includes/System_Calls.h $(SDK_PATH)/Libraries/Includes
	@printf "-> Copy installer CD image..."
	@cp Installer/Binaries/Lemon_Installer_CD_Image.iso $(SDK_PATH)
	@printf "### SDK successfully built ###"

# Copy the installation image to the floppy disk
floppy:
	@if (test ! -e Installer/Binaries/Lemon_Installer_Floppy_Image.img) then printf "\033[31mThe installer floppy image has not been generated. Run \'make\' before calling \'make floppy\'.\033[0m\n"; false; fi
	@sudo dd if=Installer/Binaries/Lemon_Installer_Floppy_Image.img of=/dev/fd0

clean:
	@cd Installer && $(MAKE) clean
	@cd Libraries && $(MAKE) clean
	@cd Applications && $(MAKE) clean
	@cd System && $(MAKE) clean
