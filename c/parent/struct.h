#ifndef PARENT_STRUCT_H
#define PARENT_STRUCT_H

#include <semaphore.h>
#include "stack.h"
#include "params.h"

typedef struct parent {
  const ParentParams *pp;

  unsigned num_of_children;
  unsigned loops_per_child;
  char *input_file;
  unsigned long file_segment_length;

  char *sem_name_yes_please;
  char *shmem_name_yes_please;
  char *shmem_name_youre_ready;

  sem_t *sem_yes_please;
  sem_t **sems_youre_ready;
  char *shmem_yes_please;
  char *shmem_youre_ready;
  int lines_in_file;
} Parent;


#endif
