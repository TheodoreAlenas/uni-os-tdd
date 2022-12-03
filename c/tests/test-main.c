#include <stdio.h>

#include "test-stack.h"

void test_one(bool (*f) (void), char *name);

int main() {

  test_one(test_stack_is_empty, "stack is empty");

  return 0;
}

void test_one(bool (*f) (void), char *name) {
  bool passed;
  char *red   = "\033[0;31m";
  char *green = "\033[0;32m";
  char *no_color = "\033[0m";

  printf("%sTesting function '%s' ... %s", green, name, no_color);
  passed = f();

  if (passed) {
    printf("%sPassed%s\n", green, no_color);
  }
  else {
    printf("%sFailed%s\n", red, no_color);
  }

}
