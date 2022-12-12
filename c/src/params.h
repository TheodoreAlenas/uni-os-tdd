#ifndef PARAMS_H
#define PARAMS_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "parent_params.h"

typedef struct parameters {
  bool show_help;
  bool show_params;
  unsigned operations_of_each_worker;
  char *output_dir;
  ParentParams *parent_params;
} Params;

Params *parameters_parse(int argc, char **argv);
void parameters_free(Params *p);
void parameters_print(Params *p);
void parameters_help();

#endif
