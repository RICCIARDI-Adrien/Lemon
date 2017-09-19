BINARY_NAME = u
PROGRAM_NAME = UNIX_Commands
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Df.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Diff.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Ls.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_More.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_TFTP.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Uptime.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

all: $(OBJECTS)
	$(call ApplicationsLinkProgram,$(OBJECTS),$(BINARY_NAME))

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(BINARY_NAME)

download:
	$(call ApplicationsDownloadFile,$(BINARIES_PATH)/$(BINARY_NAME),$(BINARY_NAME))

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Df.o: Commands.h Command_Df.c Strings.h
	$(call ApplicationsCompileSourceFile,Command_Df.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Df.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Diff.o: Commands.h Command_Diff.c Strings.h
	$(call ApplicationsCompileSourceFile,Command_Diff.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Diff.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Ls.o: Commands.h Command_Ls.c Strings.h
	$(call ApplicationsCompileSourceFile,Command_Ls.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Ls.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_More.o: Commands.h Command_More.c Strings.h
	$(call ApplicationsCompileSourceFile,Command_More.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_More.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_TFTP.o: Commands.h Command_TFTP.c Strings.h
	$(call ApplicationsCompileSourceFile,Command_TFTP.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_TFTP.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Uptime.o: Commands.h Command_Uptime.c Strings.h
	$(call ApplicationsCompileSourceFile,Command_Uptime.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Command_Uptime.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Commands.h Main.c Strings.h
	$(call ApplicationsCompileSourceFile,Main.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o)
