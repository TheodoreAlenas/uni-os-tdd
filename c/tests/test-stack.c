#include "../src/stack.h"
#include "test.h"


void test_stack() {

  Stack *s;
  Item **i, *x, *y, *x1, *y1, *x2, *y2;

  /* below here */
  announce("test_negative_stack_has_size_zero", stack_create(-1)->capacity == 0);
  announce("test_zero_stack_is_empty_succeeds", stack_is_empty(stack_create(0)));
  announce("test_zero_stack_is_full_succeeds", stack_is_full(stack_create(0)));
  announce("test_tiny_stack_is_empty_succeeds", stack_is_empty(stack_create(1)));
  announce("test_tiny_stack_is_full_fails", !stack_is_full(stack_create(1)));
  announce("test_zero_stack_push_fails", !stack_push(stack_create(0), NULL));
  announce("test_tiny_stack_push_succeeds", stack_push(stack_create(1), NULL));
  announce("test_zero_stack_pop_fails", !stack_pop(stack_create(0), NULL));

  i = stack_create(3)->items;
  announce("test_big_stack_has_all_items_null",
      i[0] == NULL && i[1] == NULL && i[2] == NULL);

  s = stack_create(1);
  stack_push(s, NULL);
  announce("test_stack_push_pop_succeeds", stack_pop(s, i));

  s = stack_create(1);
  stack_push(s, NULL);
  stack_pop(s, i);
  announce("test_stack_push_pop_pop_fails", !stack_pop(s, i));

  s = stack_create(1);
  stack_push(s, x);
  stack_pop(s, &y);
  announce("test_stack_push_pop_same_item", x == y);

  s = stack_create(2);
  stack_push(s, x1);
  stack_push(s, x2);
  stack_pop(s, &y2);
  stack_pop(s, &y1);
  announce("test_stack_double_push_pop_same_item", x1 == y1 && x2 == y2);
}

