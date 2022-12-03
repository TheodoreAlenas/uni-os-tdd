#include "stack.h"

void stack_init(Stack *s, int capacity) {
  s->capacity = capacity;
  if (capacity < 0)
    s->capacity = 0;
  s->items = NULL;
  s->size = 0;
}

bool stack_is_empty(Stack *s) {
  return s->size <= 0;
}

bool stack_is_full(Stack *s) {
  return s->size >= s->capacity;
}

bool stack_push(Stack *s) {
  return false;
}

bool stack_pop(Stack *s) {
  return true;
}

