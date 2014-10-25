OBJECTS = $(OBJECTS_PATH)/No_Match_Dealer/No_Match_Dealer.o
BINARY_NAME = No_Match_Dealer.32b

all: $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(BINARIES_PATH)/$(BINARY_NAME)

$(OBJECTS_PATH)/No_Match_Dealer/No_Match_Dealer.o: No_Match_Dealer.c No_Match_Dealer.h
	$(CC) $(CCFLAGS) -c No_Match_Dealer.c -o $(OBJECTS_PATH)/No_Match_Dealer/No_Match_Dealer.o

clean:
	rm -f $(OBJECTS_PATH)/No_Match_Dealer/*.o $(BINARIES_PATH)/$(BINARY_NAME)