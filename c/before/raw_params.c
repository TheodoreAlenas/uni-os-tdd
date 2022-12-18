#include <stdio.h>
#include "raw_params.h"
#include "raw_params_exposed.h"

void dont_run(Params *p) {
  printf("number of children: %d\n", p->parent_params->num_of_children);
  printf("'I want' shared memory name: '%s'\n", p->parent_params->shmem_name_yes_please);
  printf("'thank you' shared memory name: '%s'\n", p->parent_params->shmem_name_youre_ready);
  printf("loops per child: %d\n", p->parent_params->loops_per_child);
  printf("output directory: '%s'\n", p->output_dir);
  printf("input file: '%s'\n", p->parent_params->file_name);
  printf("lines in file segment: %lu\n", p->parent_params->file_segment_length);
  printf("child's delay in microseconds: %u\n", p->microsecond_delay);
}

void param_pos_init(ParamPos *pp, char *p) {
  int i = 0;

  while (p[i] != 'f') i++;
  pp->short_flag = i;

  while (p[i] != 'F') i++;
  pp->long_flag = i;

  while (p[i] != 'h') i++;
  pp->help = i;

  while (p[i] != '0') i++;
  pp->value_buffer = i;

  while (p[i] != '\0') i++;
  pp->end = i;
}

void fill_them(char *p, int argc, char **argv) {
  ParamPos pp;

  param_pos_init(&pp, p);
}

void raw_params_parse(Params *p, int argc, char **argv) {
  raw_params_callback(p, argc, argv, fill_them);
}

void raw_params_callback(Params *p, int argc, char **argv,
    void (*callback) (char *params, int argc, char **argv)) {

#define VAL_BUF "0123456789" "0123456789" "0123456789" \
  "0123456789" "0123456789" "0123456789" "012\0"

  char params[] =
    " f\0 F\0                   h\0                             " VAL_BUF
    " h\0 help\0                print this help\0               " VAL_BUF
    " h\0 help\0                print this help\0               " VAL_BUF
    " p\0 print\0               print defaults, plus overrides\0" VAL_BUF
    " c\0 children\0            number of children\0            " VAL_BUF
    " r\0 loops\0               loops per child\0               " VAL_BUF
    " i\0 input\0               input file\0                    " VAL_BUF
    " o\0 output\0              output directory\0              " VAL_BUF
    " w\0 shm-i-want\0          'I want' shared memory name\0   " VAL_BUF
    " t\0 shm-thank-you\0       'thank you' shared memory name\0" VAL_BUF
    " l\0 file-segment-length\0 lines in file segment\0         " VAL_BUF
    " m\0 microsecond-delay\0   children's fake delay\0         " VAL_BUF
    "$"
    ;

#undef VAL_BUF

  callback(params, argc, argv);

}
