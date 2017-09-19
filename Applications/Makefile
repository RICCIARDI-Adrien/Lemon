# Applications main makefile.
# Author : Adrien RICCIARDI
BINARIES_PATH = $(realpath Binaries)
LIBRARIES_PATH = $(realpath ../Libraries)
OBJECTS_PATH = $(realpath Objects)
SOURCES_PATH = $(realpath Sources)
SYSTEM_PATH = $(realpath ../System)
TOOLS_PATH = $(realpath Tools)

TOOL_SERIAL_PORT_SERVER = $(TOOLS_PATH)/Serial_Port_Server.out

# The libgcc library provides support functions for native C operations that are not supported by the hardware (such as long long arithmetic on x86). Provide this feature only to applications for now (system does not need it, nor Libraries)
GCC_LIBRARY_PATH_AND_FILE_NAME = $(shell $(GLOBAL_TOOL_COMPILER) -m32 -print-libgcc-file-name)
# Remove the library file name to keep only the path
GCC_LIBRARY_PATH = $(subst /libgcc.a,,$(GCC_LIBRARY_PATH_AND_FILE_NAME))

CCFLAGS += -W -Wall -Wunused -I$(LIBRARIES_PATH)/Includes -I$(SYSTEM_PATH)/Includes
LDFLAGS = -T $(TOOLS_PATH)/Linker_Script.ld --strip-all -s -L$(LIBRARIES_PATH)/Binaries -l:Libraries.a -L$(GCC_LIBRARY_PATH) -l:libgcc.a

# Find all available applications (-mindepth 1 allows to remove the '.' entry)
FOUND_APPLICATION_DIRECTORIES_1 = $(shell cd $(SOURCES_PATH) && find . -mindepth 1 -maxdepth 1 -type d)
# Remove the "./" preceding each name
FOUND_APPLICATION_DIRECTORIES_2 = $(subst ./,,$(FOUND_APPLICATION_DIRECTORIES_1))
# Sort the names to build in order
APPLICATIONS = $(sort $(FOUND_APPLICATION_DIRECTORIES_2))

# The following variables are shared by all sub-makefiles
export BINARIES_PATH
export LIBRARIES_PATH
export OBJECTS_PATH
export CCFLAGS
export LDFLAGS

#------------------------------------------------------------------------------------------------------------------------------
# Public functions
#------------------------------------------------------------------------------------------------------------------------------
# Compile the provided .c file into an object file.
# @param $(1) The source file to compile.
# @param $(2) The object file to generate.
# @param $(3) Additional gcc flags to append to the command line.
define ApplicationsCompileSourceFile
	@# Must use $$(x) around function parameters because this function is called from a sub-makefile, so the '$' character must be kept when exporting the function
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) $$(3) -c $$(1) -o $$(2)
endef
export ApplicationsCompileSourceFile

# Link the provided object files to a Lemon executable file.
# @param $(1) The list of the objects file to link, each file separated by a space.
# @param $(2) The name to give to the Lemon executable (file extension will be added automatically, do not provide it).
define ApplicationsLinkProgram
	@# Must use $$(x) around function parameters because this function is called from a sub-makefile, so the '$' character must be kept when exporting the function
	@# Generate the executable
	$(GLOBAL_TOOL_LINKER) $$(1) $(LDFLAGS) -o $(BINARIES_PATH)/$$(2).bin
	@# Append the Lemon executable header in front of the generated binary
	@printf "\022\064\126\170" > $(BINARIES_PATH)/$$(2)
	@cat $(BINARIES_PATH)/$$(2).bin >> $(BINARIES_PATH)/$$(2)
	@rm $(BINARIES_PATH)/$$(2).bin
endef
export ApplicationsLinkProgram

# Download a file on the Lemon system using the serial port server.
# @param $(1) The file to send.
# @param $(2) The name the file will have on the Lemon file system.
define ApplicationsDownloadFile
	$(TOOL_SERIAL_PORT_SERVER) /tmp/Lemon_Serial_Port_Pipe $$(1) $$(2)
endef
export ApplicationsDownloadFile

#------------------------------------------------------------------------------------------------------------------------------
# Rules
#------------------------------------------------------------------------------------------------------------------------------
# Append the /compile target to all discovered applications
all: $(patsubst %,%/compile,$(APPLICATIONS))

# Append the /clean target to all discovered applications
clean: $(patsubst %,%/clean,$(APPLICATIONS))
	# Delete also the tools
	rm -f $(TOOL_SERIAL_PORT_SERVER)

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
