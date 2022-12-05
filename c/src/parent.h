#ifndef PARENT_H
#define PARENT_H

#include <stdio.h>

#include "parent_params.h"
#include "stack.h"
#include "child_data.h"


typedef struct parent {
  Stack *requests;
  ChildData *children;
  ParentParams *pp;
} Parent;

Parent *parent_create(ParentParams *pp);
void parent_free(Parent *r);
int parent_loop(Parent *r);
char *parent_read_file_segment(Parent *parent, unsigned long segment);

#endif
