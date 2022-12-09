#include "dev_mode.h"
#include "constants.h"
#include "defaults.h"
#include "parent_params.h"
#include "params.h"

ParentParams *parent_params_create() {
  ParentParams *pp;

  pp = malloc(sizeof(ParentParams));
  pp->file_name = malloc(MAX_FILE_NAME_LEN * sizeof(char));
  strcpy(pp->file_name, DEFAULT_INPUT_FILE);

  pp->file_segment_length = DEFAULT_SEGMENT_LENGTH;
  pp->num_of_children = DEFAULT_NUM_OF_CHILDREN;

  pp->sem_name_yes_please = malloc(MAX_FILE_NAME_LEN);
  strcpy(pp->sem_name_yes_please, DEFAULT_SEM_I_WANT);
  pp->shmem_name_yes_please = malloc(MAX_FILE_NAME_LEN);
  pp->shmem_name_youre_ready = malloc(MAX_FILE_NAME_LEN);

  pp->children = NULL;

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

