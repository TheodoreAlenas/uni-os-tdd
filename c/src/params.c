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

void parameters_print(Params *p) {
  printf("number of children: %d\n", p->parent_params->num_of_children);
  printf("'I want' shared memory name: '%s'\n", p->parent_params->shmem_name_yes_please);
  printf("'thank you' shared memory name: '%s'\n", p->parent_params->shmem_name_youre_ready);
  printf("loops per child: %d\n", p->parent_params->loops_per_child);
}

#define IF_ITS(ARG_FLAG) else if (strcmp(ARG_FLAG, flag) == 0)
#define UPON(ARG_FLAG) else if (strcmp(ARG_FLAG, argv[i]) == 0)

Params *parameters_parse(int argc, char **argv) {
  int i, just_read_flag = 0, nope = 0;
  char *flag = NULL;
  Params *p;
  p = parameters_create();

  for (i = 1; i < argc; i++) {

    if (argv[i][0] == '-') {
      if (flag)
        fprintf(stderr, "expecting argument for flag %s\n", flag);

      if (0) {}
      UPON("--print") p->show_params = true;
      UPON("--help") p->show_help = true;
      else flag = argv[i];

      continue;
    }

    if (0) {}
    IF_ITS("-c") p->parent_params->num_of_children = atoi(argv[i]);
    IF_ITS("--shm-i-want") strcpy(p->parent_params->shmem_name_yes_please, argv[i]);
    IF_ITS("--shm-thank-you") strcpy(p->parent_params->shmem_name_youre_ready, argv[i]);
    IF_ITS("--loops") p->parent_params->loops_per_child = atoi(argv[i]);

    else p->parent_params->file_name = argv[i];

    flag = NULL;
  }

  return p;
}
#undef IF_ITS
#undef UPON

