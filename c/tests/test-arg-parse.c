#include <string.h>

#include "../src/util_for_main.h"
#include "test.h"

void test_arg_parse() {
  char *help_str = "--help";
  char *file_str = "filename.txt";

  announce("help_defaults_to_off",
      parameters_parse(0, NULL)->show_help == false);
  announce("help_turns_on",
      parameters_parse(1, &help_str)->show_help == true);
  announce("filename_gets_passed",
      0 == strcmp(file_str, parameters_parse(1, &file_str)->file_name));
}
