# System makefile.
# There is no need to take dependencies into account as the whole system must be entirely rebuilded each time.
# Author : Adrien RICCIARDI
PATH_SOURCES = Sources
PATH_OBJECTS = Objects
PATH_INCLUDES = Includes

CCFLAGS += -DCONFIGURATION_BUILD_INSTALLER=0 -DSTRING_BUILD_CONFIGURATION_VARIABLES=$(STRING_BUILD_CONFIGURATION_VARIABLES)
LDFLAGS = -T Linker_Script.ld
MBR_FLAGS = -DSECTORS_TO_LOAD_COUNT=64

# The common makefile must be included after the paths and flags variables have been defined (because it uses them), and before the object variables are set (because it contains some of them)
include Makefile_Core.mk

OBJECTS_SHELL = $(PATH_OBJECTS)/Shell.o $(PATH_OBJECTS)/Shell_Command_Copy_File.o $(PATH_OBJECTS)/Shell_Command_Delete_File.o $(PATH_OBJECTS)/Shell_Command_Download.o $(PATH_OBJECTS)/Shell_Command_File_Size.o $(PATH_OBJECTS)/Shell_Command_List.o $(PATH_OBJECTS)/Shell_Command_Rename_File.o
OBJECTS = $(OBJECTS_CORE) $(OBJECTS_DRIVERS) $(OBJECTS_FILE_SYSTEM) $(OBJECTS_SHELL)

all: clean $(PATH_OBJECTS)/MBR.bin $(OBJECTS)
	$(GLOBAL_TOOL_LINKER) $(LDFLAGS) $(OBJECTS) -o $(PATH_OBJECTS)/Kernel.bin
	######## Build successful ########
	@ls -l $(PATH_OBJECTS)/Kernel.bin | awk '{print "Kernel size : " $$5 + 512 " bytes"}'

#------------------------------------------------------------------------------------------------------------------------------
# Shell
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Shell.o: $(PATH_SOURCES)/Shell/Shell.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell.c -o $(PATH_OBJECTS)/Shell.o

$(PATH_OBJECTS)/Shell_Command_Copy_File.o: $(PATH_SOURCES)/Shell/Shell_Command_Copy_File.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Copy_File.c -o $(PATH_OBJECTS)/Shell_Command_Copy_File.o

$(PATH_OBJECTS)/Shell_Command_Delete_File.o: $(PATH_SOURCES)/Shell/Shell_Command_Delete_File.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Delete_File.c -o $(PATH_OBJECTS)/Shell_Command_Delete_File.o

$(PATH_OBJECTS)/Shell_Command_Download.o: $(PATH_SOURCES)/Shell/Shell_Command_Download.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Download.c -o $(PATH_OBJECTS)/Shell_Command_Download.o

$(PATH_OBJECTS)/Shell_Command_File_Size.o: $(PATH_SOURCES)/Shell/Shell_Command_File_Size.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_File_Size.c -o $(PATH_OBJECTS)/Shell_Command_File_Size.o

$(PATH_OBJECTS)/Shell_Command_List.o: $(PATH_SOURCES)/Shell/Shell_Command_List.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_List.c -o $(PATH_OBJECTS)/Shell_Command_List.o

$(PATH_OBJECTS)/Shell_Command_Rename_File.o: $(PATH_SOURCES)/Shell/Shell_Command_Rename_File.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Rename_File.c -o $(PATH_OBJECTS)/Shell_Command_Rename_File.o