#include <string.h>

#include "../src/params.h"
#include "test.h"

void test_arg_parse() {
  char *path_str = "/some/path/to/binary";
  char *help_str = "--help";
  char *file_str = "filename.txt";

  char *help_b[] = { path_str, help_str };
  char *file_b[] = { path_str, file_str };

  announce("help_defaults_to_off",
      parameters_parse(1, NULL)->show_help == false);
  announce("help_turns_on",
      parameters_parse(2, help_b)->show_help == true);
  announce("filename_gets_passed",
      0 == strcmp(file_str, parameters_parse(2, file_b)->file_name));
}
