#ifndef PARENT_H
#define PARENT_H

#include <stdio.h>

#include "stack.h"
#include "child_data.h"


typedef struct parent {
  Stack *requests;
  ChildData *children;
  unsigned num_of_children;
  char *file_name;
  unsigned long file_segment_length;
} Parent;

Parent *parent_create(int num_of_children,
    char *file_name,
    unsigned long file_segment_length);
void parent_free(Parent *r);
int parent_loop(Parent *r);
char *parent_read_file_segment(Parent *parent, unsigned long segment);

#endif
