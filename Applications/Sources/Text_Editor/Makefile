BINARY_NAME = edit
PROGRAM_NAME = Text_Editor
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Buffer.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Cursor.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Display.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

all: $(OBJECTS)
	$(call ApplicationsLinkProgram,$(OBJECTS),$(BINARY_NAME))

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(BINARY_NAME)

download:
	$(call ApplicationsDownloadFile,$(BINARIES_PATH)/$(BINARY_NAME),$(BINARY_NAME))

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Buffer.o: Buffer.c Buffer.h
	$(call ApplicationsCompileSourceFile,Buffer.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Buffer.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Cursor.o: Cursor.c Cursor.h
	$(call ApplicationsCompileSourceFile,Cursor.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Cursor.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Display.o: Display.c Display.h
	$(call ApplicationsCompileSourceFile,Display.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Display.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Buffer.h Configuration.h Cursor.h Display.h Main.c
	$(call ApplicationsCompileSourceFile,Main.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o)
