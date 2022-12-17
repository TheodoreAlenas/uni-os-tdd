#include "../parent/stack.h"
#include "test.h"

int callback_0(Item *item, void *args);
int callback_3(Item *item, void *args);

void test_stack() {

  Stack *s;
  Item **i, *x, *y, *x1, *y1, *x2, *y2, *x3, *y3, xs[32], *ys[32];

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

  x1->file_segment = 1;
  x2->file_segment = 2;
  x3->file_segment = 1;
  s = stack_create(3);
  stack_push(s, x1);
  stack_push(s, x2);
  stack_push(s, x3);
  announce("test_stack_position_2_1_1", stack_position(s, 1) == 1);
  announce("test_stack_position_2_1_1", stack_position(s, 2) == 0);
  announce("test_stack_position_2_1_1", stack_position(s, 3) == -1);
  announce("test_stack_bubble_end_2_1_1", stack_end_of_bubble(s, 1) == 2);
  announce("test_stack_bubble_end_2_1_1", stack_end_of_bubble(s, 0) == 0);

  stack_pop(s, &y3);
  stack_pop(s, &y2);
  stack_pop(s, &y1);
  announce("test_stack_1_2_1_is_2_1_1",
      y1->file_segment == 2 &&
      y2->file_segment == 1 &&
      y3->file_segment == 1
      );

  announce("test_stack_xs_remain",
      x1->file_segment == 1 &&
      x2->file_segment == 2 &&
      x3->file_segment == 1);

  announce("test_stack_for_each_on_empty", 0 == stack_for_all_of_segment(s, callback_3, x1));
  stack_push(s, x1);
  stack_push(s, x2);
  stack_push(s, x3);

  announce("test_stack_for_each_0", 0 == stack_for_all_of_segment(s, callback_0, x1));
  stack_push(s, x1);
  stack_push(s, x2);
  stack_push(s, x3);
  announce("test_stack_for_each_3", 3 == stack_for_all_of_segment(s, callback_3, x1));

  stack_pop(s, &y);
  stack_pop(s, &y);
  stack_pop(s, &y);
  announce("test_stack_empty", stack_is_empty(s));

  xs[0].file_segment = 1; xs[0].child = 2;
  xs[1].file_segment = 1; xs[1].child = 5;
  xs[2].file_segment = 2; xs[2].child = 6;
  xs[3].file_segment = 2; xs[3].child = 0;

  xs[4].file_segment = 2; xs[4].child = 3;

  s = stack_create(5);

  stack_push(s, xs + 0);
  stack_push(s, xs + 1);
  stack_push(s, xs + 2);
  stack_push(s, xs + 3);
  stack_push(s, xs + 4);

  stack_pop(s, ys + 4);
  announce("test_stack_pain_ys4", ys[4]->child == 3);
  stack_pop(s, ys + 3);
  announce("test_stack_pain_ys3", ys[3]->child == 0);
  stack_pop(s, ys + 2);
  announce("test_stack_pain_ys2", ys[2]->child == 6);
  stack_pop(s, ys + 1);
  announce("test_stack_pain_ys1", ys[1]->child == 5);
  stack_pop(s, ys + 0);
  announce("test_stack_pain_ys0", ys[0]->child == 2);
}


int callback_0(Item *item, void *args) {
  return 0;
}

int callback_3(Item *item, void *args) {
  if (item->file_segment > 2 || item->file_segment < 1)
    return 0;
  if (
      ((Item *) args)->file_segment > 2 ||
      ((Item *) args)->file_segment < 1)
    return 0;
  return 3;
}

