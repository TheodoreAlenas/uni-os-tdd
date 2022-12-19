#ifndef CHILD_STRUCT_H
#define CHILD_STRUCT_H

#include <semaphore.h>
#include "../both/constants.h"

typedef struct child {
  char *sem_name_i_want;
  char sem_name_thank_you[MAX_FILE_NAME_LEN];
  char *shmem_name_i_want;
  char *shmem_name_thank_you;
  char *output_file;
  unsigned loops;
  unsigned id;
  unsigned num_of_children;
  unsigned file_segment_length;
  unsigned microsecond_delay;

  sem_t *sem_i_want;
  sem_t *sem_thank_you;
  char *shmem_i_want_offset;
  char *shmem_thank_you;
  int lines_in_file;
} Child;

#endif
