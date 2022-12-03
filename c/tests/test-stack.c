#include "test-stack.h"

bool test_stack_is_empty() {
  Stack *s;
  stack_init(s);

  return stack_is_empty(s);
}

/*
int stack_order() {
  Item i1 = 0;
  Item i2 = 1;

  Stack *stack;
  stack_init(stack);

  stack_push(i1, stack);
  stack_push(i2, stack);

  i2 = stack_pop(stack);
  i1 = stack_pop(stack);

  return i1 == 0 && i2 == 1;
}
*/
