PROGRAM_NAME = Network_Chat
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Interface.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

all: $(OBJECTS)
	$(call ApplicationsLinkProgram,$(OBJECTS),$(PROGRAM_NAME))

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(PROGRAM_NAME).*

download:
	$(call ApplicationsDownloadFile,$(BINARIES_PATH)/$(PROGRAM_NAME).lemon chat)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Interface.o: Interface.c Interface.h
	$(call ApplicationsCompileSourceFile,Interface.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Interface.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Main.c Interface.h Strings.h
	$(call ApplicationsCompileSourceFile,Main.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o)
