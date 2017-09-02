PROGRAM_NAME = Benchmark
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Benchmark.o

all: $(OBJECTS)
	$(call ApplicationsLinkProgram,$(OBJECTS),$(PROGRAM_NAME))

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(PROGRAM_NAME).*

download:
	$(call ApplicationsDownloadFile,$(BINARIES_PATH)/$(PROGRAM_NAME).lemon benchmark)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Benchmark.o: Benchmark.c
	$(call ApplicationsCompileSourceFile,Benchmark.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Benchmark.o,-O2)
