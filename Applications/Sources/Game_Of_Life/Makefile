OBJECTS = $(OBJECTS_PATH)/Game_Of_Life/Game_Of_Life.o
BINARY_NAME = Game_Of_Life.32b

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(BINARY_NAME)

$(OBJECTS_PATH)/Game_Of_Life/Game_Of_Life.o: Game_Of_Life.c
	$(CC) $(CCFLAGS) -c Game_Of_Life.c -o $(OBJECTS_PATH)/Game_Of_Life/Game_Of_Life.o

clean:
	rm -f $(OBJECTS_PATH)/Game_Of_Life/*.o $(BINARIES_PATH)/$(BINARY_NAME)
