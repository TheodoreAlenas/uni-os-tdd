#ifndef CHILD_STRUCT_H
#define CHILD_STRUCT_H

#include <semaphore.h>

typedef struct child_arguments {
  char *sem_name_i_want;
  char *sem_name_thank_you;
  char *shmem_name_i_want;
  char *shmem_name_thank_you;
  char *file_name;
  unsigned loops;
  unsigned id;
  unsigned num_of_children;
  unsigned file_segment_length;
  unsigned microsecond_delay;
} ChildArgs;

typedef struct child {
  sem_t *sem_i_want;
  sem_t *sem_thank_you;
  void *shmem_i_want;
  void *shmem_thank_you;
  const ChildArgs *names;
} Child;

#endif
