#include "test.h"

/*
1G/END OF MACROSn/boolma
0W"tyeG?void test_stack() {}Otest_one(t, "t");'a/boolma
G?void test_stack() {o}jdap

@a15@b@c

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

bool test_stack_push_pop_succeeds() {
  Stack *s = stack_create(1);
  Item *r;
  stack_push(s, NULL);
  return stack_pop(s, &r);
}

bool test_stack_push_pop_pop_fails() {
  Stack *s = stack_create(1);
  Item *r;
  stack_push(s, NULL);
  stack_pop(s, &r);
  return !stack_pop(s, &r);
}

bool test_stack_push_pop_same_item() {
  Stack *s = stack_create(1);
  Item *x;
  Item *y;
  stack_push(s, x);
  stack_pop(s, &y);
  return x == y;
}

bool test_stack_double_push_pop_same_item() {
  Stack *s = stack_create(2);
  Item *x1, *x2, *y1, *y2;
  stack_push(s, x1);
  stack_push(s, x2);
  stack_pop(s, &y2);
  stack_pop(s, &y1);
  return x1 == y1 && x2 == y2;
}

void test_stack() {

  test_one(test_zero_stack_is_empty_succeeds, "test_zero_stack_is_empty_succeeds");
  test_one(test_zero_stack_is_full_succeeds, "test_zero_stack_is_full_succeeds");
  test_one(test_tiny_stack_is_empty_succeeds, "test_tiny_stack_is_empty_succeeds");
  test_one(test_tiny_stack_is_full_fails, "test_tiny_stack_is_full_fails");
  test_one(test_zero_stack_push_fails, "test_zero_stack_push_fails");
  test_one(test_tiny_stack_push_succeeds, "test_tiny_stack_push_succeeds");
  test_one(test_stack_push_pop_succeeds, "test_stack_push_pop_succeeds");
  test_one(test_stack_push_pop_pop_fails, "test_stack_push_pop_pop_fails");
  test_one(test_stack_push_pop_same_item, "test_stack_push_pop_same_item");
  test_one(test_stack_double_push_pop_same_item, "test_stack_double_push_pop_same_item");

}
