PROGRAM_NAME = UNIX_Commands
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Df.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Ls.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_More.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_TFTP.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

all: $(OBJECTS)
	$(GLOBAL_TOOL_LINKER) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(PROGRAM_NAME).bin

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(PROGRAM_NAME).*

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Df.o: Commands.h Command_Df.c Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Command_Df.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Df.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Ls.o: Commands.h Command_Ls.c Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Command_Ls.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Ls.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_More.o: Commands.h Command_More.c Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Command_More.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_More.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_TFTP.o: Commands.h Command_TFTP.c Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Command_TFTP.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_TFTP.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Commands.h Main.c Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Main.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o