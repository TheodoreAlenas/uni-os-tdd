#ifndef PARENT_LOOP_H
#define PARENT_LOOP_H

#include <semaphore.h>
#include "msg_cycler.h"
#include "struct.h"

typedef struct {
  sem_t *sem_yes_please;
  sem_t **sems_youre_ready;
  void *shmem_yes_please;
  void *shmem_youre_ready;
  int num_of_children;
} ParentLoopArgs;


int parent_loop(Parent *r);

#endif
