PROGRAM_NAME = Benchmark
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Benchmark.o
CCFLAGS += -O2

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(PROGRAM_NAME).bin

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(PROGRAM_NAME).*

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Benchmark.o: Benchmark.c
	$(CC) $(CCFLAGS) -c Benchmark.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Benchmark.o