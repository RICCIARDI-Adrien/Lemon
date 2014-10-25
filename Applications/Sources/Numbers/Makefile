OBJECTS = $(OBJECTS_PATH)/Numbers/Numbers.o
BINARY_NAME = Numbers.32b

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(BINARY_NAME)

$(OBJECTS_PATH)/Numbers/Numbers.o: Numbers.c Numbers.h
	$(CC) $(CCFLAGS) -c Numbers.c -o $(OBJECTS_PATH)/Numbers/Numbers.o

clean:
	rm -f $(OBJECTS_PATH)/Numbers/*.o $(BINARIES_PATH)/$(BINARY_NAME)