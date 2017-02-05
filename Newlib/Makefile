PATH_BINARIES = $(realpath Binaries)
PATH_INCLUDES = $(realpath Includes)
PATH_OBJECTS = $(realpath Objects)

BUILD_TARGET=i586

all: configure compile install

configure:
	# --with-newlib --disable-shared parameters allow to build a fully static library
	# Install everything to objects directory as paths fine tuning via configure script seems to be not working
	cd $(PATH_OBJECTS) && \
	export AR_FOR_TARGET=ar && \
	export CC_FOR_TARGET=gcc && \
	export GCC_FOR_TARGET=gcc && \
	export LD_FOR_TARGET=ld && \
	export RANLIB_FOR_TARGET=ranlib && \
	export CFLAGS_FOR_TARGET="-m32 -O2" && \
	../newlib-cygwin/configure --target=$(BUILD_TARGET) --with-newlib --disable-shared --disable-newlib-multithread --disable-multilib --prefix=$(PATH_OBJECTS)

compile:
	cd $(PATH_OBJECTS) && make

install:
	cd $(PATH_OBJECTS) && make install
	cp -r $(PATH_OBJECTS)/$(BUILD_TARGET)/include/* $(PATH_INCLUDES)
	cp -r $(PATH_OBJECTS)/$(BUILD_TARGET)/lib/* $(PATH_BINARIES)

clean:
	rm -rf $(PATH_BINARIES)/* $(PATH_INCLUDES)/* $(PATH_OBJECTS)/*