PROGRAM_NAME = Network_Chat
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Interface.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

all: $(OBJECTS)
	$(call ApplicationsLinkProgram,$(OBJECTS),$(PROGRAM_NAME))

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(PROGRAM_NAME).*

download:
	@$(TOOL_SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/$(PROGRAM_NAME).lemon chat

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Interface.o: Interface.c Interface.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Interface.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Interface.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Main.c Interface.h Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Main.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o