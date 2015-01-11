CCFLAGS += -O2
OBJECTS = $(OBJECTS_PATH)/Benchmark/Benchmark.o
BINARY_NAME = Benchmark.32b

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(BINARY_NAME)

$(OBJECTS_PATH)/Benchmark/Benchmark.o: Benchmark.c
	$(CC) $(CCFLAGS) -c Benchmark.c -o $(OBJECTS_PATH)/Benchmark/Benchmark.o

clean:
	rm -f $(OBJECTS_PATH)/Benchmark/*.o $(BINARIES_PATH)/$(BINARY_NAME)