#!/usr/bin/env sh

set -em

DIR=$(dirname $0)

if [ $# -ne 1 ]; then
  echo "usage: debug.sh FILE"
  exit 1
fi

ELF="$1"

# flash file for debugging
. "$DIR/flash.sh" "$ELF"

trap '' SIGINT

# start gdb-server
st-util &

arm-none-eabi-gdb -tui -ex "file $ELF" -ex 'target remote :4242' 

# launch debugger and connect to gdb-server
# lldb "$ELF" -o "platform select remote-gdb-server" -o "gdb-remote 4242"
