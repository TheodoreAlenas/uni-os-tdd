#include <stdio.h>

#include "stack.h"

void test_one(bool (*f) (void), char *name);

int main() {

  printf("%d\n", i);
  printf("%d\n", t());
  test_stack_is_empty();
  //test_one(test_stack_is_empty, "stack is empty");

  return 0;
}

void test_one(bool (*f) (void), char *name) {
  bool passed;

  printf("Testing function '%s' ... ", name);
  passed = f();

  if (passed) {
    printf("Passed");
  }
  else {
    printf("Failed");
  }

}
