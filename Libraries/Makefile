# Makefile for program libraries generation
# Author : Adrien RICCIARDI
# V 1.0 : 27/04/2012
# V 1.1 : 28/07/2012
# V 1.2 : 28/04/2014, made a better organization.
# V 1.3 : 03/05/2014, System_Program_Entry_Point is now added to the archive and linked into a different section.
# V 1.4 : 06/07/2014, added Newlib 2.1.0 support and removed custom libc.
# V 1.5 : 25/10/2014, completely removed UNIX libc, using only Lemon libraries.
# V 1.6 : 07/12/2014, added dependencies generation for each file.
PATH_BINARIES = Binaries
PATH_INCLUDES = Includes
PATH_OBJECTS = Objects
PATH_SOURCES = Sources

OBJECTS_FILE = $(PATH_OBJECTS)/File_Close.o $(PATH_OBJECTS)/File_Delete.o $(PATH_OBJECTS)/File_Exists.o $(PATH_OBJECTS)/File_Get_Size.o $(PATH_OBJECTS)/File_List_Initialize.o $(PATH_OBJECTS)/File_List_Next.o $(PATH_OBJECTS)/File_Open.o $(PATH_OBJECTS)/File_Read.o $(PATH_OBJECTS)/File_Write.o
OBJECTS_KEYBOARD = $(PATH_OBJECTS)/Keyboard_Is_Key_Available.o $(PATH_OBJECTS)/Keyboard_Read_Character.o
OBJECTS_MATH = $(PATH_OBJECTS)/Math_Integer_Add.o $(PATH_OBJECTS)/Math_Integer_Copy.o $(PATH_OBJECTS)/Math_Integer_Display_Hexadecimal.o $(PATH_OBJECTS)/Math_Integer_Initialize_From_Int.o
OBJECTS_MEMORY = $(PATH_OBJECTS)/Memory_Copy_Area.o $(PATH_OBJECTS)/Memory_Set_Area_Value.o
OBJECTS_RANDOM = $(PATH_OBJECTS)/Random_Generate_Number.o $(PATH_OBJECTS)/Random_Initialize.o
OBJECTS_SCREEN = $(PATH_OBJECTS)/Screen_Clear.o $(PATH_OBJECTS)/Screen_Display_Buffer.o $(PATH_OBJECTS)/Screen_Get_Cursor_Position.o $(PATH_OBJECTS)/Screen_Set_Background_Color.o $(PATH_OBJECTS)/Screen_Set_Cursor_Position.o $(PATH_OBJECTS)/Screen_Set_Font_Color.o $(PATH_OBJECTS)/Screen_Write_Character.o $(PATH_OBJECTS)/Screen_Write_Integer.o $(PATH_OBJECTS)/Screen_Write_String.o $(PATH_OBJECTS)/Screen_Write_Unsigned_Integer.o
OBJECTS_STRING = $(PATH_OBJECTS)/String_Compare.o $(PATH_OBJECTS)/String_Concatenate.o $(PATH_OBJECTS)/String_Convert_String_To_Unsigned_Integer.o $(PATH_OBJECTS)/String_Convert_Unsigned_Integer_To_String.o $(PATH_OBJECTS)/String_Copy.o $(PATH_OBJECTS)/String_Get_Size.o
OBJECTS_SYSTEM = $(PATH_OBJECTS)/System_Exit_Program.o $(PATH_OBJECTS)/System_Get_File_System_Total_Size.o $(PATH_OBJECTS)/System_Program_Entry_Point.o $(PATH_OBJECTS)/System_Wait.o
OBJECTS_UART = $(PATH_OBJECTS)/UART_Initialize.o $(PATH_OBJECTS)/UART_Is_Data_Received.o $(PATH_OBJECTS)/UART_Read_Byte.o $(PATH_OBJECTS)/UART_Write_Byte.o
OBJECTS = $(OBJECTS_FILE) $(OBJECTS_KEYBOARD) $(OBJECTS_MATH) $(OBJECTS_MEMORY) $(OBJECTS_RANDOM) $(OBJECTS_SCREEN) $(OBJECTS_STRING) $(OBJECTS_SYSTEM) $(OBJECTS_UART)

ARCHIVE_NAME = Libraries.a
INCLUDES = -IIncludes -I../System/Includes

CC = gcc
CCFLAGS += -c -nostdlib -fno-builtin -masm=intel -m32 -W -Wall -O2 $(INCLUDES)

all: $(OBJECTS)
	ar -rvsc $(PATH_BINARIES)/$(ARCHIVE_NAME) $(OBJECTS)
	ls -lh $(PATH_BINARIES)/$(ARCHIVE_NAME)
	
clean:
	rm -rf $(PATH_BINARIES)/* $(PATH_OBJECTS)/*

#--------------------------------------------------------------------------------------------------
# File
#--------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/File_Close.d: $(PATH_SOURCES)/File/File_Close.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/File/File_Close.c -MF $(PATH_OBJECTS)/File_Close.d
$(PATH_OBJECTS)/File_Close.o: $(PATH_OBJECTS)/File_Close.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/File/File_Close.c -o $(PATH_OBJECTS)/File_Close.o

$(PATH_OBJECTS)/File_Delete.d: $(PATH_SOURCES)/File/File_Delete.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/File/File_Delete.c -MF $(PATH_OBJECTS)/File_Delete.d
$(PATH_OBJECTS)/File_Delete.o: $(PATH_OBJECTS)/File_Delete.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/File/File_Delete.c -o $(PATH_OBJECTS)/File_Delete.o

$(PATH_OBJECTS)/File_Exists.d: $(PATH_SOURCES)/File/File_Exists.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/File/File_Exists.c -MF $(PATH_OBJECTS)/File_Exists.d
$(PATH_OBJECTS)/File_Exists.o: $(PATH_OBJECTS)/File_Exists.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/File/File_Exists.c -o $(PATH_OBJECTS)/File_Exists.o

$(PATH_OBJECTS)/File_Get_Size.d: $(PATH_SOURCES)/File/File_Get_Size.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/File/File_Get_Size.c -MF $(PATH_OBJECTS)/File_Get_Size.d
$(PATH_OBJECTS)/File_Get_Size.o: $(PATH_OBJECTS)/File_Get_Size.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/File/File_Get_Size.c -o $(PATH_OBJECTS)/File_Get_Size.o

$(PATH_OBJECTS)/File_List_Initialize.d: $(PATH_SOURCES)/File/File_List_Initialize.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/File/File_List_Initialize.c -MF $(PATH_OBJECTS)/File_List_Initialize.d
$(PATH_OBJECTS)/File_List_Initialize.o: $(PATH_OBJECTS)/File_List_Initialize.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/File/File_List_Initialize.c -o $(PATH_OBJECTS)/File_List_Initialize.o

$(PATH_OBJECTS)/File_List_Next.d: $(PATH_SOURCES)/File/File_List_Next.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/File/File_List_Next.c -MF $(PATH_OBJECTS)/File_List_Next.d
$(PATH_OBJECTS)/File_List_Next.o: $(PATH_OBJECTS)/File_List_Next.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/File/File_List_Next.c -o $(PATH_OBJECTS)/File_List_Next.o

$(PATH_OBJECTS)/File_Open.d: $(PATH_SOURCES)/File/File_Open.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/File/File_Open.c -MF $(PATH_OBJECTS)/File_Open.d
$(PATH_OBJECTS)/File_Open.o: $(PATH_OBJECTS)/File_Open.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/File/File_Open.c -o $(PATH_OBJECTS)/File_Open.o

$(PATH_OBJECTS)/File_Read.d: $(PATH_SOURCES)/File/File_Read.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/File/File_Read.c -MF $(PATH_OBJECTS)/File_Read.d
$(PATH_OBJECTS)/File_Read.o: $(PATH_OBJECTS)/File_Read.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/File/File_Read.c -o $(PATH_OBJECTS)/File_Read.o

$(PATH_OBJECTS)/File_Write.d: $(PATH_SOURCES)/File/File_Write.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/File/File_Write.c -MF $(PATH_OBJECTS)/File_Write.d
$(PATH_OBJECTS)/File_Write.o: $(PATH_OBJECTS)/File_Write.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/File/File_Write.c -o $(PATH_OBJECTS)/File_Write.o
	
#--------------------------------------------------------------------------------------------------
# Keyboard
#--------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Keyboard_Is_Key_Available.d: $(PATH_SOURCES)/Keyboard/Keyboard_Is_Key_Available.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Keyboard/Keyboard_Is_Key_Available.c -MF $(PATH_OBJECTS)/Keyboard_Is_Key_Available.d
$(PATH_OBJECTS)/Keyboard_Is_Key_Available.o: $(PATH_OBJECTS)/Keyboard_Is_Key_Available.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Keyboard/Keyboard_Is_Key_Available.c -o $(PATH_OBJECTS)/Keyboard_Is_Key_Available.o

$(PATH_OBJECTS)/Keyboard_Read_Character.d: $(PATH_SOURCES)/Keyboard/Keyboard_Read_Character.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Keyboard/Keyboard_Read_Character.c -MF $(PATH_OBJECTS)/Keyboard_Read_Character.d
$(PATH_OBJECTS)/Keyboard_Read_Character.o: $(PATH_OBJECTS)/Keyboard_Read_Character.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Keyboard/Keyboard_Read_Character.c -o $(PATH_OBJECTS)/Keyboard_Read_Character.o

#--------------------------------------------------------------------------------------------------
# Math
#--------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Math_Integer_Add.d: $(PATH_SOURCES)/Math/Math_Integer_Add.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Math/Math_Integer_Add.c -MF $(PATH_OBJECTS)/Math_Integer_Add.d
$(PATH_OBJECTS)/Math_Integer_Add.o: $(PATH_OBJECTS)/Math_Integer_Add.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Math/Math_Integer_Add.c -o $(PATH_OBJECTS)/Math_Integer_Add.o

$(PATH_OBJECTS)/Math_Integer_Copy.d: $(PATH_SOURCES)/Math/Math_Integer_Copy.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Math/Math_Integer_Copy.c -MF $(PATH_OBJECTS)/Math_Integer_Copy.d
$(PATH_OBJECTS)/Math_Integer_Copy.o: $(PATH_OBJECTS)/Math_Integer_Copy.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Math/Math_Integer_Copy.c -o $(PATH_OBJECTS)/Math_Integer_Copy.o

$(PATH_OBJECTS)/Math_Integer_Display_Hexadecimal.d: $(PATH_SOURCES)/Math/Math_Integer_Display_Hexadecimal.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Math/Math_Integer_Display_Hexadecimal.c -MF $(PATH_OBJECTS)/Math_Integer_Display_Hexadecimal.d
$(PATH_OBJECTS)/Math_Integer_Display_Hexadecimal.o: $(PATH_OBJECTS)/Math_Integer_Display_Hexadecimal.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Math/Math_Integer_Display_Hexadecimal.c -o $(PATH_OBJECTS)/Math_Integer_Display_Hexadecimal.o

$(PATH_OBJECTS)/Math_Integer_Initialize_From_Int.d: $(PATH_SOURCES)/Math/Math_Integer_Initialize_From_Int.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Math/Math_Integer_Initialize_From_Int.c -MF $(PATH_OBJECTS)/Math_Integer_Initialize_From_Int.d
$(PATH_OBJECTS)/Math_Integer_Initialize_From_Int.o: $(PATH_OBJECTS)/Math_Integer_Initialize_From_Int.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Math/Math_Integer_Initialize_From_Int.c -o $(PATH_OBJECTS)/Math_Integer_Initialize_From_Int.o

#--------------------------------------------------------------------------------------------------
# Memory
#--------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Memory_Copy_Area.d: $(PATH_SOURCES)/Memory/Memory_Copy_Area.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Memory/Memory_Copy_Area.c -MF $(PATH_OBJECTS)/Memory_Copy_Area.d
$(PATH_OBJECTS)/Memory_Copy_Area.o: $(PATH_OBJECTS)/Memory_Copy_Area.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Memory/Memory_Copy_Area.c -o $(PATH_OBJECTS)/Memory_Copy_Area.o

$(PATH_OBJECTS)/Memory_Set_Area_Value.d: $(PATH_SOURCES)/Memory/Memory_Set_Area_Value.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Memory/Memory_Set_Area_Value.c -MF $(PATH_OBJECTS)/Memory_Set_Area_Value.d
$(PATH_OBJECTS)/Memory_Set_Area_Value.o: $(PATH_OBJECTS)/Memory_Set_Area_Value.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Memory/Memory_Set_Area_Value.c -o $(PATH_OBJECTS)/Memory_Set_Area_Value.o

#--------------------------------------------------------------------------------------------------
# Random
#--------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Random_Generate_Number.d: $(PATH_SOURCES)/Random/Random_Generate_Number.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Random/Random_Generate_Number.c -MF $(PATH_OBJECTS)/Random_Generate_Number.d
$(PATH_OBJECTS)/Random_Generate_Number.o: $(PATH_OBJECTS)/Random_Generate_Number.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Random/Random_Generate_Number.c -o $(PATH_OBJECTS)/Random_Generate_Number.o

$(PATH_OBJECTS)/Random_Initialize.d: $(PATH_SOURCES)/Random/Random_Initialize.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Random/Random_Initialize.c -MF $(PATH_OBJECTS)/Random_Initialize.d
$(PATH_OBJECTS)/Random_Initialize.o: $(PATH_OBJECTS)/Random_Initialize.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Random/Random_Initialize.c -o $(PATH_OBJECTS)/Random_Initialize.o

#--------------------------------------------------------------------------------------------------
# Screen
#--------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/Screen_Clear.d: $(PATH_SOURCES)/Screen/Screen_Clear.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Screen/Screen_Clear.c -MF $(PATH_OBJECTS)/Screen_Clear.d
$(PATH_OBJECTS)/Screen_Clear.o: $(PATH_OBJECTS)/Screen_Clear.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Screen/Screen_Clear.c -o $(PATH_OBJECTS)/Screen_Clear.o

$(PATH_OBJECTS)/Screen_Display_Buffer.d: $(PATH_SOURCES)/Screen/Screen_Display_Buffer.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Screen/Screen_Display_Buffer.c -MF $(PATH_OBJECTS)/Screen_Display_Buffer.d
$(PATH_OBJECTS)/Screen_Display_Buffer.o: $(PATH_OBJECTS)/Screen_Display_Buffer.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Screen/Screen_Display_Buffer.c -o $(PATH_OBJECTS)/Screen_Display_Buffer.o

$(PATH_OBJECTS)/Screen_Get_Cursor_Position.d: $(PATH_SOURCES)/Screen/Screen_Get_Cursor_Position.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Screen/Screen_Get_Cursor_Position.c -MF $(PATH_OBJECTS)/Screen_Get_Cursor_Position.d
$(PATH_OBJECTS)/Screen_Get_Cursor_Position.o: $(PATH_OBJECTS)/Screen_Get_Cursor_Position.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Screen/Screen_Get_Cursor_Position.c -o $(PATH_OBJECTS)/Screen_Get_Cursor_Position.o

$(PATH_OBJECTS)/Screen_Set_Background_Color.d: $(PATH_SOURCES)/Screen/Screen_Set_Background_Color.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Screen/Screen_Set_Background_Color.c -MF $(PATH_OBJECTS)/Screen_Set_Background_Color.d
$(PATH_OBJECTS)/Screen_Set_Background_Color.o: $(PATH_OBJECTS)/Screen_Set_Background_Color.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Screen/Screen_Set_Background_Color.c -o $(PATH_OBJECTS)/Screen_Set_Background_Color.o

$(PATH_OBJECTS)/Screen_Set_Cursor_Position.d: $(PATH_SOURCES)/Screen/Screen_Set_Cursor_Position.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Screen/Screen_Set_Cursor_Position.c -MF $(PATH_OBJECTS)/Screen_Set_Cursor_Position.d
$(PATH_OBJECTS)/Screen_Set_Cursor_Position.o: $(PATH_OBJECTS)/Screen_Set_Cursor_Position.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Screen/Screen_Set_Cursor_Position.c -o $(PATH_OBJECTS)/Screen_Set_Cursor_Position.o

$(PATH_OBJECTS)/Screen_Set_Font_Color.d: $(PATH_SOURCES)/Screen/Screen_Set_Font_Color.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Screen/Screen_Set_Font_Color.c -MF $(PATH_OBJECTS)/Screen_Set_Font_Color.d
$(PATH_OBJECTS)/Screen_Set_Font_Color.o: $(PATH_OBJECTS)/Screen_Set_Font_Color.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Screen/Screen_Set_Font_Color.c -o $(PATH_OBJECTS)/Screen_Set_Font_Color.o

$(PATH_OBJECTS)/Screen_Write_Character.d: $(PATH_SOURCES)/Screen/Screen_Write_Character.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Screen/Screen_Write_Character.c -MF $(PATH_OBJECTS)/Screen_Write_Character.d
$(PATH_OBJECTS)/Screen_Write_Character.o: $(PATH_OBJECTS)/Screen_Write_Character.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Screen/Screen_Write_Character.c -o $(PATH_OBJECTS)/Screen_Write_Character.o

$(PATH_OBJECTS)/Screen_Write_Integer.d: $(PATH_SOURCES)/Screen/Screen_Write_Integer.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Screen/Screen_Write_Integer.c -MF $(PATH_OBJECTS)/Screen_Write_Integer.d
$(PATH_OBJECTS)/Screen_Write_Integer.o: $(PATH_OBJECTS)/Screen_Write_Integer.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Screen/Screen_Write_Integer.c -o $(PATH_OBJECTS)/Screen_Write_Integer.o

$(PATH_OBJECTS)/Screen_Write_String.d: $(PATH_SOURCES)/Screen/Screen_Write_String.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Screen/Screen_Write_String.c -MF $(PATH_OBJECTS)/Screen_Write_String.d
$(PATH_OBJECTS)/Screen_Write_String.o: $(PATH_OBJECTS)/Screen_Write_String.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Screen/Screen_Write_String.c -o $(PATH_OBJECTS)/Screen_Write_String.o

$(PATH_OBJECTS)/Screen_Write_Unsigned_Integer.d: $(PATH_SOURCES)/Screen/Screen_Write_Unsigned_Integer.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/Screen/Screen_Write_Unsigned_Integer.c -MF $(PATH_OBJECTS)/Screen_Write_Unsigned_Integer.d
$(PATH_OBJECTS)/Screen_Write_Unsigned_Integer.o: $(PATH_OBJECTS)/Screen_Write_Unsigned_Integer.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/Screen/Screen_Write_Unsigned_Integer.c -o $(PATH_OBJECTS)/Screen_Write_Unsigned_Integer.o

#--------------------------------------------------------------------------------------------------
# String
#--------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/String_Compare.d: $(PATH_SOURCES)/String/String_Compare.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/String/String_Compare.c -MF $(PATH_OBJECTS)/String_Compare.d
$(PATH_OBJECTS)/String_Compare.o: $(PATH_OBJECTS)/String_Compare.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/String/String_Compare.c -o $(PATH_OBJECTS)/String_Compare.o

$(PATH_OBJECTS)/String_Concatenate.d: $(PATH_SOURCES)/String/String_Concatenate.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/String/String_Concatenate.c -MF $(PATH_OBJECTS)/String_Concatenate.d
$(PATH_OBJECTS)/String_Concatenate.o: $(PATH_OBJECTS)/String_Concatenate.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/String/String_Concatenate.c -o $(PATH_OBJECTS)/String_Concatenate.o

$(PATH_OBJECTS)/String_Convert_String_To_Unsigned_Integer.d: $(PATH_SOURCES)/String/String_Convert_String_To_Unsigned_Integer.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/String/String_Convert_String_To_Unsigned_Integer.c -MF $(PATH_OBJECTS)/String_Convert_String_To_Unsigned_Integer.d
$(PATH_OBJECTS)/String_Convert_String_To_Unsigned_Integer.o: $(PATH_OBJECTS)/String_Convert_String_To_Unsigned_Integer.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/String/String_Convert_String_To_Unsigned_Integer.c -o $(PATH_OBJECTS)/String_Convert_String_To_Unsigned_Integer.o

$(PATH_OBJECTS)/String_Convert_Unsigned_Integer_To_String.d: $(PATH_SOURCES)/String/String_Convert_Unsigned_Integer_To_String.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/String/String_Convert_Unsigned_Integer_To_String.c -MF $(PATH_OBJECTS)/String_Convert_Unsigned_Integer_To_String.d
$(PATH_OBJECTS)/String_Convert_Unsigned_Integer_To_String.o: $(PATH_OBJECTS)/String_Convert_Unsigned_Integer_To_String.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/String/String_Convert_Unsigned_Integer_To_String.c -o $(PATH_OBJECTS)/String_Convert_Unsigned_Integer_To_String.o

$(PATH_OBJECTS)/String_Copy.d: $(PATH_SOURCES)/String/String_Copy.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/String/String_Copy.c -MF $(PATH_OBJECTS)/String_Copy.d
$(PATH_OBJECTS)/String_Copy.o: $(PATH_OBJECTS)/String_Copy.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/String/String_Copy.c -o $(PATH_OBJECTS)/String_Copy.o

$(PATH_OBJECTS)/String_Get_Size.d: $(PATH_SOURCES)/String/String_Get_Size.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/String/String_Get_Size.c -MF $(PATH_OBJECTS)/String_Get_Size.d
$(PATH_OBJECTS)/String_Get_Size.o: $(PATH_OBJECTS)/String_Get_Size.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/String/String_Get_Size.c -o $(PATH_OBJECTS)/String_Get_Size.o

#--------------------------------------------------------------------------------------------------
# System
#--------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/System_Exit_Program.d: $(PATH_SOURCES)/System/System_Exit_Program.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/System/System_Exit_Program.c -MF $(PATH_OBJECTS)/System_Exit_Program.d
$(PATH_OBJECTS)/System_Exit_Program.o: $(PATH_OBJECTS)/System_Exit_Program.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/System/System_Exit_Program.c -o $(PATH_OBJECTS)/System_Exit_Program.o

$(PATH_OBJECTS)/System_Get_File_System_Total_Size.d: $(PATH_SOURCES)/System/System_Get_File_System_Total_Size.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/System/System_Get_File_System_Total_Size.c -MF $(PATH_OBJECTS)/System_Get_File_System_Total_Size.d
$(PATH_OBJECTS)/System_Get_File_System_Total_Size.o: $(PATH_OBJECTS)/System_Get_File_System_Total_Size.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/System/System_Get_File_System_Total_Size.c -o $(PATH_OBJECTS)/System_Get_File_System_Total_Size.o

# This is the equivalent of the C runtime code, it must not be optimized out to preserve the system executable magic number (which is the beginning of the stack frame)
$(PATH_OBJECTS)/System_Program_Entry_Point.d: $(PATH_SOURCES)/System/System_Program_Entry_Point.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/System/System_Program_Entry_Point.c -MF $(PATH_OBJECTS)/System_Program_Entry_Point.d
$(PATH_OBJECTS)/System_Program_Entry_Point.o: $(PATH_OBJECTS)/System_Program_Entry_Point.d
	$(CC) $(CCFLAGS) -O0 $(PATH_SOURCES)/System/System_Program_Entry_Point.c -o $(PATH_OBJECTS)/System_Program_Entry_Point.o

$(PATH_OBJECTS)/System_Wait.d: $(PATH_SOURCES)/System/System_Wait.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/System/System_Wait.c -MF $(PATH_OBJECTS)/System_Wait.d
$(PATH_OBJECTS)/System_Wait.o: $(PATH_OBJECTS)/System_Wait.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/System/System_Wait.c -o $(PATH_OBJECTS)/System_Wait.o

#--------------------------------------------------------------------------------------------------
# UART
#--------------------------------------------------------------------------------------------------
$(PATH_OBJECTS)/UART_Initialize.d: $(PATH_SOURCES)/UART/UART_Initialize.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/UART/UART_Initialize.c -MF $(PATH_OBJECTS)/UART_Initialize.d
$(PATH_OBJECTS)/UART_Initialize.o: $(PATH_OBJECTS)/UART_Initialize.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/UART/UART_Initialize.c -o $(PATH_OBJECTS)/UART_Initialize.o

$(PATH_OBJECTS)/UART_Is_Data_Received.d: $(PATH_SOURCES)/UART/UART_Is_Data_Received.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/UART/UART_Is_Data_Received.c -MF $(PATH_OBJECTS)/UART_Is_Data_Received.d
$(PATH_OBJECTS)/UART_Is_Data_Received.o: $(PATH_OBJECTS)/UART_Is_Data_Received.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/UART/UART_Is_Data_Received.c -o $(PATH_OBJECTS)/UART_Is_Data_Received.o

$(PATH_OBJECTS)/UART_Read_Byte.d: $(PATH_SOURCES)/UART/UART_Read_Byte.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/UART/UART_Read_Byte.c -MF $(PATH_OBJECTS)/UART_Read_Byte.d
$(PATH_OBJECTS)/UART_Read_Byte.o: $(PATH_OBJECTS)/UART_Read_Byte.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/UART/UART_Read_Byte.c -o $(PATH_OBJECTS)/UART_Read_Byte.o

$(PATH_OBJECTS)/UART_Write_Byte.d: $(PATH_SOURCES)/UART/UART_Write_Byte.c
	$(CC) $(CCFLAGS) -MM $(PATH_SOURCES)/UART/UART_Write_Byte.c -MF $(PATH_OBJECTS)/UART_Write_Byte.d
$(PATH_OBJECTS)/UART_Write_Byte.o: $(PATH_OBJECTS)/UART_Write_Byte.d
	$(CC) $(CCFLAGS) $(PATH_SOURCES)/UART/UART_Write_Byte.c -o $(PATH_OBJECTS)/UART_Write_Byte.o
