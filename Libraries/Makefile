# Makefile for program libraries generation
# Author : Adrien RICCIARDI
# V 1.0 : 27/04/2012
# V 1.1 : 28/07/2012
# V 1.2 : 28/04/2014, made a better organization.
# V 1.3 : 03/05/2014, System_Program_Entry_Point is now added to the archive and linked into a different section.
# V 1.4 : 06/07/2014, added Newlib 2.1.0 support and removed custom libc.
# V 1.5 : 25/10/2014, completely removed UNIX libc, using only Lemon libraries.
# V 1.6 : 07/12/2014, added dependencies generation for each file.
# V 1.7 : 19/03/2016, fully automated the build process.
include ../Global_Rules.mk

PATH_BINARIES = Binaries
PATH_INCLUDES = Includes
PATH_OBJECTS = Objects
PATH_SOURCES = Sources

INCLUDES = -IIncludes -I../System/Includes
CCFLAGS += -c -O2 $(INCLUDES)

ARCHIVE_NAME = Libraries.a

# Create the sources list
FOUND_SOURCES = $(shell find $(PATH_SOURCES) -name "*.c")
# Remove the System_Program_Entry_Point file from the list as it needs a special compilation rule to work fine
FILTERED_SOURCES = $(filter-out $(PATH_SOURCES)/System/System_Program_Entry_Point.c,$(FOUND_SOURCES))
# Remove the leading "Sources/" to avoid creating the Sources directory in the Objects directory
SOURCES = $(subst $(PATH_SOURCES)/,,$(FILTERED_SOURCES))

# Create the objects list based on the sources list
OBJECTS = $(patsubst %.c,$(PATH_OBJECTS)/%.o,$(SOURCES))
# Append special files
OBJECTS += $(PATH_OBJECTS)/System/System_Program_Entry_Point.o $(PATH_OBJECTS)/System/System_Call.o

all: $(OBJECTS)
	ar -rvsc $(PATH_BINARIES)/$(ARCHIVE_NAME) $(OBJECTS)
	ls -lh $(PATH_BINARIES)/$(ARCHIVE_NAME)
	
clean:
	rm -rf $(PATH_BINARIES)/* $(PATH_OBJECTS)/*
	
#--------------------------------------------------------------------------------------------------
# Special rules
#--------------------------------------------------------------------------------------------------
# This is the equivalent of the C runtime code, it must not be optimized out to preserve the system executable magic number (which is the beginning of the stack frame)
$(PATH_OBJECTS)/System/System_Program_Entry_Point.d: $(PATH_SOURCES)/System/System_Program_Entry_Point.c
	$(CC) $(CCFLAGS) -MT $(PATH_OBJECTS)/System/System_Program_Entry_Point.o -MM $(PATH_SOURCES)/System/System_Program_Entry_Point.c -MF $(PATH_OBJECTS)/System/System_Program_Entry_Point.d

# Using __attribute__((optimize(0)), __attribute__((optimize("O0")) or #pragma GCC optimize("O0") in the source file does not give the same result and builds a non-working object file, so this rule is needed as a work-around
# EDIT : using the attribute or pragma specified above works with gcc 5.3.1 but fails with gcc 4.9.2 (current Debian 8 gcc), so keep the work-around until Debian's gcc is upgraded
$(PATH_OBJECTS)/System/System_Program_Entry_Point.o: $(PATH_OBJECTS)/System/System_Program_Entry_Point.d
	$(CC) $(CCFLAGS) -O0 $(PATH_SOURCES)/System/System_Program_Entry_Point.c -o $(PATH_OBJECTS)/System/System_Program_Entry_Point.o

-include $(PATH_OBJECTS)/System/System_Program_Entry_Point.d

$(PATH_OBJECTS)/System/System_Call.o: $(PATH_SOURCES)/System/System_Call.asm
	$(AS) -f elf32 $(PATH_SOURCES)/System/System_Call.asm -o $(PATH_OBJECTS)/System/System_Call.o

#--------------------------------------------------------------------------------------------------
# Generic rules
#--------------------------------------------------------------------------------------------------
# Generic rule creating the header dependencies (.d makefile) for a .c file
$(PATH_OBJECTS)/%.d: $(PATH_SOURCES)/%.c
	@# Create the object subdirectories if needed
	@$(shell mkdir -p $(dir $@))
	@# Create a makefile named "Objects/.../File_Name.d" which contains a rule "Objects/.../File_Name.o: all_dependent_headers"
	$(CC) $(CCFLAGS) -MT $(patsubst %.d,%.o,$@) -MM $< -MF $@

# Generic rule compiling each .c file to an .o file
$(PATH_OBJECTS)/%.o: $(PATH_SOURCES)/%.c $(PATH_OBJECTS)/%.d
	@# Create the object subdirectories if needed
	@$(shell mkdir -p $(dir $@))
	$(CC) $(CCFLAGS) $< -o $@
	
# Include all "Objects/.../File_Name.d" makefile to add a specific rule for each file that provides all needed header prerequisites
-include $(OBJECTS:.o=.d)