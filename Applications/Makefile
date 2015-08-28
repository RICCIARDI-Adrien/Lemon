# Applications main makefile.
# Author : Adrien RICCIARDI
BINARIES_PATH = $(realpath Binaries)
LIBRARIES_PATH = $(realpath ../Libraries)
OBJECTS_PATH = $(realpath Objects)
SOURCES_PATH = $(realpath Sources)
SYSTEM_PATH = $(realpath ../System)
TOOLS_PATH = $(realpath Tools)

CC = gcc
LD = ld
SERIAL_PORT_SERVER = $(TOOLS_PATH)/Serial_Port_Server.out

CCFLAGS += -nostdlib -fno-builtin -masm=intel -m32 -W -Wall -Wunused -fno-asynchronous-unwind-tables -I$(LIBRARIES_PATH)/Includes -I$(SYSTEM_PATH)/Includes
LDFLAGS = -T $(TOOLS_PATH)/Linker_Script.ld --strip-all -s -L$(LIBRARIES_PATH)/Binaries -l:Libraries.a
SERIAL_PORT_SERVER_FLAGS = /tmp/Lemon_Serial_Port_Pipe

# The following variables are shared by all sub-makefiles
export CC
export LD
export BINARIES_PATH
export LIBRARIES_PATH
export OBJECTS_PATH
export TOOLS_PATH
export CCFLAGS
export LDFLAGS

#------------------------------------------------------------------------------------------------------------------------------
# Private functions
#------------------------------------------------------------------------------------------------------------------------------
define DisplayProgramToCompile
	@printf "\033[33mCompiling $1...\033[0m\n"
endef

#------------------------------------------------------------------------------------------------------------------------------
# Global rules
#------------------------------------------------------------------------------------------------------------------------------
all: benchmark games help tests text_viewer unix_commands

clean: benchmark_clean games_clean help_clean tests_clean text_viewer_clean unix_commands_clean
	# Delete also the tools
	rm -rf $(SERIAL_PORT_SERVER)

# Compile the server program if it is not done yet
$(SERIAL_PORT_SERVER): $(TOOLS_PATH)/Serial_Port_Server.c
	gcc -W -Wall $(TOOLS_PATH)/Serial_Port_Server.c -o $(SERIAL_PORT_SERVER)

#------------------------------------------------------------------------------------------------------------------------------
# Benchmark
#------------------------------------------------------------------------------------------------------------------------------
benchmark:
	@$(call DisplayProgramToCompile,Benchmark)
	@cd $(SOURCES_PATH)/Benchmark && $(MAKE)

benchmark_clean:
	@cd $(SOURCES_PATH)/Benchmark && $(MAKE) clean

benchmark_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Benchmark.32b benchmark

#------------------------------------------------------------------------------------------------------------------------------
# Games
#------------------------------------------------------------------------------------------------------------------------------
games:
	@$(call DisplayProgramToCompile,Games)
	@cd $(SOURCES_PATH)/Games && $(MAKE)

games_clean:
	@cd $(SOURCES_PATH)/Games && $(MAKE) clean

games_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Games.32b games

#------------------------------------------------------------------------------------------------------------------------------
# Help
#------------------------------------------------------------------------------------------------------------------------------
help:
	@$(call DisplayProgramToCompile,Help)
	@cd $(SOURCES_PATH)/Help && $(MAKE)

help_clean:
	@cd $(SOURCES_PATH)/Help && $(MAKE) clean

help_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Help.32b help

#------------------------------------------------------------------------------------------------------------------------------
# Tests
#------------------------------------------------------------------------------------------------------------------------------
tests:
	@$(call DisplayProgramToCompile,Tests)
	@cd $(SOURCES_PATH)/Tests && $(MAKE)

tests_clean:
	@cd $(SOURCES_PATH)/Tests && $(MAKE) clean

tests_download_test_display_ascii_characters: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Test_Display_ASCII_Characters.32b test_ascii

tests_download_test_protection_check: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Test_Protection_Check.32b test_prot

tests_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Tests.32b tests
	
#------------------------------------------------------------------------------------------------------------------------------
# Text editor
#------------------------------------------------------------------------------------------------------------------------------
text_editor:
	@$(call DisplayProgramToCompile,Text editor)
	@cd $(SOURCES_PATH)/Text_Editor && $(MAKE)

text_editor_clean:
	@cd $(SOURCES_PATH)/Text_Editor && $(MAKE) clean

text_editor_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Text_Editor.32b edit

#------------------------------------------------------------------------------------------------------------------------------
# Text viewer
#------------------------------------------------------------------------------------------------------------------------------
text_viewer:
	@$(call DisplayProgramToCompile,Text viewer)
	@cd $(SOURCES_PATH)/Text_Viewer && $(MAKE)

text_viewer_clean:
	@cd $(SOURCES_PATH)/Text_Viewer && $(MAKE) clean

text_viewer_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Text_Viewer.32b text

#------------------------------------------------------------------------------------------------------------------------------
# UNIX commands
#------------------------------------------------------------------------------------------------------------------------------
unix_commands:
	@$(call DisplayProgramToCompile,UNIX commands)
	@cd $(SOURCES_PATH)/UNIX_Commands && $(MAKE)

unix_commands_clean:
	@cd $(SOURCES_PATH)/UNIX_Commands && $(MAKE) clean

unix_commands_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/UNIX_Commands.32b u
