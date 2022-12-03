#include "test.h"

bool test_empty_stack_is_empty() {
  Stack *s;
  stack_init(s, 0);

  return stack_is_empty(s);
}

bool test_zero_stack_is_full() {
  Stack *s;
  stack_init(s, 0);

  return stack_is_full(s);
}

bool test_stack_empty_and_full() {
  Stack *s1;
  Stack *s2;
  stack_init(s1, 0);
  stack_init(s2, 1);
  if (!stack_is_full(s1)) return false;
  if ( stack_is_full(s2)) return false;
  if (!stack_is_empty(s1)) return false;
  if (!stack_is_empty(s2)) return false;
  return true;
}

bool test_stack_push() {
  Stack *s;
  stack_init(s, 0);
  if (stack_push(s)) return false;
  return true;
}

void test_stack() {

  test_one(test_empty_stack_is_empty, "test_empty_stack_is_empty");
  test_one(test_zero_stack_is_full, "test_zero_stack_is_full");
  test_one(test_stack_empty_and_full, "test_stack_empty_and_full");
  test_one(test_stack_push, "test_stack_push");

}
