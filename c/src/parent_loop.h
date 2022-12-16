#ifndef PARENT_LOOP_H
#define PARENT_LOOP_H

#include <semaphore.h>
#include "msg_cycler.h"

typedef struct {
  sem_t *sem_yes_please;
  sem_t *sem_youre_ready;
  void *shmem_youre_ready;
} ParentLoopArgs;

#include "parent_struct.h"
int parent_loop_backend(Parent *r);

#endif
