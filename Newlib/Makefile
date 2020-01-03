# Newlib makefile (no newlib file is modified to allow using the library as a git sub-module).
# Author : Adrien RICCIARDI

PATH_BINARIES = $(realpath Binaries)
PATH_INCLUDES = $(realpath Includes)
PATH_OBJECTS = $(realpath Objects)
PATH_SOURCES = $(realpath Sources)

BUILD_TARGET=i586

# List all system calls source files (absolute paths)
NEWLIB_SYSTEM_CALLS_SOURCES_ABSOLUTE_PATHS = $(shell find $(PATH_SOURCES) -name "*.c")
# Remove each file absolute path to keep only the file name
NEWLIB_SYSTEM_CALLS_SOURCES = $(subst $(PATH_SOURCES)/,,$(NEWLIB_SYSTEM_CALLS_SOURCES_ABSOLUTE_PATHS))
# Create all newlib system calls object files from source files
NEWLIB_SYSTEM_CALLS_OBJECTS = $(patsubst %.c,$(PATH_OBJECTS)/%.o,$(NEWLIB_SYSTEM_CALLS_SOURCES))

# Lemon system calls must be built after newlib because newlib headers are needed
all: configure compile install lemon_newlib_system_calls

# Build all Lemon newlib system calls and generate the corresponding library
lemon_newlib_system_calls: $(NEWLIB_SYSTEM_CALLS_OBJECTS)
	ar -rvsc $(PATH_BINARIES)/Lemon_System_Calls.a $(NEWLIB_SYSTEM_CALLS_OBJECTS)

# Generic rule compiling each system call .c file to an .o file
$(PATH_OBJECTS)/%.o: $(PATH_SOURCES)/%.c
	@# Allow access to Libraries headers (and System headers needed by Libraries ones), and allow access to newlib headers (not build system ones)
	@# See configure rules comments for __linux__ macro undefinition
	$(GLOBAL_TOOL_COMPILER) $(CCFLAGS) -c -U__linux__ -I../Libraries/Includes -I../System/Includes -I$(PATH_INCLUDES) $< -o $@

configure:
	@# --with-newlib --disable-shared parameters allow to build a fully static library
	@# Install everything to objects directory as paths fine tuning via configure script seems to be not working
	@# Set -fno-pic to avoid generating Position Independent Code, used only for dynamic libraries (here only static libraries are needed)
	@# Undefined standard __linux__ macro as it was set on the build system and enabled unwanted __LARGE64_FILES support
	cd $(PATH_OBJECTS) && \
	export AR_FOR_TARGET=ar && \
	export CC_FOR_TARGET=gcc && \
	export GCC_FOR_TARGET=gcc && \
	export LD_FOR_TARGET=ld && \
	export RANLIB_FOR_TARGET=ranlib && \
	export READELF_FOR_TARGET=readelf && \
	export CFLAGS_FOR_TARGET="-m32 -O2 -fno-pic -U__linux__" && \
	../newlib-cygwin/configure --target=$(BUILD_TARGET) --with-newlib --disable-shared --disable-newlib-multithread --disable-multilib --prefix=$(PATH_OBJECTS)

compile:
	cd $(PATH_OBJECTS) && make -j $(HOST_PROCESSORS_COUNT)

install:
	cd $(PATH_OBJECTS) && make install
	cp -r $(PATH_OBJECTS)/$(BUILD_TARGET)/include/* $(PATH_INCLUDES)
	cp -r $(PATH_OBJECTS)/$(BUILD_TARGET)/lib/* $(PATH_BINARIES)

clean:
	rm -rf $(PATH_BINARIES)/* $(PATH_INCLUDES)/* $(PATH_OBJECTS)/*