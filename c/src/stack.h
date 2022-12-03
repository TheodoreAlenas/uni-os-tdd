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
} Stack;


void stack_init(Stack *s);
bool stack_is_empty(Stack *s);


#endif
