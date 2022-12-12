#include <stdio.h>
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

void print_all(Params *p) {
  printf("num_of_children %d\n", p->parent_params->num_of_children);
}

enum FlagType { F_NONE, F_NUM_OF_CHILDREN, F_SHMEM_I_WANT, F_SHMEM_THANK_YOU };

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

    else if (flag == F_SHMEM_I_WANT)
      strcpy(p->parent_params->shmem_name_yes_please, argv[i]);

    else if (flag == F_SHMEM_THANK_YOU)
      strcpy(p->parent_params->shmem_name_youre_ready, argv[i]);

    IF_ITS("-c") flag = F_NUM_OF_CHILDREN;
    IF_ITS("--shm-i-want") flag = F_SHMEM_I_WANT;
    IF_ITS("--shm-thank-you") flag = F_SHMEM_THANK_YOU;

    IF_ITS("--print") { print_all(p); p->quit = true; }
    else
      p->parent_params->file_name = argv[i];
  }

  return p;
}
#undef IF_ITS

