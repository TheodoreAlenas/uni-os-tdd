#ifndef PARENT_STRUCT_H
#define PARENT_STRUCT_H

#include <semaphore.h>
#include "stack.h"
#include "params.h"

typedef struct parent {
  Stack *requests;
  const ParentParams *pp;
  sem_t *sem_yes_please;
  sem_t **sems_youre_ready;
  void *shmem_yes_please;
  void *shmem_youre_ready;
} Parent;


#endif
