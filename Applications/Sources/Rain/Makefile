OBJECTS = $(OBJECTS_PATH)/Rain/Rain.o
BINARY_NAME = Rain.32b

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(BINARY_NAME)

$(OBJECTS_PATH)/Rain/Rain.o: Rain.c Rain.h
	$(CC) $(CCFLAGS) -c Rain.c -o $(OBJECTS_PATH)/Rain/Rain.o

clean:
	rm -f $(OBJECTS_PATH)/Rain/*.o $(BINARIES_PATH)/$(BINARY_NAME)