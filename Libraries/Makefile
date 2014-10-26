# Makefile for program libraries generation
# Author : Adrien RICCIARDI
# V 1.0 : 27/04/2012
# V 1.1 : 28/07/2012
# V 1.2 : 28/04/2014, made a better organization.
# V 1.3 : 03/05/2014, System_Program_Entry_Point is now added to the archive and linked into a different section.
# V 1.4 : 06/07/2014, added Newlib 2.1.0 support and removed custom libc.
# V 1.5 : 25/10/2014, completely removed UNIX libc, using only Lemon libraries.
BINARIES_PATH = Binaries
INCLUDES_PATH = Includes
OBJECTS_PATH = Objects
SOURCES_PATH = Sources

OBJECTS_FILE = $(OBJECTS_PATH)/File_Close.o $(OBJECTS_PATH)/File_Exists.o $(OBJECTS_PATH)/File_List_Initialize.o $(OBJECTS_PATH)/File_List_Next.o $(OBJECTS_PATH)/File_Open.o $(OBJECTS_PATH)/File_Read.o $(OBJECTS_PATH)/File_Size.o $(OBJECTS_PATH)/File_Write.o
OBJECTS_KEYBOARD = $(OBJECTS_PATH)/Keyboard_Is_Key_Available.o $(OBJECTS_PATH)/Keyboard_Read_Character.o
OBJECTS_MEMORY = $(OBJECTS_PATH)/Memory_Copy_Area.o $(OBJECTS_PATH)/Memory_Set_Area_Value.o
OBJECTS_RANDOM = $(OBJECTS_PATH)/Random_Generate_Number.o $(OBJECTS_PATH)/Random_Initialize.o
OBJECTS_SCREEN = $(OBJECTS_PATH)/Screen_Clear.o $(OBJECTS_PATH)/Screen_Display_Buffer.o $(OBJECTS_PATH)/Screen_Get_Cursor_Position.o $(OBJECTS_PATH)/Screen_Set_Background_Color.o $(OBJECTS_PATH)/Screen_Set_Cursor_Position.o $(OBJECTS_PATH)/Screen_Set_Font_Color.o $(OBJECTS_PATH)/Screen_Write_Character.o $(OBJECTS_PATH)/Screen_Write_Integer.o $(OBJECTS_PATH)/Screen_Write_String.o $(OBJECTS_PATH)/Screen_Write_Unsigned_Integer.o
OBJECTS_STRING = $(OBJECTS_PATH)/String_Compare.o $(OBJECTS_PATH)/String_Convert_String_To_Unsigned_Integer.o $(OBJECTS_PATH)/String_Copy.o $(OBJECTS_PATH)/String_Size.o
OBJECTS_SYSTEM = $(OBJECTS_PATH)/System_Program_Entry_Point.o $(OBJECTS_PATH)/System_Wait.o
OBJECTS_UART = $(OBJECTS_PATH)/UART_Initialize.o $(OBJECTS_PATH)/UART_Is_Data_Received.o $(OBJECTS_PATH)/UART_Read_Byte.o $(OBJECTS_PATH)/UART_Write_Byte.o
OBJECTS = $(OBJECTS_FILE) $(OBJECTS_KEYBOARD) $(OBJECTS_MEMORY) $(OBJECTS_RANDOM) $(OBJECTS_SCREEN) $(OBJECTS_STRING) $(OBJECTS_SYSTEM) $(OBJECTS_UART)

CC = gcc

ARCHIVE_NAME = Libraries.a
INCLUDES = -IIncludes -I../System/Includes
CCFLAGS += -c -nostdlib -fno-builtin -masm=intel -m32 -W -Wall -O2 $(INCLUDES)

all: $(OBJECTS)
	ar -rvsc $(BINARIES_PATH)/$(ARCHIVE_NAME) $(OBJECTS)
	ls -lh $(BINARIES_PATH)/$(ARCHIVE_NAME)

#--------------------------------------------------------------------------------------------------
# File
#--------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/File_Close.o: $(SOURCES_PATH)/File/File_Close.c $(INCLUDES_PATH)/File.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/File/File_Close.c -o $(OBJECTS_PATH)/File_Close.o

$(OBJECTS_PATH)/File_Exists.o: $(SOURCES_PATH)/File/File_Exists.c $(INCLUDES_PATH)/File.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/File/File_Exists.c -o $(OBJECTS_PATH)/File_Exists.o

$(OBJECTS_PATH)/File_List_Initialize.o: $(SOURCES_PATH)/File/File_List_Initialize.c $(INCLUDES_PATH)/File.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/File/File_List_Initialize.c -o $(OBJECTS_PATH)/File_List_Initialize.o

$(OBJECTS_PATH)/File_List_Next.o: $(SOURCES_PATH)/File/File_List_Next.c $(INCLUDES_PATH)/File.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/File/File_List_Next.c -o $(OBJECTS_PATH)/File_List_Next.o

$(OBJECTS_PATH)/File_Open.o: $(SOURCES_PATH)/File/File_Open.c $(INCLUDES_PATH)/File.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/File/File_Open.c -o $(OBJECTS_PATH)/File_Open.o

$(OBJECTS_PATH)/File_Read.o: $(SOURCES_PATH)/File/File_Read.c $(INCLUDES_PATH)/File.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/File/File_Read.c -o $(OBJECTS_PATH)/File_Read.o

$(OBJECTS_PATH)/File_Size.o: $(SOURCES_PATH)/File/File_Size.c $(INCLUDES_PATH)/File.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/File/File_Size.c -o $(OBJECTS_PATH)/File_Size.o

$(OBJECTS_PATH)/File_Write.o: $(SOURCES_PATH)/File/File_Write.c $(INCLUDES_PATH)/File.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/File/File_Write.c -o $(OBJECTS_PATH)/File_Write.o
	
#--------------------------------------------------------------------------------------------------
# Keyboard
#--------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/Keyboard_Is_Key_Available.o: $(SOURCES_PATH)/Keyboard/Keyboard_Is_Key_Available.c $(INCLUDES_PATH)/Keyboard.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Keyboard/Keyboard_Is_Key_Available.c -o $(OBJECTS_PATH)/Keyboard_Is_Key_Available.o

$(OBJECTS_PATH)/Keyboard_Read_Character.o: $(SOURCES_PATH)/Keyboard/Keyboard_Read_Character.c $(INCLUDES_PATH)/Keyboard.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Keyboard/Keyboard_Read_Character.c -o $(OBJECTS_PATH)/Keyboard_Read_Character.o
	
#--------------------------------------------------------------------------------------------------
# Memory
#--------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/Memory_Copy_Area.o: $(SOURCES_PATH)/Memory/Memory_Copy_Area.c $(INCLUDES_PATH)/Memory.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Memory/Memory_Copy_Area.c -o $(OBJECTS_PATH)/Memory_Copy_Area.o
	
$(OBJECTS_PATH)/Memory_Set_Area_Value.o: $(SOURCES_PATH)/Memory/Memory_Set_Area_Value.c $(INCLUDES_PATH)/Memory.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Memory/Memory_Set_Area_Value.c -o $(OBJECTS_PATH)/Memory_Set_Area_Value.o

#--------------------------------------------------------------------------------------------------
# Random
#--------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/Random_Generate_Number.o: $(SOURCES_PATH)/Random/Random_Generate_Number.c $(INCLUDES_PATH)/Random.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Random/Random_Generate_Number.c -o $(OBJECTS_PATH)/Random_Generate_Number.o
	
$(OBJECTS_PATH)/Random_Initialize.o: $(SOURCES_PATH)/Random/Random_Initialize.c $(INCLUDES_PATH)/Random.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Random/Random_Initialize.c -o $(OBJECTS_PATH)/Random_Initialize.o
	
#--------------------------------------------------------------------------------------------------
# String
#--------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/String_Compare.o: $(SOURCES_PATH)/String/String_Compare.c $(INCLUDES_PATH)/String.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/String/String_Compare.c -o $(OBJECTS_PATH)/String_Compare.o
	
$(OBJECTS_PATH)/String_Convert_String_To_Unsigned_Integer.o: $(SOURCES_PATH)/String/String_Convert_String_To_Unsigned_Integer.c $(INCLUDES_PATH)/String.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/String/String_Convert_String_To_Unsigned_Integer.c -o $(OBJECTS_PATH)/String_Convert_String_To_Unsigned_Integer.o

$(OBJECTS_PATH)/String_Copy.o: $(SOURCES_PATH)/String/String_Copy.c $(INCLUDES_PATH)/String.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/String/String_Copy.c -o $(OBJECTS_PATH)/String_Copy.o

$(OBJECTS_PATH)/String_Size.o: $(SOURCES_PATH)/String/String_Size.c $(INCLUDES_PATH)/String.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/String/String_Size.c -o $(OBJECTS_PATH)/String_Size.o

#--------------------------------------------------------------------------------------------------
# Screen
#--------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/Screen_Clear.o: $(SOURCES_PATH)/Screen/Screen_Clear.c $(INCLUDES_PATH)/Screen.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Screen/Screen_Clear.c -o $(OBJECTS_PATH)/Screen_Clear.o

$(OBJECTS_PATH)/Screen_Display_Buffer.o: $(SOURCES_PATH)/Screen/Screen_Display_Buffer.c $(INCLUDES_PATH)/Screen.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Screen/Screen_Display_Buffer.c -o $(OBJECTS_PATH)/Screen_Display_Buffer.o

$(OBJECTS_PATH)/Screen_Get_Cursor_Position.o: $(SOURCES_PATH)/Screen/Screen_Get_Cursor_Position.c $(INCLUDES_PATH)/Screen.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Screen/Screen_Get_Cursor_Position.c -o $(OBJECTS_PATH)/Screen_Get_Cursor_Position.o

$(OBJECTS_PATH)/Screen_Set_Background_Color.o: $(SOURCES_PATH)/Screen/Screen_Set_Background_Color.c $(INCLUDES_PATH)/Screen.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Screen/Screen_Set_Background_Color.c -o $(OBJECTS_PATH)/Screen_Set_Background_Color.o

$(OBJECTS_PATH)/Screen_Set_Cursor_Position.o: $(SOURCES_PATH)/Screen/Screen_Set_Cursor_Position.c $(INCLUDES_PATH)/Screen.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Screen/Screen_Set_Cursor_Position.c -o $(OBJECTS_PATH)/Screen_Set_Cursor_Position.o

$(OBJECTS_PATH)/Screen_Set_Font_Color.o: $(SOURCES_PATH)/Screen/Screen_Set_Font_Color.c $(INCLUDES_PATH)/Screen.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Screen/Screen_Set_Font_Color.c -o $(OBJECTS_PATH)/Screen_Set_Font_Color.o

$(OBJECTS_PATH)/Screen_Write_Character.o: $(SOURCES_PATH)/Screen/Screen_Write_Character.c $(INCLUDES_PATH)/Screen.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Screen/Screen_Write_Character.c -o $(OBJECTS_PATH)/Screen_Write_Character.o

$(OBJECTS_PATH)/Screen_Write_Integer.o: $(SOURCES_PATH)/Screen/Screen_Write_Integer.c $(INCLUDES_PATH)/Screen.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Screen/Screen_Write_Integer.c -o $(OBJECTS_PATH)/Screen_Write_Integer.o

$(OBJECTS_PATH)/Screen_Write_String.o: $(SOURCES_PATH)/Screen/Screen_Write_String.c $(INCLUDES_PATH)/Screen.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Screen/Screen_Write_String.c -o $(OBJECTS_PATH)/Screen_Write_String.o

$(OBJECTS_PATH)/Screen_Write_Unsigned_Integer.o: $(SOURCES_PATH)/Screen/Screen_Write_Unsigned_Integer.c $(INCLUDES_PATH)/Screen.h $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/Screen/Screen_Write_Unsigned_Integer.c -o $(OBJECTS_PATH)/Screen_Write_Unsigned_Integer.o

#--------------------------------------------------------------------------------------------------
# System
#--------------------------------------------------------------------------------------------------
# This is the equivalent of the C runtime code, it must not be optimized out to preserve the system executable magic number (which is the beginning of the stack frame)
$(OBJECTS_PATH)/System_Program_Entry_Point.o: $(SOURCES_PATH)/System/System_Program_Entry_Point.c $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) -O0 $(SOURCES_PATH)/System/System_Program_Entry_Point.c -o $(OBJECTS_PATH)/System_Program_Entry_Point.o
	
$(OBJECTS_PATH)/System_Wait.o: $(SOURCES_PATH)/System/System_Wait.c $(INCLUDES_PATH)/System.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/System/System_Wait.c -o $(OBJECTS_PATH)/System_Wait.o

#--------------------------------------------------------------------------------------------------
# UART
#--------------------------------------------------------------------------------------------------
$(OBJECTS_PATH)/UART_Initialize.o: $(SOURCES_PATH)/UART/UART_Initialize.c $(INCLUDES_PATH)/System.h $(INCLUDES_PATH)/UART.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/UART/UART_Initialize.c -o $(OBJECTS_PATH)/UART_Initialize.o

$(OBJECTS_PATH)/UART_Is_Data_Received.o: $(SOURCES_PATH)/UART/UART_Is_Data_Received.c $(INCLUDES_PATH)/System.h $(INCLUDES_PATH)/UART.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/UART/UART_Is_Data_Received.c -o $(OBJECTS_PATH)/UART_Is_Data_Received.o

$(OBJECTS_PATH)/UART_Read_Byte.o: $(SOURCES_PATH)/UART/UART_Read_Byte.c $(INCLUDES_PATH)/System.h $(INCLUDES_PATH)/UART.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/UART/UART_Read_Byte.c -o $(OBJECTS_PATH)/UART_Read_Byte.o

$(OBJECTS_PATH)/UART_Write_Byte.o: $(SOURCES_PATH)/UART/UART_Write_Byte.c $(INCLUDES_PATH)/System.h $(INCLUDES_PATH)/UART.h
	$(CC) $(CCFLAGS) $(SOURCES_PATH)/UART/UART_Write_Byte.c -o $(OBJECTS_PATH)/UART_Write_Byte.o

clean:
	rm -rf $(BINARIES_PATH)/* $(OBJECTS_PATH)/*
