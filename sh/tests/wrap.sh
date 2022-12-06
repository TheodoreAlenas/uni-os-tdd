make_arg="$1"
to_run="$2"

make clean > /dev/null 2> /dev/null
make $make_arg > /dev/null && $to_run
make clean > /dev/null
