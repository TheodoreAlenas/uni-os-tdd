#include "test.h"

/*
0W"tye/test_stack}Otest_one(t, "t");
1G/boolma
'a/boolma
1G/voido}jdap


1G/END OF MACROSn/boolma
0W"tye/test_stack}Otest_one(t, "t");'a/boolma
G?void test_stack() {o}jdap

@a30@b@c

END OF MACROS
*/

bool test_zero_stack_is_empty_succeeds() {
  return stack_is_empty(stack_create(0));
}

bool test_zero_stack_is_full_succeeds() {
  return stack_is_full(stack_create(0));
}

bool test_tiny_stack_is_empty_succeeds() {
  return stack_is_empty(stack_create(1));
}

bool test_tiny_stack_is_full_fails() {
  return !stack_is_full(stack_create(1));
}

bool test_zero_stack_push_fails() {
  return !stack_push(stack_create(0), NULL);
}

bool test_tiny_stack_push_succeeds() {
  return stack_push(stack_create(1), NULL);
}

void test_stack() {

  test_one(test_zero_stack_is_empty_succeeds, "test_zero_stack_is_empty_succeeds");
  test_one(test_zero_stack_is_full_succeeds, "test_zero_stack_is_full_succeeds");
  test_one(test_tiny_stack_is_empty_succeeds, "test_tiny_stack_is_empty_succeeds");
  test_one(test_tiny_stack_is_full_fails, "test_tiny_stack_is_full_fails");
  test_one(test_zero_stack_push_fails, "test_zero_stack_push_fails");
  test_one(test_tiny_stack_push_succeeds, "test_tiny_stack_push_succeeds");

}
