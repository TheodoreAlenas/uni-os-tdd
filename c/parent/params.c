#include <unistd.h>
#include <stdio.h>
#include "../both/dev_mode.h"
#include "../both/constants.h"
#include "../before/defaults.h"
#include "params.h"
#include "../before/params.h"

ParentParams *parent_params_create() {
  ParentParams *pp;
  char pid[256];

  sprintf(pid, "%d", getpid());

  pp = malloc(sizeof(ParentParams));
  pp->file_name = malloc(MAX_FILE_NAME_LEN * sizeof(char));
  strcpy(pp->file_name, DEFAULT_INPUT_FILE);

  pp->file_segment_length = DEFAULT_SEGMENT_LENGTH;
  pp->num_of_children = DEFAULT_NUM_OF_CHILDREN;
  pp->loops_per_child = DEFAULT_OPERATIONS_PER_CHILD;

  pp->sem_name_yes_please = malloc(MAX_FILE_NAME_LEN);
  strcpy(pp->sem_name_yes_please, DEFAULT_SEM_I_WANT);
  strcat(pp->sem_name_yes_please, pid);

  pp->shmem_name_yes_please = malloc(MAX_FILE_NAME_LEN);
  strcpy(pp->shmem_name_yes_please, DEFAULT_SHM_I_WANT);
  strcat(pp->shmem_name_yes_please, pid);

  pp->shmem_name_youre_ready = malloc(MAX_FILE_NAME_LEN);
  strcpy(pp->shmem_name_youre_ready, DEFAULT_SHM_THANK_YOU);
  strcat(pp->shmem_name_youre_ready, pid);

  return pp;
}

#define CONSIDER_FREEING(n) if (pp->n) free(pp->n)
void parent_params_free(ParentParams *pp) {
  int i;

  if (pp == NULL)
    return;

  CONSIDER_FREEING(file_name);
  CONSIDER_FREEING(sem_name_yes_please);
  CONSIDER_FREEING(shmem_name_yes_please);
  CONSIDER_FREEING(shmem_name_youre_ready);

  free(pp);

  WELL("done");
}
#undef CONSIDER_FREEING

