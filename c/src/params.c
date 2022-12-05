#include <stdlib.h>
#include <string.h>

#include "params.h"

Params *parameters_create() {
  Params *p = malloc(sizeof(Params));

  p->file_name = malloc(sizeof(char) * MAX_FILE_NAME_LEN);
  strcpy(p->file_name, "data/1001-line-numbers.dat");

  p->output_dir = malloc(sizeof(char) * MAX_FILE_NAME_LEN);
  strcpy(p->output_dir, "rlr-output");

  p->file_segment_length = 128;
  p->num_of_children = 16;
  p->operations_of_each_worker = 1024;
  p->show_help = false;

  return p;
}

enum FlagType { F_NONE, F_NUM_OF_CHILDREN };

#define IF_ITS(s) else if (strcmp(argv[i], (s)) == 0)

Params *parameters_parse(int argc, char **argv) {
  int i;
  enum FlagType flag = F_NONE;
  Params *p;
  p = parameters_create();

  if (argc == 1)
    return p;

  if (strcmp(argv[1], "--help") == 0)
    p->show_help = true;

  for (i = 1; i < argc; i++) {

    if (0) {}
    else if (flag == F_NUM_OF_CHILDREN)
      p->num_of_children = atoi(argv[i]);
    IF_ITS("-c") flag = F_NUM_OF_CHILDREN;
    else
      p->file_name = argv[i];
  }

  return p;
}
#undef IF_ITS

void parameters_free(Params *p) {
  free(p->file_name);
  free(p);
}
