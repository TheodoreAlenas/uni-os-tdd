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

  return pp;
}

void parent_params_free(ParentParams *pp) {
  if (pp == NULL)
    return;

  if (pp->file_name)
    free(pp->file_name);

  free(pp);
}

