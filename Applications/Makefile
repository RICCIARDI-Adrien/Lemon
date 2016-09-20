# Applications main makefile.
# Author : Adrien RICCIARDI
BINARIES_PATH = $(realpath Binaries)
LIBRARIES_PATH = $(realpath ../Libraries)
OBJECTS_PATH = $(realpath Objects)
SOURCES_PATH = $(realpath Sources)
SYSTEM_PATH = $(realpath ../System)
TOOLS_PATH = $(realpath Tools)

SERIAL_PORT_SERVER = $(TOOLS_PATH)/Serial_Port_Server.out

CCFLAGS += -W -Wall -Wunused -I$(LIBRARIES_PATH)/Includes -I$(SYSTEM_PATH)/Includes
LDFLAGS = -T $(TOOLS_PATH)/Linker_Script.ld --strip-all -s -L$(LIBRARIES_PATH)/Binaries -l:Libraries.a
SERIAL_PORT_SERVER_FLAGS = /tmp/Lemon_Serial_Port_Pipe

# The following variables are shared by all sub-makefiles
export LEMON_TOOL_COMPILER
export LEMON_TOOL_LINKER
export BINARIES_PATH
export LIBRARIES_PATH
export OBJECTS_PATH
export TOOLS_PATH
export CCFLAGS
export LDFLAGS

#------------------------------------------------------------------------------------------------------------------------------
# Private functions
#------------------------------------------------------------------------------------------------------------------------------
# Prepare the build environment for the program compilation and display the name of the program being compiled
# @param $(1) The program name to display (can contain spaces)
# @param $(2) The build directory name (must be a valid file system path)
define StartProgramBuild
	@# Display the name of the program being compiled
	@printf "\033[33mCompiling $(1)...\033[0m\n"
	@# Create the program build directory
	@mkdir -p $(OBJECTS_PATH)/$(2)
endef

# Append the Lemon file header telling the system that the program is executable
# @param $(1) The executable file name without file extension
define AppendExecutableHeader
	@cat $(TOOLS_PATH)/Lemon_Executable_File_Header.dat > $(BINARIES_PATH)/$(1).lemon
	@cat $(BINARIES_PATH)/$(1).bin >> $(BINARIES_PATH)/$(1).lemon
endef

#------------------------------------------------------------------------------------------------------------------------------
# Global rules
#------------------------------------------------------------------------------------------------------------------------------
all: benchmark games help network_chat tests text_editor unix_commands

clean: benchmark_clean games_clean help_clean network_chat_clean tests_clean text_editor_clean unix_commands_clean
	# Delete also the tools
	rm -rf $(SERIAL_PORT_SERVER)

# Compile the server program if it is not done yet
$(SERIAL_PORT_SERVER): $(TOOLS_PATH)/Serial_Port_Server.c
	gcc -W -Wall $(TOOLS_PATH)/Serial_Port_Server.c -o $(SERIAL_PORT_SERVER)

#------------------------------------------------------------------------------------------------------------------------------
# Benchmark
#------------------------------------------------------------------------------------------------------------------------------
benchmark:
	@$(call StartProgramBuild,Benchmark,Benchmark)
	@cd $(SOURCES_PATH)/Benchmark && $(MAKE)
	@$(call AppendExecutableHeader,Benchmark)

benchmark_clean:
	@cd $(SOURCES_PATH)/Benchmark && $(MAKE) clean

benchmark_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Benchmark.lemon benchmark

#------------------------------------------------------------------------------------------------------------------------------
# Games
#------------------------------------------------------------------------------------------------------------------------------
games:
	@$(call StartProgramBuild,Games,Games)
	@cd $(SOURCES_PATH)/Games && $(MAKE)
	@$(call AppendExecutableHeader,Games)

games_clean:
	@cd $(SOURCES_PATH)/Games && $(MAKE) clean

games_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Games.lemon games

#------------------------------------------------------------------------------------------------------------------------------
# Help
#------------------------------------------------------------------------------------------------------------------------------
help:
	@$(call StartProgramBuild,Help,Help)
	@cd $(SOURCES_PATH)/Help && $(MAKE)
	@$(call AppendExecutableHeader,Help)

help_clean:
	@cd $(SOURCES_PATH)/Help && $(MAKE) clean

help_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Help.lemon help

#------------------------------------------------------------------------------------------------------------------------------
# Network_Chat
#------------------------------------------------------------------------------------------------------------------------------
network_chat:
	@$(call StartProgramBuild,Network Chat,Network_Chat)
	@cd $(SOURCES_PATH)/Network_Chat && $(MAKE)
	@$(call AppendExecutableHeader,Network_Chat)

network_chat_clean:
	@cd $(SOURCES_PATH)/Network_Chat && $(MAKE) clean

network_chat_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Network_Chat.lemon chat


#------------------------------------------------------------------------------------------------------------------------------
# Tests
#------------------------------------------------------------------------------------------------------------------------------
tests:
	@$(call StartProgramBuild,Tests,Tests)
	@cd $(SOURCES_PATH)/Tests && $(MAKE)
	@$(call AppendExecutableHeader,Test_Display_ASCII_Characters)
	@$(call AppendExecutableHeader,Test_Protection_Check)
	@$(call AppendExecutableHeader,Tests)

tests_clean:
	@cd $(SOURCES_PATH)/Tests && $(MAKE) clean

tests_download_test_display_ascii_characters: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Test_Display_ASCII_Characters.lemon test_ascii

tests_download_test_protection_check: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Test_Protection_Check.lemon test_prot

tests_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Tests.lemon tests
	
#------------------------------------------------------------------------------------------------------------------------------
# Text editor
#------------------------------------------------------------------------------------------------------------------------------
text_editor:
	@$(call StartProgramBuild,Text editor,Text_Editor)
	@cd $(SOURCES_PATH)/Text_Editor && $(MAKE)
	@$(call AppendExecutableHeader,Text_Editor)

text_editor_clean:
	@cd $(SOURCES_PATH)/Text_Editor && $(MAKE) clean

text_editor_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Text_Editor.lemon edit

#------------------------------------------------------------------------------------------------------------------------------
# UNIX commands
#------------------------------------------------------------------------------------------------------------------------------
unix_commands:
	@$(call StartProgramBuild,UNIX commands,UNIX_Commands)
	@cd $(SOURCES_PATH)/UNIX_Commands && $(MAKE)
	@$(call AppendExecutableHeader,UNIX_Commands)

unix_commands_clean:
	@cd $(SOURCES_PATH)/UNIX_Commands && $(MAKE) clean

unix_commands_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/UNIX_Commands.lemon u
