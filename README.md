# Husqvarna Automower(R) Doom
This is the official Husqvarna Automower(R) port of Doom initially released Spring 2024 as part of the "One Hell of an update" campaign.  
The port is based on a heavily stripped [stm32Doom](https://github.com/floppes/stm32doom), in turn based on [Chocolate Doom](https://github.com/chocolate-doom/chocolate-doom).

## Compiling DOOM

### Toolchain(s) and Tools
#### Make
A makefile is supplied with the source code. In order to compile with this, `make` is required.

#### ARMGCC
[GNU Arm Embedded Toolchain Downloads](https://developer.arm.com/downloads/-/gnu-rm)

The distributed binary has been compiled on Windows with GNU Arm Embedded Toolchain version `7-2017-q4-major`, however GNU Arm Embedded Toolchain version `10.3-2021.10` has been tested on Linux and found to be working as well.

Download GNU Arm Embedded Toolchain and extract/install it, then add the `bin` directory to your system path (either globally or temporarily in your current shell).

#### CubeMX 5.6.0 (optional)
[STM32CubeMX Downloads](https://www.st.com/en/development-tools/stm32cubemx.html)

CubeMX 5.6.0 has been used to configure most of the hardware/drivers. Other versions of CubeMX is not guaranteed to work. The CubeMX project file is [Port/stm32f469/CubeMX/app.ioc](Port/stm32f469/CubeMX/app.ioc).

Relevant source- and header files have been copied from the generated directories to the [CUBEMX](Port/stm32f469/CubeMX) folder.

The generated configuration files from CubeMX have been modified and adapted after generation, so if you generate and copy the files anew, make sure to diff the files before/after and restore relevant modifications.

If you are not planning on using a different hardware, there is no need to modify the CubeMX project or the generated files.

### Compiling the DOOM binary
To compile the DOOM binary, see to that both `make`, `arm-eabi-none-gcc`, `arm-eabi-none-size` and `arm-eabi-none-objcopy` is accessible via command line.

**The supplied `Makefile` has been tested in BASH and ZSH on Linux (ubuntu) as well as MSYS2/MinGW32 on windows.  
Some modification may be required to run in Windows PowerShell or with other toolchains.**

To temporarily add the GNU Arm Embedded Toolchain to your system path, instead of permanently adding it to your path, you can run the following:

**BASH:**
```bash
PATH=$PATH:/path/to/gcc-arm-none-eabi-{version}/bin
```
_modify the path to match your extracted directory location._

**POWERSHELL**:
```powershell
$Env:PATH = "$($Env:PATH)C:\Path\To\gcc-arm-none-eabi-{version}\bin;"
```
_modify the path to match your extracted directory location._

Then simply run `make`:

```bash
> make

Compiling main.c

.
.
.

Compiling w_wad.c

Compiling z_zone.c
Linking doom.elf
   text	   data	    bss	    dec	    hex	filename
 234272	  59608	5482632	5776512	 582480	out/doom.elf
Creating binary doom.bin

```

All files are built to `./out` and the `doom.bin` file is what shall be uploaded to the Automower(R).

#### GZIP
For a faster transfer, you can GZIP the `doom.bin` file before uploading.
```bash
gzip -k ./out/doom.bin
```
and transfer the `doom.bin.gz` file instead.

There's also the `make gzip` target if you have `gzip` in your path/shell.

### Transferring the doom binary the Automower(R)
In order to transfer the compiled binary to the Automower(R), the Husqvarna command-line `doomloader` tool is required as well as a **USB-C** cable. The tool is supplied as different packages in the release(s) available in this repository, for both Windows and Linux/Unix for x86 and x86_64. Extract the one you need and follow the instructions in the help text by running `doomloader --help` (see example below).

The USB-C port is located in the front of the Automower(R), labelled `Accessory Port`, and can be accessed by lifting the glossy panel from the front of the Automower(R).

After the binary has been transferred successfully, simply launch DOOM via the menu / interface on the Automower(R) itself.

```
> ./doomloader.exe --help

DoomLoader utility 0.0.1
Copyright (c) 2023 Husqvarna AB, All rights reserved.
(See LICENSE.txt for license details.)

This utility is used for loading a custom DOOM binary onto Husqvarna Automower(R).

Options:

 --help   Print this help text.
 --file   Binary file to load onto Husqvarna Automower(R).
 --port   Serial port connection to Husqvarna Automower(R) (ex. COM4 or /dev/tty3).
 --pin    Pin code of the connected Husqvarna Automower(R).

Example:

    doomloader.exe --file doom.bin --port /dev/tty3 --pin 1234


(Automower(R) is a registered trademark of Husqvarna AB.)
```

#### Linux specific issues
##### **Unable to execute file**
Make sure the file has the execute-flag set. If not, add it with `chmod +x ./doomloader`.

##### **Unable to access existing COM-port**
In linux, the user executing the binary needs to have access to the serial ports in order for the program to access the serial port interface. Make sure your user is part of the relevant group for your distro. Ex:

- In **ubuntu**, the group is called `dialout`.
- In **arch linux**, the group is called `uucp`.

After adding your user to the relevant group for your distro, you may need to restart the computer for the change to apply.

## doom1.wad
The `WAD`-file used in the Automower(R) is the shareware version of doom (`doom1.wad`, version 1.9, SHA-1 `5b2e249b9c5133ec987b3ea77596381dc0d6bc1d`).

The `WAD` file is not distributed as part of the source code, but is highly available from various sources if needed.

For updating the software on the Automower(R), the `WAD` file is not required, as it is already on the Automower(R).