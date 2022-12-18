#include <stdio.h>
#include <string.h>

#include "../both/dev_mode.h"
#include "../both/constants.h"
#include "defaults.h"
#include "params.h"
#include "../parent/params.h"

Params *parameters_init(Params *p) {
  p->parent_params = parent_params_create();
  strcpy(p->output_dir, DEFAULT_OUTPUT_DIR);
  p->loops_per_child = 1024;
  p->show_help = false;
  p->microsecond_delay = 20000;

  return p;
}

void parameters_print(Params *p) {
  printf("number of children: %d\n", p->parent_params->num_of_children);
  printf("'I want' shared memory name: '%s'\n", p->parent_params->shmem_name_yes_please);
  printf("'thank you' shared memory name: '%s'\n", p->parent_params->shmem_name_youre_ready);
  printf("loops per child: %d\n", p->parent_params->loops_per_child);
  printf("output directory: '%s'\n", p->output_dir);
  printf("input file: '%s'\n", p->parent_params->file_name);
  printf("lines in file segment: %lu\n", p->parent_params->file_segment_length);
  printf("child's delay in microseconds: %u\n", p->microsecond_delay);
}

void parameters_help() {
  /* TODO ouch oh my God */
  printf(
  " Useage: ./rlr [options] [in-file]                             \n"
  " where options may be:                                         \n"
  "                                                               \n"
  " --help          print this help                               \n"
  " --print         print defaults, plus overrides (see example)  \n"
  " -c  --children              number of children                \n"
  " -r  --loops                 loops per child                   \n"
  " -i  --input                 input file                        \n"
  " -o  --output                output directory                  \n"
  " -w  --shm-i-want            'I want' shared memory name       \n"
  " -t  --shm-thank-you         'thank you' shared memory name    \n"
  " -l  --file-segment-length   lines in file segment             \n"
  " -m  --microsecond-delay     children's fake delay             \n"
  "                                                               \n"
  " Example:   ./rlr -c 2 --output /tmp                           \n"
  " Example:   ./rlr input-file.txt                               \n"
  " Example:   ./rlr -l 100 input-file.txt                        \n"
  " Example:   ./rlr --microsecond-delay 0                        \n"
  " Example:   ./rlr --print        # prints defaults             \n"
  " Example:   ./rlr -c 2 --print   # almost prints defaults      \n"
  ) ;
}

#define IF_ITS(SHORT_FLAG, LONG_FLAG) else if (strcmp(SHORT_FLAG, flag) == 0 || strcmp(LONG_FLAG, flag) == 0)
#define UPON(ARG_FLAG) else if (strcmp(ARG_FLAG, argv[i]) == 0)

Params *parameters_parse(Params *p, int argc, char **argv) {
  int i, just_read_flag = 0, nope = 0;
  char *flag = NULL;

  parameters_init(p);

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
    else if (flag == NULL && argv[i][0] != '-') strcpy(p->parent_params->file_name, argv[i]);
    else if (flag == NULL && argv[i][0] == '-') fprintf(stderr, "bad flag %s\n", argv[i]);
    IF_ITS("-c", "--children") p->parent_params->num_of_children = atoi(argv[i]);
    IF_ITS("-w", "--shm-i-want") strcpy(p->parent_params->shmem_name_yes_please, argv[i]);
    IF_ITS("-t", "--shm-thank-you") strcpy(p->parent_params->shmem_name_youre_ready, argv[i]);
    IF_ITS("-r", "--loops") p->parent_params->loops_per_child = atoi(argv[i]);
    IF_ITS("-i", "--input") strcpy(p->parent_params->file_name, argv[i]);
    IF_ITS("-o", "--output") strcpy(p->output_dir, argv[i]);
    IF_ITS("-l", "--file-segment-length") p->parent_params->file_segment_length = atoi(argv[i]);
    IF_ITS("-m", "--microsecond-delay") p->microsecond_delay = atoi(argv[i]);

    flag = NULL;
  }

  return p;
}
#undef IF_ITS
#undef UPON

