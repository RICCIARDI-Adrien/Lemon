PROGRAM_NAME = No_Match_Dealer
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/No_Match_Dealer.o

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(PROGRAM_NAME).32b

$(OBJECTS_PATH)/$(PROGRAM_NAME)/No_Match_Dealer.o: No_Match_Dealer.c No_Match_Dealer.h
	$(CC) $(CCFLAGS) -c No_Match_Dealer.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/No_Match_Dealer.o

clean:
	rm -f $(OBJECTS_PATH)/$(PROGRAM_NAME)/*.o $(BINARIES_PATH)/$(PROGRAM_NAME).32b