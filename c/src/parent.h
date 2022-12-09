#ifndef PARENT_H
#define PARENT_H

#include <stdio.h>
#include <semaphore.h>

#include "parent_params.h"
#include "stack.h"
#include "child_data.h"


typedef struct parent {
  Stack *requests;
  ParentParams *pp;
  sem_t *sem_yes_please;
} Parent;

Parent *parent_create(ParentParams *pp);
void parent_free(Parent *r);
int parent_loop(Parent *r);
char *parent_read_file_segment(Parent *parent, unsigned long segment);
int parent_waitpid(Parent *r);

#endif
