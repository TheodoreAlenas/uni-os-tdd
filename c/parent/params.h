#ifndef PARENT_PARAMS_H
#define PARENT_PARAMS_H

#include <semaphore.h>

typedef struct parent_params {
  unsigned num_of_children;
  unsigned loops_per_child;
  char *file_name;
  unsigned long file_segment_length;
  char *sem_name_yes_please;
  char *shmem_name_yes_please;
  char *shmem_name_youre_ready;
} ParentParams;

ParentParams *parent_params_create();
void parent_params_free(ParentParams *pp);

#endif
