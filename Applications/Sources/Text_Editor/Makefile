PROGRAM_NAME = Text_Editor
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Buffer.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Cursor.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Display.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

all: $(OBJECTS)
	$(call ApplicationsLinkProgram,$(OBJECTS),$(PROGRAM_NAME))

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(PROGRAM_NAME).*

download:
	@$(TOOL_SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/$(PROGRAM_NAME).lemon edit

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Buffer.o: Buffer.c Buffer.h
	$(call ApplicationsCompileSourceFile,Buffer.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Buffer.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Cursor.o: Cursor.c Cursor.h
	$(call ApplicationsCompileSourceFile,Cursor.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Cursor.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Display.o: Display.c Display.h
	$(call ApplicationsCompileSourceFile,Display.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Display.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Buffer.h Configuration.h Cursor.h Display.h Main.c
	$(call ApplicationsCompileSourceFile,Main.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o)
