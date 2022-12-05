#include "parent_params.h"
#include "params.h"

ParentParams *parent_params_create() {
  ParentParams *pp;

  pp = malloc(sizeof(ParentParams));
  pp->file_name = malloc(MAX_FILE_NAME_LEN * sizeof(char));
  strcpy(pp->file_name, "data/1001-line-numbers.dat");

  pp->file_segment_length = 128;
  pp->num_of_children = 16;

  return pp;
}

void parent_params_free(ParentParams *pp) {
  if (pp == NULL)
    return;

  if (pp->file_name)
    free(pp->file_name);

  free(pp);
}

