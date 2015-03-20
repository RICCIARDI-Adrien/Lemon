PROGRAM_NAME = Text_Viewer
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Text_Viewer.o

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(PROGRAM_NAME).32b

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Text_Viewer.o: Text_Viewer.c Text_Viewer.h
	$(CC) $(CCFLAGS) -c Text_Viewer.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Text_Viewer.o

clean:
	rm -f $(OBJECTS_PATH)/$(PROGRAM_NAME)/*.o $(BINARIES_PATH)/$(PROGRAM_NAME).32b