. sh/test-one

nope () {
  make clean > /dev/null
  exit 1
}

wrap () {
  if ! [ $(basename $PWD) = build ]
  then cd build || exit 1
  fi

  make_arg="$1"
  to_run="$2"
  verbose=no
  if [ "$3" = '-v' ]
  then verbose=yes
  fi

  make clean > /dev/null 2> /dev/null
  make $make_arg > /dev/null || nope
  if [ "$verbose" = yes ]
  then test_one -v "$to_run"
  else test_one "$to_run"
  fi
  test $? = 0 || nope
  make clean > /dev/null
}
