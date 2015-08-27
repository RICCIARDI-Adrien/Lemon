PROGRAM_NAME = Games
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(PROGRAM_NAME).32b

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o: Brain_Calculation.c Games.h Strings.h
	$(CC) $(CCFLAGS) -c Brain_Calculation.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Main.c Games.h Strings.h
	$(CC) $(CCFLAGS) -c Main.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

clean:
	rm -f $(OBJECTS_PATH)/$(PROGRAM_NAME)/*.o $(BINARIES_PATH)/$(PROGRAM_NAME).32b