PROGRAM_NAME = Tests
OBJECTS_TESTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/crc32.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Display_Message.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_File.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_Memory.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_String.o
OBJECTS = $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Display_ASCII_Characters.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Protection_Check.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Network_UDP_Transmission.o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Network_Ethernet_Reception.o $(OBJECTS_TESTS)

all: $(OBJECTS)
	$(call ApplicationsLinkProgram,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Display_ASCII_Characters.o,Test_Display_ASCII_Characters)
	$(call ApplicationsLinkProgram,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Protection_Check.o,Test_Protection_Check)
	$(call ApplicationsLinkProgram,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Network_UDP_Transmission.o,Test_Network_UDP_Transmission)
	$(call ApplicationsLinkProgram,$(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Network_Ethernet_Reception.o,Test_Network_Ethernet_Reception)
	$(call ApplicationsLinkProgram,$(OBJECTS_TESTS),$(PROGRAM_NAME))

clean:
	rm -rf $(OBJECTS_PATH)/$(PROGRAM_NAME) $(BINARIES_PATH)/Test_Display_ASCII_Characters.* $(BINARIES_PATH)/Test_Protection_Check.* $(BINARIES_PATH)/Test_Network_UDP_Transmission.* $(BINARIES_PATH)/Test_Network_Ethernet_Reception.* $(BINARIES_PATH)/Tests.*

download:
	@$(TOOL_SERIAL_PORT_SERVER) $(SERIAL_PORT_SERVER_FLAGS) $(BINARIES_PATH)/$(PROGRAM_NAME).lemon tests

$(OBJECTS_PATH)/$(PROGRAM_NAME)/crc32.o: crc32.c crc32.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c crc32.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/crc32.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Display_Message.o: Display_Message.c Display_Message.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Display_Message.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Display_Message.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests.o: Tests.c Display_Message.h Tests.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Tests.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_File.o: Tests_File.c crc32.h Display_Message.h Tests.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Tests_File.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_File.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_Memory.o: Tests_Memory.c Display_Message.h Tests.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Tests_Memory.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_Memory.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_String.o: Tests_String.c Display_Message.h Tests.h
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Tests_String.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Tests_String.o

# The following objects will be linked as separate executables, but this can't be done in one time as a custom linker script must be used
$(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Display_ASCII_Characters.o: Test_Display_ASCII_Characters.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Test_Display_ASCII_Characters.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Display_ASCII_Characters.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Protection_Check.o: Test_Protection_Check.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Test_Protection_Check.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Protection_Check.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Network_UDP_Transmission.o: Test_Network_UDP_Transmission.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Test_Network_UDP_Transmission.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Network_UDP_Transmission.o

$(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Network_Ethernet_Reception.o: Test_Network_Ethernet_Reception.c
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c Test_Network_Ethernet_Reception.c -o $(OBJECTS_PATH)/$(PROGRAM_NAME)/Test_Network_Ethernet_Reception.o
