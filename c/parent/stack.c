#include <stdio.h>
#include "stack.h"
#include "../both/dev_mode.h"

int max(int a, int b) { if (a < b) return b; return a; }

void nullify_items(Stack *s) {
  int i;
  for (i = 0; i < s->capacity; i++)
    s->items[i] = NULL;
}

Stack *stack_init(Stack *s, int capacity) {
  s->capacity = max(0, capacity);
  s->items = malloc(s->capacity * sizeof(Item));
  s->size = 0;
  nullify_items(s);

  return s;
}

Stack *stack_create(int capacity) {
  Stack *s;
  s = malloc(sizeof(Stack));

  return stack_init(s, capacity);
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

int stack_for_all_of_segment(Stack *s, int (*f) (Item *item, void *args), void *args) {
  Item *item;
  int err;
  unsigned segment;

  if (stack_is_empty(s))
    return 0;

  segment = s->items[s->size - 1]->file_segment;

  while (s->size > 0) {

    if (s->items[s->size - 1]->file_segment != segment)
      break;

    stack_pop(s, &item);

    err = f(item, args);

    if (err)
      return err;
  }

  return 0;
}

bool stack_push(Stack *s, Item *item) {
  if (stack_is_full(s))
    return false;
  stack_bubble(s, item->file_segment);
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

int stack_position(const Stack *s, unsigned file_segment) {
  int i;
  for (i = 0; i < s->size; i++)
    if (s->items[i]->file_segment == file_segment)
      return i;
  return -1;
}

int stack_end_of_bubble(const Stack *s, int index) {
  while (++index < s->size)
    if (s->items[index]->file_segment
        != s->items[index - 1]->file_segment)
      break;
  return index - 1;
}

void bubble(Stack *s, int start, int end);

int stack_bubble(Stack *s, unsigned file_segment) {
  int start, end, dx;

  if (stack_is_full(s))
    return -1;

  start = stack_position(s, file_segment);
  if (start == -1)
    return s->size;
  end = stack_end_of_bubble(s, start);

  if (end < s->size - 1)
    bubble(s, start, end);

  return 0;
}

void bubble(Stack *s, int start, int end) {
  int i, dx;

  s->items[s->size] = s->items[start];  /* making space */

  dx = end - start + 1;  /* bubbling */
  for (i = end + 1; i < s->size; i++) {
    s->items[i - dx] = s->items[i];
    s->items[i] = s->items[i - dx + 1];
  }

  s->items[s->size - 1] = s->items[s->size]; /* putting back */
}

void stack_print_inline(Stack *s) {
  int i;

  printf("stack with size %d/%d (segment, child)", s->size, s->capacity);
  for (i = 0; i < s->size; i++) {
    printf(" > %d,%d", s->items[i]->file_segment, s->items[i]->child);
  }
}

void stack_print(Stack *s) {
  stack_print_inline(s);
  printf("\n");
}
