# Applications main makefile.
# Author : Adrien RICCIARDI
BINARIES_PATH = $(realpath Binaries)
LIBRARIES_PATH = $(realpath ../Libraries)
OBJECTS_PATH = $(realpath Objects)
SOURCES_PATH = $(realpath Sources)
SYSTEM_PATH = $(realpath ../System)
TOOLS_PATH = $(realpath Tools)

TOOL_SERIAL_PORT_SERVER = $(TOOLS_PATH)/Serial_Port_Server.out
TOOL_ADD_EXECUTABLE_HEADER = $(TOOLS_PATH)/Add_Executable_Header.sh

CCFLAGS += -W -Wall -Wunused -I$(LIBRARIES_PATH)/Includes -I$(SYSTEM_PATH)/Includes
LDFLAGS = -T $(TOOLS_PATH)/Linker_Script.ld --strip-all -s -L$(LIBRARIES_PATH)/Binaries -l:Libraries.a
SERIAL_PORT_SERVER_FLAGS = /tmp/Lemon_Serial_Port_Pipe

# Find all available applications (-mindepth 1 allows to remove the '.' entry)
FOUND_APPLICATION_DIRECTORIES_1 = $(shell cd $(SOURCES_PATH) && find . -mindepth 1 -maxdepth 1 -type d)
# Remove the "./" preceding each name
FOUND_APPLICATION_DIRECTORIES_2 = $(subst ./,,$(FOUND_APPLICATION_DIRECTORIES_1))
# Sort the names to build in order
APPLICATIONS = $(sort $(FOUND_APPLICATION_DIRECTORIES_2))

# The following variables are shared by all sub-makefiles
export LEMON_TOOL_COMPILER
export LEMON_TOOL_LINKER
export BINARIES_PATH
export LIBRARIES_PATH
export OBJECTS_PATH
export TOOLS_PATH
export TOOL_SERIAL_PORT_SERVER
export TOOL_ADD_EXECUTABLE_HEADER
export CCFLAGS
export LDFLAGS
export SERIAL_PORT_SERVER_FLAGS

#------------------------------------------------------------------------------------------------------------------------------
# Rules
#------------------------------------------------------------------------------------------------------------------------------
# Append the /compile target to all discovered applications
all: $(patsubst %,%/compile,$(APPLICATIONS))

# Append the /clean target to all discovered applications
clean: $(patsubst %,%/clean,$(APPLICATIONS))
	# Delete also the tools
	rm -rf $(SERIAL_PORT_SERVER)

%/compile:
	@# Display the name of the application being compiled
	@printf "\033[33mCompiling $*...\033[0m\n"
	@# Create the application build directory
	@mkdir -p $(OBJECTS_PATH)/$*
	@# Build the application
	@cd $(SOURCES_PATH)/$* && $(MAKE)

%/clean:
	@cd $(SOURCES_PATH)/$* && $(MAKE) clean

%/download: $(TOOL_SERIAL_PORT_SERVER)
	@cd $(SOURCES_PATH)/$* && $(MAKE) download

# Compile the server program if it is not done yet
$(TOOL_SERIAL_PORT_SERVER): $(TOOLS_PATH)/Serial_Port_Server.c
	gcc -W -Wall $(TOOLS_PATH)/Serial_Port_Server.c -o $(TOOL_SERIAL_PORT_SERVER)
