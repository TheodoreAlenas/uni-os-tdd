#ifndef PARENT_LOOP_H
#define PARENT_LOOP_H

#include <semaphore.h>
#include "msg_cycler.h"
#include "parent_struct.h"

typedef struct {
  sem_t *sem_yes_please;
  sem_t **sems_youre_ready;
  void *shmem_yes_please;
  void *shmem_youre_ready;
  int num_of_children;
} ParentLoopArgs;

int parent_loop_backend(Parent *r);

int _read_file_segment(Parent *parent, void *shm, int segment);
int _sem_wait(sem_t *s);
int _sem_post(sem_t **sems, int child);

#endif
