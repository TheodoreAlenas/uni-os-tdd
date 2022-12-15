#ifndef PARENT_H
#define PARENT_H

#include <stdio.h>
#include <semaphore.h>

#include "parent_params.h"
#include "stack.h"


typedef struct parent {
  Stack *requests;
  const ParentParams *pp;
  sem_t *sem_yes_please;
  sem_t **sems_youre_ready;
  void *shmem_yes_please;
  void *shmem_youre_ready;
} Parent;

Parent *parent_create(const ParentParams *pp, char **sem_names);
void parent_free(Parent *r);
int parent_loop(Parent *r);
int parent_waitpid(const Parent *r);

#endif
