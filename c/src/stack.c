#include "stack.h"

Stack *stack_create(int capacity) {
  Stack *s;
  s = malloc(sizeof(Stack));

  s->capacity = capacity;
  if (capacity < 0)
    s->capacity = 0;
  s->items = NULL;
  s->size = 0;

  return s;
}

bool stack_is_empty(Stack *s) {
  return s->size <= 0;
}

bool stack_is_full(Stack *s) {
  return s->size >= s->capacity;
}

bool stack_push(Stack *s, Item *item) {
  if (stack_is_full(s))
    return false;
  return true;
}

bool stack_pop(Stack *s, Item *item) {
  if (stack_is_empty(s))
    return false;
  return true;
}

