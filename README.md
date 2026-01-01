# Lemon

**A x86 32-bit minimal operating system designed and developed from scratch.**  
*Copyright (C) Adrien RICCIARDI*

## Building

These instructions have been tested on an AMD64 Debian 13 (Trixie).

### Install the required tools
```
sudo apt install build-essential genisoimage kconfig-frontends nasm qemu-system-x86 wodim
```

### Configure the system

You can configure the system from scratch by using :
```
make menuconfig
```
Or, you can apply an already existing configuration. If you type :
```
make Configurations
```
And then press the `Tab` key, you will see all the available configurations.

### Build

You can now build the entire system with the single command :
```
make
```

## Testing

Three system installation images will be generated :
* `Lemon_Installer_CD_Image.iso` : to burn to a CD-ROM, you can use the `sudo make burn-cdrom-image`.
* `Lemon_Installer_Floppy_Image.img` : to write to a 1.44MB floppy disk, you can use the `sudo make burn-floppy-image`.
* `Lemon_Installer_USB_Stick_Image.img` : to write to an USB stick, you can use `sudo dd if=<path/to/Lemon_Installer_USB_Stick_Image.img> of=/dev/<path/to/usb_device/device_node>`

You can either test on real hardware, or use a virtual machine.  
To run the system on a virtual machine, you can use two helper commands :
* `make qemu-install`: start the system installer on a virtual machine, then reboot into the installed system.
* `make qemu-run` : start the virtual machine.

## Using the system

* Type `help` to the Lemon prompt shell to get help, commands list and information about the system usage.
* The whole system documentation can be found in the repository [Documentation](Documentation) directory.

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
