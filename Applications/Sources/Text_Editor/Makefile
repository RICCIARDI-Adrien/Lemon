PROGRAM_NAME = Text_Editor
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Buffer.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Cursor.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(PROGRAM_NAME).32b

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Buffer.o: Buffer.c Buffer.h
	$(CC) $(CCFLAGS) -c Buffer.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Buffer.o
	
$(OBJECTS_PATH)/$(PROGRAM_NAME)/Cursor.o: Cursor.c Cursor.h
	$(CC) $(CCFLAGS) -c Cursor.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Cursor.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Buffer.h Cursor.h Main.c
	$(CC) $(CCFLAGS) -c Main.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

clean:
	rm -f $(OBJECTS_PATH)/$(PROGRAM_NAME)/*.o $(BINARIES_PATH)/$(PROGRAM_NAME).32b