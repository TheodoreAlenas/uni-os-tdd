#ifndef PARAMS_H
#define PARAMS_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "parent_params.h"

#define MAX_FILE_NAME_LEN 256
#define MAX_REQUEST_LEN 256

typedef struct parameters {
  bool show_help;
  unsigned operations_of_each_worker;
  char *output_dir;
  ParentParams *parent_params;
} Params;

Params *parameters_parse(int argc, char **argv);
void parameters_free(Params *p);


#endif
