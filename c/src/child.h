#ifndef CHILD_H
#define CHILD_H

#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct child_arguments {
  char *sem_name_i_want;
  char *sem_name_thank_you;
  char *shmem_name_i_want;
  char *shmem_name_thank_you;
  char *file_name;
  unsigned loops;
  unsigned id;
  unsigned file_segment_length;
} ChildArgs;

typedef struct child {
  sem_t *sem_i_want;
  sem_t *sem_thank_you;
  void *shmem_i_want;
  void *shmem_thank_you;
  const ChildArgs *names;
} Child;

Child *child_create(const ChildArgs *args);
void child_free(Child *child);
void child_loop(const Child *child);

#endif
