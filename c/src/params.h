#ifndef PARAMS_H
#define PARAMS_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILE_NAME_LEN 1024

typedef struct parameters {
  bool show_help;
  unsigned file_segment_length;
  unsigned num_of_workers;
  unsigned operations_of_each_worker;
  char *file_name;
} Params;

Params *parameters_parse(int argc, char **argv);
void parameters_free(Params *p);


#endif
