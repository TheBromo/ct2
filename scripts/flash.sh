#!/usr/bin/env sh

set -e

OBJCOPY="arm-none-eabi-objcopy"
STFLASH="st-flash"

if [ $# -ne 1 ]; then
  echo "usage: flash.sh FILE"
  exit 1
fi

ELF="$1"
BIN="$ELF.bin"

if [ ! -f "$ELF" ]; then
  echo "flash.sh: input file $ELF does not exist"
  exit 1
fi

# convert elf file into binary file
$OBJCOPY -O binary "$ELF" "$BIN"

# flash binary file onto chip
$STFLASH --reset write "$BIN" 0x8000000
