#include <stdlib.h>
#include <string.h>

#include "dev_mode.h"
#include "constants.h"
#include "defaults.h"
#include "params.h"
#include "parent_params.h"

Params *parameters_create() {
  Params *p = malloc(sizeof(Params));

  p->parent_params = parent_params_create();

  p->output_dir = malloc(sizeof(char) * MAX_FILE_NAME_LEN);
  strcpy(p->output_dir, DEFAULT_OUTPUT_DIR);

  p->operations_of_each_worker = 1024;
  p->show_help = false;

  return p;
}

void parameters_free(Params *p) {

  WELL("");
  if (p == NULL)
    return;

  if (p->parent_params)
    parent_params_free(p->parent_params);

  if (p->output_dir)
    free(p->output_dir);

  free(p);
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
      p->parent_params->num_of_children = atoi(argv[i]);
    IF_ITS("-c") flag = F_NUM_OF_CHILDREN;
    else
      p->parent_params->file_name = argv[i];
  }

  return p;
}
#undef IF_ITS

