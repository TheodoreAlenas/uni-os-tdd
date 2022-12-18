#ifndef PARAMS_H
#define PARAMS_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "../both/constants.h"
#include "../parent/params.h"

typedef struct parameters {
  bool show_help;
  bool show_params;
  unsigned num_of_children;
  unsigned loops_per_child;
  unsigned microsecond_delay;
  unsigned file_segment_length;
  char output_dir[MAX_FILE_NAME_LEN];
  char input_file[MAX_FILE_NAME_LEN];
  ParentParams *parent_params;
  char *sem_name_i_want;
  char *shmem_name_i_want;
  char *shmem_name_thank_you;
} Params;

Params *parameters_parse(Params *p, int argc, char **argv);
void parameters_print(Params *p);
void parameters_help();

#endif
