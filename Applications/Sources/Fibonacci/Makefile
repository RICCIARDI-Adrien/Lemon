OBJECTS = $(OBJECTS_PATH)/Fibonacci/Fibonacci.o
BINARY_NAME = Fibonacci.32b

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(BINARY_NAME)

$(OBJECTS_PATH)/Fibonacci/Fibonacci.o: Fibonacci.c
	$(CC) $(CCFLAGS) -c Fibonacci.c -o $(OBJECTS_PATH)/Fibonacci/Fibonacci.o

clean:
	rm -f $(OBJECTS_PATH)/Fibonacci/*.o $(BINARIES_PATH)/$(BINARY_NAME)