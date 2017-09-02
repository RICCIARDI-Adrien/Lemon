PROGRAM_NAME = Games
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Game_Of_Life.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Hangman.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/No_Match_Dealer.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Numbers.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Rain.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Submarine.o

all: $(OBJECTS)
	$(call ApplicationsLinkProgram,$(OBJECTS),$(PROGRAM_NAME))

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/$(PROGRAM_NAME).*

download:
	$(call ApplicationsDownloadFile,$(BINARIES_PATH)/$(PROGRAM_NAME).lemon games)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o: Brain_Calculation.c Games.h Strings.h
	$(call ApplicationsCompileSourceFile,Brain_Calculation.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Brain_Calculation.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Game_Of_Life.o: Game_Of_Life.c Games.h Strings.h
	$(call ApplicationsCompileSourceFile,Game_Of_Life.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Game_Of_Life.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Hangman.o: Hangman.c Games.h Strings.h
	$(call ApplicationsCompileSourceFile,Hangman.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Hangman.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o: Main.c Games.h Strings.h
	$(call ApplicationsCompileSourceFile,Main.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Main.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/No_Match_Dealer.o: No_Match_Dealer.c Games.h Strings.h
	$(call ApplicationsCompileSourceFile,No_Match_Dealer.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/No_Match_Dealer.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Numbers.o: Numbers.c Games.h Strings.h
	$(call ApplicationsCompileSourceFile,Numbers.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Numbers.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Rain.o: Rain.c Games.h Strings.h
	$(call ApplicationsCompileSourceFile,Rain.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Rain.o)

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Submarine.o: Submarine.c Games.h Strings.h
	$(call ApplicationsCompileSourceFile,Submarine.c,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Submarine.o)
