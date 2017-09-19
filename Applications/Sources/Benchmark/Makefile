BINARY_NAME = benchmark
PROGRAM_NAME = Benchmark
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Benchmark.o

all: $(OBJECTS)
	$(call ApplicationsLinkProgram,$(OBJECTS),$(BINARY_NAME))

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(BINARY_NAME)

download:
	$(call ApplicationsDownloadFile,$(BINARIES_PATH)/$(BINARY_NAME),$(BINARY_NAME))

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Benchmark.o: Benchmark.c
	$(call ApplicationsCompileSourceFile,Benchmark.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Benchmark.o,-O2)
