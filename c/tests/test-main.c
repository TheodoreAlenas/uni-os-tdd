#include <stdio.h>

#include "test.h"

int main() {

  test_stack();
  test_arg_parse();

  printf("%d/%d passed, %d failed\n",
      get_total_tests_passed(),
      get_total_tests(),
      get_total_tests_failed());

  return get_total_tests_failed();
}

