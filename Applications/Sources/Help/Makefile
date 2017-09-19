BINARY_NAME = help
PROGRAM_NAME = Help
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Help.o

all: $(OBJECTS)
	$(call ApplicationsLinkProgram,$(OBJECTS),$(BINARY_NAME))

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(BINARY_NAME)

download:
	$(call ApplicationsDownloadFile,$(BINARIES_PATH)/$(BINARY_NAME),$(BINARY_NAME))

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Help.o: Help.c Strings.h
	$(call ApplicationsCompileSourceFile,Help.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Help.o)
