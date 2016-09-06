PROGRAM_NAME = Games
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Game_Of_Life.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Hangman.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/No_Match_Dealer.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Numbers.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Rain.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Submarine.o

all: $(OBJECTS)
	$(GLOBAL_TOOL_LINKER) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(PROGRAM_NAME).bin

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(PROGRAM_NAME).*

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o: Brain_Calculation.c Games.h Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Brain_Calculation.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Game_Of_Life.o: Game_Of_Life.c Games.h Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Game_Of_Life.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Game_Of_Life.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Hangman.o: Hangman.c Games.h Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Hangman.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Hangman.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Main.c Games.h Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Main.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/No_Match_Dealer.o: No_Match_Dealer.c Games.h Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c No_Match_Dealer.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/No_Match_Dealer.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Numbers.o: Numbers.c Games.h Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Numbers.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Numbers.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Rain.o: Rain.c Games.h Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Rain.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Rain.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Submarine.o: Submarine.c Games.h Strings.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Submarine.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Submarine.o