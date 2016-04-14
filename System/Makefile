# System makefile.
# There is no need to take dependencies into account as the whole system must be entirely rebuilded each time.
# Author : Adrien RICCIARDI
include ../Rules.mk

PATH_SOURCES = Sources
PATH_OBJECTS = Objects
PATH_INCLUDES = Includes

LDFLAGS = -T Linker_Script.ld
MBR_FLAGS = -DSECTORS_TO_LOAD_COUNT=64

# The common makefile must be included after the paths and flags variables have been defined (because it uses them), and before the object variables are set (because it contains some of them)
include Makefile_Core.mk

OBJECTS_SHELL = $(PATH_OBJECTS)/Shell.o $(PATH_OBJECTS)/Shell_Command_Copy_File.o $(PATH_OBJECTS)/Shell_Command_Delete_File.o $(PATH_OBJECTS)/Shell_Command_Download.o $(PATH_OBJECTS)/Shell_Command_File_Size.o $(PATH_OBJECTS)/Shell_Command_List.o $(PATH_OBJECTS)/Shell_Command_Rename_File.o
OBJECTS = $(OBJECTS_CORE) $(OBJECTS_DRIVERS) $(OBJECTS_FILE_SYSTEM) $(OBJECTS_SHELL)

all: clean $(PATH_OBJECTS)/MBR.32b $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(PATH_OBJECTS)/Kernel.32b
	######## Build successful ########
	@ls -l $(PATH_OBJECTS)/Kernel.32b | awk '{print "Kernel size : " $$5 + 512 " bytes"}'

#------------------------------------------------------------------------------------------------------------------------------
# Shell
#------------------------------------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Shell.o: $(PATH_SOURCES)/Shell/Shell.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell.c -o $(PATH_OBJECTS)/Shell.o

$(PATH_OBJECTS)/Shell_Command_Copy_File.o: $(PATH_SOURCES)/Shell/Shell_Command_Copy_File.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Copy_File.c -o $(PATH_OBJECTS)/Shell_Command_Copy_File.o

$(PATH_OBJECTS)/Shell_Command_Delete_File.o: $(PATH_SOURCES)/Shell/Shell_Command_Delete_File.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Delete_File.c -o $(PATH_OBJECTS)/Shell_Command_Delete_File.o

$(PATH_OBJECTS)/Shell_Command_Download.o: $(PATH_SOURCES)/Shell/Shell_Command_Download.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Download.c -o $(PATH_OBJECTS)/Shell_Command_Download.o

$(PATH_OBJECTS)/Shell_Command_File_Size.o: $(PATH_SOURCES)/Shell/Shell_Command_File_Size.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_File_Size.c -o $(PATH_OBJECTS)/Shell_Command_File_Size.o

$(PATH_OBJECTS)/Shell_Command_List.o: $(PATH_SOURCES)/Shell/Shell_Command_List.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_List.c -o $(PATH_OBJECTS)/Shell_Command_List.o

$(PATH_OBJECTS)/Shell_Command_Rename_File.o: $(PATH_SOURCES)/Shell/Shell_Command_Rename_File.c
	$(CC) $(CCFLAGS) -c $(PATH_SOURCES)/Shell/Shell_Command_Rename_File.c -o $(PATH_OBJECTS)/Shell_Command_Rename_File.o