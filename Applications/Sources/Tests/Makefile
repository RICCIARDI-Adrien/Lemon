PROGRAM_NAME = Tests
OBJECTS_TESTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/crc32.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Display_Message.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_File.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_Memory.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_String.o
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Display_ASCII_Characters.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Protection_Check.o $(OBJECTS_TESTS)

all: $(OBJECTS)
	$(LD) $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Display_ASCII_Characters.o $(LDFLAGS) -o $(BINARIES_PATH)/Test_Display_ASCII_Characters.bin
	$(LD) $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Protection_Check.o $(LDFLAGS) -o $(BINARIES_PATH)/Test_Protection_Check.bin
	$(LD) $(OBJECTS_TESTS) $(LDFLAGS) -o $(BINARIES_PATH)/Tests.bin

clean:
	rm -f $(OBJECTS_PATH)/$(PROGRAM_NAME)/*.o $(BINARIES_PATH)/Test_Display_ASCII_Characters.* $(BINARIES_PATH)/Test_Protection_Check.* $(BINARIES_PATH)/Tests.*

$(OBJECTS_PATH)/$(PROGRAM_NAME)/crc32.o: crc32.c crc32.h
	$(CC) $(CCFLAGS) -c crc32.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/crc32.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Display_Message.o: Display_Message.c Display_Message.h
	$(CC) $(CCFLAGS) -c Display_Message.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Display_Message.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests.o: Tests.c Display_Message.h Tests.h
	$(CC) $(CCFLAGS) -c Tests.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_File.o: Tests_File.c crc32.h Display_Message.h Tests.h
	$(CC) $(CCFLAGS) -c Tests_File.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_File.o
	
$(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_Memory.o: Tests_Memory.c Display_Message.h Tests.h
	$(CC) $(CCFLAGS) -c Tests_Memory.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_Memory.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_String.o: Tests_String.c Display_Message.h Tests.h
	$(CC) $(CCFLAGS) -c Tests_String.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_String.o

# The following objects will be linked as separate executables, but this can't be done in one time as a custom linker script must be used
$(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Display_ASCII_Characters.o: Test_Display_ASCII_Characters.c
	$(CC) $(CCFLAGS) -c Test_Display_ASCII_Characters.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Display_ASCII_Characters.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Protection_Check.o: Test_Protection_Check.c
	$(CC) $(CCFLAGS) -c Test_Protection_Check.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Protection_Check.o
