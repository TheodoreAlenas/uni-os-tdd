#include "stack.h"

int max(int a, int b) { if (a < b) return b; return a; }

void nullify_items(Stack *s) {
  int i;
  for (i = 0; i < s->capacity; i++)
    s->items[i] = NULL;
}

Stack *stack_create(int capacity) {
  Stack *s;
  s = malloc(sizeof(Stack));

  s->capacity = max(0, capacity);
  s->items = malloc(s->capacity * sizeof(Item));
  s->size = 0;
  nullify_items(s);

  return s;
}

void stack_free(Stack *s) {
  int i;
  for (i = 0; i < s->capacity; i++)
    if (s->items[i] != NULL)
      free(s->items[i]);
  free(s);
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
  s->items[s->size] = item;
  s->size++;
  return true;
}

bool stack_pop(Stack *s, Item **item) {
  if (stack_is_empty(s))
    return false;
  s->size--;
  *item = s->items[s->size];
  return true;
}

