# Main makefile
# Author : Adrien RICCIARDI
SDK_PATH = ../Lemon_SDK

# Some systems want "echo -e" to display colors, some do not want
ECHO = echo -e

all: clean
	@$(ECHO) "\033[32m--------------------------------------------------------------------------------"
	@$(ECHO) "-- Compile system"
	@$(ECHO) "--------------------------------------------------------------------------------\033[0m"
	@cd System && $(MAKE)
	@$(ECHO) "\033[32m--------------------------------------------------------------------------------"
	@$(ECHO) "-- Compile libraries"
	@$(ECHO) "--------------------------------------------------------------------------------\033[0m"
	@cd Libraries && $(MAKE)
	@$(ECHO) "\033[32m--------------------------------------------------------------------------------"
	@$(ECHO) "-- Compile applications"
	@$(ECHO) "--------------------------------------------------------------------------------\033[0m"
	@cd Applications && $(MAKE)
	@$(ECHO) "\033[32m--------------------------------------------------------------------------------"
	@$(ECHO) "-- Create installation image"
	@$(ECHO) "--------------------------------------------------------------------------------\033[0m"
	@cd Installer && $(MAKE)


# Choose the whole system and applications language
english: CCFLAGS += -DLEMON_LANGUAGE_ENGLISH
english: all

italian: CCFLAGS += -DLEMON_LANGUAGE_ITALIAN
italian: all

export CCFLAGS

sdk:
	@$(ECHO) "-> Prepare SDK directory..."
	@rm -rf $(SDK_PATH)
	@mkdir $(SDK_PATH)
	@$(ECHO) "-> Copy the whole Applications directory..."
	@cp -r Applications $(SDK_PATH)
	@# Delete serial server binary to force the target computer to recompile it (and thus to avoid some incompatibilities)
	@rm $(SDK_PATH)/Applications/Tools/Serial_Port_Server
	@# Clean the Applications .32b and .o files
	@make -C $(SDK_PATH)/Applications clean > /dev/null
	@$(ECHO) "-> Copy user relevant documentation..."
	@mkdir $(SDK_PATH)/Documentation
	@cp -r Documentation/Documentation_Libraries $(SDK_PATH)/Documentation
	@cp -r Documentation/User_Manual $(SDK_PATH)/Documentation
	@cp Documentation/Manual.html $(SDK_PATH)/Documentation
	@$(ECHO) "-> Copy Libraries binaries and includes directories..."
	@mkdir $(SDK_PATH)/Libraries
	@cp -r Libraries/Binaries $(SDK_PATH)/Libraries
	@cp -r Libraries/Includes $(SDK_PATH)/Libraries
	@# Copy System_Calls.h to Librairies includes directory
	@cp System/Includes/System_Calls.h $(SDK_PATH)/Libraries/Includes
	@$(ECHO) "-> Copy installer CD image..."
	@cp Installer/Binaries/Lemon_Installer_CD_Image.iso $(SDK_PATH)
	@$(ECHO) "### SDK successfully built ###"

clean:
	@cd Installer && $(MAKE) clean
	@cd Libraries && $(MAKE) clean
	@cd Applications && $(MAKE) clean
	@cd System && $(MAKE) clean
