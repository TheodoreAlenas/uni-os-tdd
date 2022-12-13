#include "../src/stack.h"
#include "test.h"


void test_stack() {

  Stack *s;
  Item **i, *x, *y, *x1, *y1, *x2, *y2, *x3, *y3;

  x = malloc(sizeof(Item));
  x1 = malloc(sizeof(Item));
  x2 = malloc(sizeof(Item));
  x3 = malloc(sizeof(Item));

  /* below here */
  announce("test_negative_stack_has_size_zero", stack_create(-1)->capacity == 0);
  announce("test_zero_stack_is_empty_succeeds", stack_is_empty(stack_create(0)));
  announce("test_zero_stack_is_full_succeeds", stack_is_full(stack_create(0)));
  announce("test_tiny_stack_is_empty_succeeds", stack_is_empty(stack_create(1)));
  announce("test_tiny_stack_is_full_fails", !stack_is_full(stack_create(1)));
  announce("test_zero_stack_push_fails", !stack_push(stack_create(0), x));
  announce("test_tiny_stack_push_succeeds", stack_push(stack_create(1), x));
  announce("test_zero_stack_pop_fails", !stack_pop(stack_create(0), i));

  i = stack_create(3)->items;
  announce("test_big_stack_has_all_items_null",
      i[0] == NULL && i[1] == NULL && i[2] == NULL);

  s = stack_create(1);
  stack_push(s, x);
  announce("test_stack_push_pop_succeeds", stack_pop(s, i));

  s = stack_create(1);
  stack_push(s, x);
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

  x1->file_segment = 2;
  x2->file_segment = 1;
  x3->file_segment = 1;
  s = stack_create(3);
  stack_push(s, x1);
  announce("test_stack_double_push_pop_same_item", x1 == y1 && x2 == y2);
  stack_push(s, x2);
  announce("test_stack_double_push_pop_same_item", x1 == y1 && x2 == y2);
  stack_push(s, x3);
  announce("test_stack_double_push_pop_same_item", x1 == y1 && x2 == y2);
  announce("test_stack_position_2_1_1", stack_position(s, 1) == 1);
  announce("test_stack_position_2_1_1", stack_position(s, 2) == 0);
  announce("test_stack_position_2_1_1", stack_position(s, 3) == -1);
  announce("test_stack_bubble_end_2_1_1", stack_end_of_bubble(s, 1) == 2);
  announce("test_stack_bubble_end_2_1_1", stack_end_of_bubble(s, 0) == 0);

  stack_pop(s, &y3);
  stack_pop(s, &y2);
  stack_pop(s, &y1);
  announce("test_stack_1_2_1_is_2_1_1", y1->file_segment == 2);
}

