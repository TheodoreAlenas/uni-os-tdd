#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct item {
  int file_segment;
  int line;
} Item;

typedef struct stack {
  Item *items;
  int size;
  int capacity;
} Stack;


void stack_init(Stack *s, int capacity);
bool stack_is_empty(Stack *s);
bool stack_is_full(Stack *s);
bool stack_push(Stack *s);
bool stack_pop(Stack *s);


#endif
