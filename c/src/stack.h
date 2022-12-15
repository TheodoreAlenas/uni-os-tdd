#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct item {
  unsigned file_segment;
  unsigned child;
} Item;

typedef struct stack {
  Item **items;
  int size;
  int capacity;
} Stack;


Stack *stack_create(int capacity);
void stack_free(Stack *s);
bool stack_is_empty(Stack *s);
bool stack_is_full(Stack *s);
int stack_for_all_of_segment(Stack *s, int (*f) (Item *item, void *args), void *args);
bool stack_push(Stack *s, Item *item);
bool stack_pop(Stack *s, Item **item);
void stack_print_inline(Stack *s);
void stack_print(Stack *s);

int stack_position(const Stack *s, unsigned file_segment);
int stack_end_of_bubble(const Stack *s, int index);
int stack_bubble(Stack *s, unsigned file_segment);

#endif
