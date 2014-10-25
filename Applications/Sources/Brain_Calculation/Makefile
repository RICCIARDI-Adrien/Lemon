OBJECTS = $(OBJECTS_PATH)/Brain_Calculation/Brain_Calculation.o
BINARY_NAME = Brain_Calculation.32b

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(BINARY_NAME)

$(OBJECTS_PATH)/Brain_Calculation/Brain_Calculation.o: Brain_Calculation.c Brain_Calculation.h
	$(CC) $(CCFLAGS) -c Brain_Calculation.c -o $(OBJECTS_PATH)/Brain_Calculation/Brain_Calculation.o

clean:
	rm -f $(OBJECTS_PATH)/Brain_Calculation/*.o $(BINARIES_PATH)/$(BINARY_NAME)