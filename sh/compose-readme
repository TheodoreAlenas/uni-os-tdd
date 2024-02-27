#!/bin/env sh

extract () {
  name="$@"

  ans="$(git grep -n "for README, $name")"
  file="$(echo "$ans" | cut -d ':' -f 1)"
  line_number="$(echo "$ans" | cut -d ':' -f 2)"

  exit_on_end="NR > $line_number && /end of snippet/ {exit}"
  print_with_number='{printf "%+3s | %s\n", NR, $0}'
  after_match="NR > $line_number"

  awk_scr="$exit_on_end $after_match $print_with_number"

  awk "$awk_scr" "$file" | sed 's/ *$//'

  printf "[ file $file ]\n"
}


in_stdout () {
  in="$1"
  prev_line=0
  snippets="$(grep -n SNIPPET "$in")"

  for snippet in $snippets
  do
    line="$(echo "$snippet" | cut -d : -f 1)"
    name="$(echo "$snippet" | cut -d : -f 3)"

    sed -n "$((prev_line + 1)),$((line - 1))p" "$in"
    extract "$name" | awk '{print "    " $0}'

    prev_line="$line"
  done

  sed -n "$((prev_line + 1)),\$p" "$in"

  printf "\nLast update: $(date)\n"
}


in="sh/readme-src"
out="README"
test -n "$1" && in="$1"
test -n "$2" && out="$2"
in_stdout "$in" > "$out"

