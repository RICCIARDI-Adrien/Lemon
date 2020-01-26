# Lemon

**x86 32-bit minimal operating system**  
*Copyright (C) Adrien RICCIARDI*

## Help

* Type `help` to the Lemon prompt shell to get help, commands list and information about system usage.
* Whole system documentation can be found in repository [Documentation](https://github.com/RICCIARDI-Adrien/Lemon/tree/master/Documentation) directory.

## Technical documentation

### Creating a cross toolchain to build Lemon on a non-x86 platform

1. Install some required tools.
```
sudo apt install bison flex gawk help2man libtool-bin texinfo
```

2. Build [crosstool-NG](http://crosstool-ng.github.io), a tool that will automatically generate the toolchain.
```
# Change this variable by the lastest release version
CROSSTOOL_NG_VERSION=1.24.0

# Download latest stable version
wget http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-${CROSSTOOL_NG_VERSION}.tar.bz2
tar -xf crosstool-ng-${CROSSTOOL_NG_VERSION}.tar.bz2

# Build crosstool-NG to allow execution from build directory (this avoids installing it)
cd crosstool-ng-${CROSSTOOL_NG_VERSION}
./configure --enable-local
make
```

3. Configure toolchain generation by executing `./ct-ng menuconfig` :
   * In menu `Paths and misc options`, set `Prefix directory` variable to `/opt/Lemon_Toolchain`.
   * Only if building for Raspberry Pi 4 : still in `Paths and misc options` menu, set `Extra host compiler flags` variable to `-march=armv7-a -mfloat-abi=hard -mtune=cortex-a7`.
   * In menu `Target options`, select `x86` architecture in `Target Architecture` variable.
   * Exit saving the configuration.

4. Build and install toolchain. If an error happens, return to step 2 and build crosstool-NG from git.
```
# Allow crosstool-NG to access generated toolchain directory
sudo chmod 777 /opt

# Build and install toolchain
./ct-ng build
```
