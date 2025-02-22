# CT2

Sources for the CT2 course at ZHAW (FS-2023).

## Project Layout

* `build/` Build configurations such as linker script and project wide configuration files.
* `lib/` Additional source code of the CT board itself based on the 
  [Keil uVision Pack provided by InES (version 4.0.3)](https://ennis.zhaw.ch/pack/InES.CTBoard14_DFP.4.0.3.pack). The 
  assembly files have been manually translated into GCC compatible syntax and need patching when upgrading to a newer 
  pack version.
* `src/` Personal source code solutions of the labs

## Requirements

* `arm-none-eabi-gcc` (cross-compiler)
* `arm-none-eabi-newlib` (C standard library)
* `stlink` (flash and debug tool)

### Build System

In this repository, the `gn` build tool is used to generate `ninja` build files. However you could use your own build
system by following the steps described in [this](#using-custom-build-system) chapter

* `gn` (configuration system)
* `ninja` (build system)

### Debugging

If you want to be able to debug the uploaded code on the CT-board, you will need to either install `lldb` or 
`arm-none-eabi-gdb`. 

**Note:** Since `gdb` is currently not compatible with the Apple ARM chips, you will need to use `lldb` on
those machines.

### Installation

* Arch: `pacman -S arm-none-eabi-gcc arm-none-eabi-newlib stlink gn ninja lldb`
* MacOS (MacPorts): `port install arm-none-eabi-gcc stlink gn-devel ninja lldb-15`


## Building

### Using provided build system

1. `gn gen out`
2. `ninja -C out`

This builds all the startup and board libraries as well as all the labs inside the `src/` directory into the `out/` 
directory.

### Using custom build system

When compiling the code yourself make sure, you provide at least the following flags to `gcc`:

* `-Ilib/device/include -Ilib/device/include/m4 -Ilib/hal/include` include directories for the compiler to find the
  header files to the CT-board specific implementation.
* `-mcpu=cortex-m4` selects the CPU type to generate and optimize the binary code for.
* `-mthumb` specifies that the assembler should use the thumb mode when compiling the source code.
* `-mlittle-endian` generate code for a little-endian target (technically set by default at least in `gcc-12.2.0`).

The invocation of the compiler could then look something similar to this.

```sh
arm-none-eabi-gcc             \
  -Ilib/device/include        \
  -Ilib/device/include/m4     \
  -Ilib/hal/include           \
  -mcpu=cortex-m4             \
  -mthumb                     \
  -mlittle-endian             \
  -MMD                        \
  -MF [DEP_FILE]              \
  -c                          \
  -o [OUT_FILE].o             \
  [SOURCE_FILE].{c,s}
```

When linking you additionally have to provide additional flags in order to place the code at the correct memory 
locations and 

* `-nostartfiles` disables the default startup files from the standard library, so that we can use our own (see 
  [lib/device/src/startup_ctboard.s](lib/device/src/startup_ctboard.s))
* `-specs=nosys.specs` fixes the undefined reference errors from the standard library due to the fact that the bard
  does not run an operating system and therefore functions like `_close`, `_exit`, `_read`, etc. are not available. 
* `-Tbuild/linker.ld` tells the linker at which addresses to place code, stack, heap and bss sections of the binary (see 
  [build/linker.ld](build/linker.ld)).

The invocation of the linker could then look something similar to this.

```sh
arm-none-eabi-gcc             \
  -Ilib/device/include        \
  -Ilib/device/include/m4     \
  -Ilib/hal/include           \
  -mcpu=cortex-m4             \
  -mthumb                     \
  -mlittle-endian             \
  -nostartfiles               \
  -specs=nosys.specs          \
  -Tbuild/linker.ld           \
  -o [OUT_FILE].elf           \
  [OBJ_FILES...]
```

At this point you have an ELF binary, which cannot be flashed to the device directly. You need to convert it from
the ELF format into binary format using the following command:

```sh
arm-none-eabi-objcopy -O binary [ELF_FILE] [BIN_FILE]
```

## Flashing

To flash the binary onto the board, you need the `st-flash` tool from the `stlink` dependency.

```sh
st-flash --reset write [BIN_FILE] 0x8000000
```

This repository provides a helper script to generate the binary file from the elf file and flash it onto the board 
directly.

```sh
scripts/flash.sh [ELF_FILE]
```

## Debugging

To flash the binary onto the board, you need the `st-util` tool from the `stlink` dependency. You first need to launch
a gdb-server instance and can then attach either `arm-none-eabi-gdb` or `lldb` to the server.

```sh
# launch gdb-server
st-util

# launch lldb
lldb [ELF_FILE]
(lldb) platform select remote-gdb-server
(lldb) gdb-remote 4242

# launch lldb and directly connect to gdb-server
lldb [ELF_FILE] -o "platform select remote-gdb-server" -o "gdb-remote 4242"
```

*Note:* you don't have to provide the elf file to the debugger instance, however if you want to have debug symbols you 
should add it and compile the elf file with the `-g -ggdb` flags. Otherwise you will only be able to view the raw 
assembly code currently getting executed on the board.

Inside this repository you can also find a script, which automatically converts, flashes and attaches the lldb debugger 
to the CT-board. 
```sh
scripts/debug.sh [ELF_FILE]
```

## License

```
Copyright (C) 2023 Florian Walter

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```