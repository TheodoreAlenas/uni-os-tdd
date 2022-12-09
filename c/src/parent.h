#ifndef PARENT_H
#define PARENT_H

#include <stdio.h>
#include <semaphore.h>

#include "parent_params.h"
#include "stack.h"
#include "child_data.h"


typedef struct parent {
  Stack *requests;
  const ParentParams *pp;
  sem_t *sem_yes_please;
  void *shmem_yes_please;
  void *shmem_youre_ready;
} Parent;

Parent *parent_create(const ParentParams *pp);
void parent_free(Parent *r);
int parent_loop(Parent *r);
char *parent_read_file_segment(const Parent *parent, unsigned long segment);
int parent_waitpid(const Parent *r);

#endif
