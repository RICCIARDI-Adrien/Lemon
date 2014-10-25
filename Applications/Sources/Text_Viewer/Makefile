OBJECTS = $(OBJECTS_PATH)/Text_Viewer/Text_Viewer.o
BINARY_NAME = Text_Viewer.32b

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(BINARY_NAME)

$(OBJECTS_PATH)/Text_Viewer/Text_Viewer.o: Text_Viewer.c Text_Viewer.h
	$(CC) $(CCFLAGS) -c Text_Viewer.c -o $(OBJECTS_PATH)/Text_Viewer/Text_Viewer.o

clean:
	rm -f $(OBJECTS_PATH)/Text_Viewer/*.o $(BINARIES_PATH)/$(BINARY_NAME)