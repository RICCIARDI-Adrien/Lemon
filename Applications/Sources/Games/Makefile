PROGRAM_NAME = Games
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Game_Of_Life.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Hangman.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/No_Match_Dealer.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Numbers.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Rain.o

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(PROGRAM_NAME).32b

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o: Brain_Calculation.c Games.h Strings.h
	$(CC) $(CCFLAGS) -c Brain_Calculation.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Game_Of_Life.o: Game_Of_Life.c Games.h Strings.h
	$(CC) $(CCFLAGS) -c Game_Of_Life.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Game_Of_Life.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Hangman.o: Hangman.c Games.h Strings.h
	$(CC) $(CCFLAGS) -c Hangman.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Hangman.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Main.c Games.h Strings.h
	$(CC) $(CCFLAGS) -c Main.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/No_Match_Dealer.o: No_Match_Dealer.c Games.h Strings.h
	$(CC) $(CCFLAGS) -c No_Match_Dealer.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/No_Match_Dealer.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Numbers.o: Numbers.c Games.h Strings.h
	$(CC) $(CCFLAGS) -c Numbers.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Numbers.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Rain.o: Rain.c Games.h Strings.h
	$(CC) $(CCFLAGS) -c Rain.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Rain.o

clean:
	rm -f $(OBJECTS_PATH)/$(PROGRAM_NAME)/*.o $(BINARIES_PATH)/$(PROGRAM_NAME).32b