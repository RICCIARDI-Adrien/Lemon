# Applications main makefile.
# Author : Adrien RICCIARDI
# Version 1.0 : 02/05/2014
BINARIES_PATH = $(realpath Binaries)
LIBRARIES_PATH = $(realpath ../Libraries)
OBJECTS_PATH = $(realpath Objects)
SOURCES_PATH = $(realpath Sources)
SYSTEM_PATH = $(realpath ../System)
TOOLS_PATH = $(realpath Tools)

CC = gcc
LD = ld
SERIAL_PORT_SERVER = $(TOOLS_PATH)/Serial_Port_Server.out
# Some systems want "echo -e" to display colors, some do not want
ECHO = echo -e

CCFLAGS += -nostdlib -fno-builtin -masm=intel -m32 -W -Wall -Wunused -Os -fno-asynchronous-unwind-tables -I$(LIBRARIES_PATH)/Includes -I$(SYSTEM_PATH)/Includes
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
# Global rules
#------------------------------------------------------------------------------------------------------------------------------
all: benchmark brain_calculation game_of_life hangman help no_match_dealer numbers rain tests text_viewer unix_commands

clean: benchmark_clean brain_calculation_clean game_of_life_clean hangman_clean help_clean no_match_dealer_clean numbers_clean rain_clean tests_clean text_viewer_clean unix_commands_clean
	# Delete also the tools
	rm -rf $(SERIAL_PORT_SERVER)

# Compile the server program if it is not done yet
$(SERIAL_PORT_SERVER): $(TOOLS_PATH)/Serial_Port_Server.c
	gcc -W -Wall $(TOOLS_PATH)/Serial_Port_Server.c -o $(SERIAL_PORT_SERVER)

#------------------------------------------------------------------------------------------------------------------------------
# Benchmark
#------------------------------------------------------------------------------------------------------------------------------
benchmark:
	@$(ECHO) "\033[33mCompiling Benchmark...\033[0m"
	@cd $(SOURCES_PATH)/Benchmark && $(MAKE)

benchmark_clean:
	@cd $(SOURCES_PATH)/Benchmark && $(MAKE) clean

benchmark_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Benchmark.32b benchmark

#------------------------------------------------------------------------------------------------------------------------------
# Brain calculation
#------------------------------------------------------------------------------------------------------------------------------
brain_calculation:
	@$(ECHO) "\033[33mCompiling Brain Calculation...\033[0m"
	@cd $(SOURCES_PATH)/Brain_Calculation && $(MAKE)

brain_calculation_clean:
	@cd $(SOURCES_PATH)/Brain_Calculation && $(MAKE) clean

brain_calculation_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Brain_Calculation.32b brain

#------------------------------------------------------------------------------------------------------------------------------
# Game of Life
#------------------------------------------------------------------------------------------------------------------------------
game_of_life:
	@$(ECHO) "\033[33mCompiling Game of Life...\033[0m"
	@cd $(SOURCES_PATH)/Game_Of_Life && $(MAKE)

game_of_life_clean:
	@cd $(SOURCES_PATH)/Game_Of_Life && $(MAKE) clean

game_of_life_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Game_Of_Life.32b life

#------------------------------------------------------------------------------------------------------------------------------
# Hangman
#------------------------------------------------------------------------------------------------------------------------------
hangman:
	@$(ECHO) "\033[33mCompiling Hangman...\033[0m"
	@cd $(SOURCES_PATH)/Hangman && $(MAKE)

hangman_clean:
	@cd $(SOURCES_PATH)/Hangman && $(MAKE) clean

hangman_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Hangman.32b hangman

#------------------------------------------------------------------------------------------------------------------------------
# Help
#------------------------------------------------------------------------------------------------------------------------------
help:
	@$(ECHO) "\033[33mCompiling Help...\033[0m"
	@cd $(SOURCES_PATH)/Help && $(MAKE)

help_clean:
	@cd $(SOURCES_PATH)/Help && $(MAKE) clean

help_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Help.32b help

#------------------------------------------------------------------------------------------------------------------------------
# No match dealer
#------------------------------------------------------------------------------------------------------------------------------
no_match_dealer:
	@$(ECHO) "\033[33mCompiling No match dealer...\033[0m"
	@cd $(SOURCES_PATH)/No_Match_Dealer && $(MAKE)

no_match_dealer_clean:
	@cd $(SOURCES_PATH)/No_Match_Dealer && $(MAKE) clean

no_match_dealer_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/No_Match_Dealer.32b dealer

#------------------------------------------------------------------------------------------------------------------------------
# Numbers
#------------------------------------------------------------------------------------------------------------------------------
numbers:
	@$(ECHO) "\033[33mCompiling Numbers...\033[0m"
	@cd $(SOURCES_PATH)/Numbers && $(MAKE)

numbers_clean:
	@cd $(SOURCES_PATH)/Numbers && $(MAKE) clean

numbers_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Numbers.32b numbers

#------------------------------------------------------------------------------------------------------------------------------
# Rain
#------------------------------------------------------------------------------------------------------------------------------
rain:
	@$(ECHO) "\033[33mCompiling Rain...\033[0m"
	@cd $(SOURCES_PATH)/Rain && $(MAKE)

rain_clean:
	@cd $(SOURCES_PATH)/Rain && $(MAKE) clean

rain_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Rain.32b rain

#------------------------------------------------------------------------------------------------------------------------------
# Tests
#------------------------------------------------------------------------------------------------------------------------------
tests:
	@$(ECHO) "\033[33mCompiling Tests...\033[0m"
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
# Text viewer
#------------------------------------------------------------------------------------------------------------------------------
text_viewer:
	@$(ECHO) "\033[33mCompiling Text viewer...\033[0m"
	@cd $(SOURCES_PATH)/Text_Viewer && $(MAKE)

text_viewer_clean:
	@cd $(SOURCES_PATH)/Text_Viewer && $(MAKE) clean

text_viewer_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/Text_Viewer.32b text

#------------------------------------------------------------------------------------------------------------------------------
# UNIX commands
#------------------------------------------------------------------------------------------------------------------------------
unix_commands:
	@$(ECHO) "\033[33mCompiling UNIX commands...\033[0m"
	@cd $(SOURCES_PATH)/UNIX_Commands && $(MAKE)

unix_commands_clean:
	@cd $(SOURCES_PATH)/UNIX_Commands && $(MAKE) clean

unix_commands_download: $(SERIAL_PORT_SERVER)
	@$(SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/UNIX_Commands.32b u
