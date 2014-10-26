OBJECTS = $(OBJECTS_PATH)/Help/Help.o
BINARY_NAME = Help.32b

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(BINARY_NAME)

$(OBJECTS_PATH)/Help/Help.o: Help.c Help.h
	$(CC) $(CCFLAGS) -c Help.c -o $(OBJECTS_PATH)/Help/Help.o

clean:
	rm -f $(OBJECTS_PATH)/Help/*.o $(BINARIES_PATH)/$(BINARY_NAME)