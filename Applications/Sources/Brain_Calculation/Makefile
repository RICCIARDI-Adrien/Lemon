PROGRAM_NAME = Brain_Calculation
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(PROGRAM_NAME).32b

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o: Brain_Calculation.c Brain_Calculation.h
	$(CC) $(CCFLAGS) -c Brain_Calculation.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o

clean:
	rm -f $(OBJECTS_PATH)/$(PROGRAM_NAME)/*.o $(BINARIES_PATH)/$(PROGRAM_NAME).32b