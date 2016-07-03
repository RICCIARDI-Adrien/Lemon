PROGRAM_NAME = Text_Editor
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Buffer.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Cursor.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Display.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(PROGRAM_NAME).bin

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(PROGRAM_NAME).*

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Buffer.o: Buffer.c Buffer.h
	$(CC) $(CCFLAGS) -c Buffer.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Buffer.o
	
$(OBJECTS_PATH)/$(PROGRAM_NAME)/Cursor.o: Cursor.c Cursor.h
	$(CC) $(CCFLAGS) -c Cursor.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Cursor.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Display.o: Display.c Display.h
	$(CC) $(CCFLAGS) -c Display.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Display.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Buffer.h Configuration.h Cursor.h Display.h Main.c
	$(CC) $(CCFLAGS) -c Main.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o