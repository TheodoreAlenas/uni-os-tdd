#ifndef PARENT_LOOP_EXPOSED_H
#define PARENT_LOOP_EXPOSED_H

#include "parent_loop.h"

typedef struct {
  sem_t *sem_yes_please;
  sem_t **sems_youre_ready;
  void *shmem_youre_ready;
  Stack *requests;
} ParentLoopParams;

typedef struct {
  ParentLoopParams *r; int readers; int current_segment; int child;
  Parent *parent;  /* limited use */
} LoopState;

void single_loop(LoopState *s, MsgCycler *msg_cycler);
int _read_file_segment(Parent *parent, void *shm, int segment);
int _sem_wait(sem_t *s);
int _sem_post(sem_t **sems, int child);

#endif
