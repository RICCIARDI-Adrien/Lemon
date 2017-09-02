PROGRAM_NAME = Help
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Help.o

all: $(OBJECTS)
	$(call ApplicationsLinkProgram,$(OBJECTS),$(PROGRAM_NAME))

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(PROGRAM_NAME).*

download:
	$(call ApplicationsDownloadFile,$(BINARIES_PATH)/$(PROGRAM_NAME).lemon help)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Help.o: Help.c Strings.h
	$(call ApplicationsCompileSourceFile,Help.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Help.o)
