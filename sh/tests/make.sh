#! /bin/env sh

BUILD=build

# takes one argument for what to give to "make",
# like "all" for "make all", and the other arguments
# are expected files. Just the filenames,
# not the whole path.

successful=yes-so-far

is_first=yes
for arg do

  if [ $is_first = yes ]
  then
    make "$arg" > /dev/null
    is_first=no
    continue
  fi

  if ! [ -f "$BUILD/$arg" ]
  then
    echo "didn't find build file $BUILD/$arg" >&2
    successful=no
  fi

done

if [ $successful = no ]
then {
  printf "what there is is:\n"
  ls "$BUILD"
} >&2
fi

make clean > /dev/null || exit 1
test -z "$(ls "$BUILD")"  || exit 1

if [ $successful = no ]
then
  printf "make $1 failed\n" >&2
  exit 1
fi

