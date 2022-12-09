#ifndef CHILD_H
#define CHILD_H

#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct child_arguments {
  const char *sem_name_i_want;
  const char *sem_name_thank_you;
  const char *shmem_name_i_want;
  const char *shmem_name_thank_you;
  const char *file_name;
} ChildArgs;

typedef struct child {
  sem_t *sem_i_want;
  sem_t *sem_thank_you;
  void *shmem_i_want;
  void *shmem_thank_you;
  const ChildArgs *names;
} Child;

Child *child_create(const ChildArgs args);
void child_free(Child *child);
int child_loop(const Child *child);

#endif
