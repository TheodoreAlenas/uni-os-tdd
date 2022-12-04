#ifndef PARENT_H
#define PARENT_H

#include <stdio.h>

#include "stack.h"
#include "child_data.h"


typedef struct parent {
  Stack *requests;
  ChildData *children;
} Parent;

Parent *parent_create(int num_of_workers);
int parent_loop(Parent *r);
void parent_free(Parent *r);

#endif
