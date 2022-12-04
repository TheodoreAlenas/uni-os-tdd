#include <string.h>

#include "../src/util_for_main.h"
#include "test.h"

void test_arg_parse() {
  char *s = "--help";

  announce("help_defaults_to_off",
      parameters_parse(0, NULL)->show_help == false);
  announce("help_turns_on",
      parameters_parse(1, &s)->show_help == true);
}
