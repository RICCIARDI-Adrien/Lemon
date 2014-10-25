OBJECTS_TESTS = $(OBJECTS_PATH)/Tests/crc32.o $(OBJECTS_PATH)/Tests/Tests.o $(OBJECTS_PATH)/Tests/Tests_Assert.o $(OBJECTS_PATH)/Tests/Tests_Ctype.o
OBJECTS = $(OBJECTS_PATH)/Tests/Test_Display_ASCII_Characters.o $(OBJECTS_PATH)/Tests/Test_Protection_Check.o $(OBJECTS_TESTS)

all: $(OBJECTS)
	$(LD) $(OBJECTS_PATH)/Tests/Test_Display_ASCII_Characters.o $(LDFLAGS) -o $(BINARIES_PATH)/Test_Display_ASCII_Characters.32b
	$(LD) $(OBJECTS_PATH)/Tests/Test_Protection_Check.o $(LDFLAGS) -o $(BINARIES_PATH)/Test_Protection_Check.32b
	$(LD) $(OBJECTS_TESTS) $(LDFLAGS) -o $(BINARIES_PATH)/Tests.32b

$(OBJECTS_PATH)/Tests/crc32.o: crc32.c crc32.h
	$(CC) $(CCFLAGS) -c crc32.c -o $(OBJECTS_PATH)/Tests/crc32.o

$(OBJECTS_PATH)/Tests/Tests.o: Tests.c crc32.h Tests.h
	$(CC) $(CCFLAGS) -c Tests.c -o $(OBJECTS_PATH)/Tests/Tests.o

$(OBJECTS_PATH)/Tests/Tests_Assert.o: Tests_Assert.c Tests.h
	$(CC) $(CCFLAGS) -c Tests_Assert.c -o $(OBJECTS_PATH)/Tests/Tests_Assert.o

$(OBJECTS_PATH)/Tests/Tests_Ctype.o: Tests_Ctype.c Tests.h
	$(CC) $(CCFLAGS) -c Tests_Ctype.c -o $(OBJECTS_PATH)/Tests/Tests_Ctype.o

$(OBJECTS_PATH)/Tests/Test_Display_ASCII_Characters.o: Test_Display_ASCII_Characters.c
	$(CC) $(CCFLAGS) -c Test_Display_ASCII_Characters.c -o $(OBJECTS_PATH)/Tests/Test_Display_ASCII_Characters.o

$(OBJECTS_PATH)/Tests/Test_Protection_Check.o: Test_Protection_Check.c
	$(CC) $(CCFLAGS) -c Test_Protection_Check.c -o $(OBJECTS_PATH)/Tests/Test_Protection_Check.o

clean:
	rm -f $(OBJECTS_PATH)/Tests/*.o $(BINARIES_PATH)/Test_Display_ASCII_Characters.32b $(BINARIES_PATH)/Test_Protection_Check.32b $(BINARIES_PATH)/Tests.32b
