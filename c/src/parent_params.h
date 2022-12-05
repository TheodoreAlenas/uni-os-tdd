#ifndef PARENT_PARAMS_H
#define PARENT_PARAMS_H

typedef struct parent_params {
  unsigned num_of_children;
  char *file_name;
  unsigned long file_segment_length;
} ParentParams;

ParentParams *parent_params_create();
void parent_params_free(ParentParams *pp);

#endif
