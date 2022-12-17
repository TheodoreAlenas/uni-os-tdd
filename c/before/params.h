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
  unsigned operations_of_each_worker;
  unsigned microsecond_delay;
  char output_dir[MAX_FILE_NAME_LEN];
  ParentParams *parent_params;
  size_t file_size;
} Params;

Params *parameters_parse(Params *p, int argc, char **argv);
void parameters_print(Params *p);
void parameters_help();

#endif
