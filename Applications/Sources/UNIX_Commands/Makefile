PROGRAM_NAME = UNIX_Commands
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Df.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Ls.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_More.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(PROGRAM_NAME).32b

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Df.o: Commands.h Command_Df.c Strings.h
	$(CC) $(CCFLAGS) -c Command_Df.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Df.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Ls.o: Commands.h Command_Ls.c Strings.h
	$(CC) $(CCFLAGS) -c Command_Ls.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Ls.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_More.o: Commands.h Command_More.c Strings.h
	$(CC) $(CCFLAGS) -c Command_More.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_More.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Commands.h Main.c Strings.h
	$(CC) $(CCFLAGS) -c Main.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

clean:
	rm -f $(OBJECTS_PATH)/$(PROGRAM_NAME)/*.o $(BINARIES_PATH)/$(PROGRAM_NAME).32b