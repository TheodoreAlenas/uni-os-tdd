#ifndef PARENT_STRUCT_H
#define PARENT_STRUCT_H

#include <semaphore.h>
#include "stack.h"
#include "params.h"

typedef struct parent {
  const ParentParams *pp;
  sem_t *sem_yes_please;
  sem_t **sems_youre_ready;
  char *shmem_yes_please;
  char *shmem_youre_ready;
  int lines_in_file;
} Parent;


#endif
