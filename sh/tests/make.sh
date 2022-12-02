#! /bin/env sh

make all > /dev/null   || exit 1
test -f build/manager  || exit 1
test -f build/listener || exit 1
test -f build/worker   || exit 1

make clean > /dev/null || exit 1
test -z "$(ls build)"  || exit 1
