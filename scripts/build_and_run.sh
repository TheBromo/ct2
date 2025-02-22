#!/usr/bin/env sh

DIR=$(dirname $0)

if [ $# -ne 1 ]; then
  echo "usage: build_and_run.sh FILE"
  exit 1
fi

# build
gn gen out
ninja -C out

# run
. "$DIR/flash.sh" "$1"
