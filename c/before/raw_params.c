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

  while (p[i] != 'b') i++;
  pp->takes_value = i;

  pp->value_buffer = ++i;

  while (p[i] != 'e') i++;
  pp->end = i;

  pp->len = pp->end + 2;
}

int find_long_matching(char *p, char *flag, ParamPos *pp) {
  int pos, i, row;

  for (row = pp->len; p[row] != '$'; row += pp->len) {

    for (i = 0; p[row + pp->long_flag + i] != '\0'; i++)
      if (flag[i + 2] != p[i + row + pp->long_flag])
        break;

    if (flag[i + 2] == '=' || flag[i + 2] == '\0')
      break;
  }

  if (p[row] == '$') {
    fprintf(stderr, "Flag of '%s' not found\n", flag);
    return -1;
  }

  if (p[row + pp->takes_value] == '|' && flag[i + 2] != '=') {
    fprintf(stderr, "Expected an '=' and the value, on '%s'\n", flag);
    return -1;
  }

  if (p[row + pp->takes_value] != '|' && flag[i + 2] == '=') {
    fprintf(stderr, "The flag '%s' doesn't take an argument\n", flag);
    return -1;
  }

  if (p[row + pp->takes_value] != '|' && flag[i + 2] != '\0') {
    fprintf(stderr, "Flag of '%s' not found\n", flag);
    return -1;
  }

  return row;
}

int find_short_matching(char *p, char *flag, ParamPos *pp) {
  int i;

  for (i = pp->len; p[i] != '$'; i += pp->len)
    if (0 == strcmp(p + i + pp->short_flag, flag + 1))
      return i;

  return -1;
}

int get_is_long_flag(char *s) { return s[0] == '-' && s[1] == '-'; }
int get_is_short_flag(char *s) { return s[0] == '-' && s[1] != '-'; }

void set_long_flag(char *p, char *s, ParamPos *pp) {
  int pos, eq;
  pos = find_long_matching(p, s, pp);

  if (p[pos + pp->takes_value] != '|') {
    strcpy(p + pos + pp->takes_value, "on");
    return;
  }

  for (eq = 0; s[eq] != '='; eq++) {}
  strcpy(p + pos + pp->value_buffer, s + eq + 1);
}

void set_short_flag(char *p, int argc, char **argv, int *i, ParamPos *pp) {
  int pos;
  pos = find_short_matching(p, argv[*i], pp);

  if (pos < 0) {
    fprintf(stderr, "Invalid short flag '%s', try --help\n", argv[*i]);
    return;
  }

  if (p[pos + pp->takes_value] != '|') {
    strcpy(p + pos + pp->takes_value, "on");
    return;
  }

  (*i)++;
  if (*i >= argc) {
    fprintf(stderr, "Missing argument for flag '%s' (%s)\n",
        argv[*i - 1], p + pos + pp->help);
    return;
  }

  if (strlen(argv[*i]) > MAX_PARAM_LEN) {
    fprintf(stderr, "Argument '%s' too long (max is %d bytes)\n",
        argv[*i], MAX_PARAM_LEN);
    return;
  }

  strcpy(p + pos + pp->value_buffer, argv[*i]);
}

void set_input_file(char *p, char *name, ParamPos *pp) {
  int pos;
  pos = find_short_matching(p, "-i", pp);

  if (strlen(name) > MAX_PARAM_LEN) {
    fprintf(stderr, "Input file name '%s' too long (max is %d bytes)\n",
        name, MAX_PARAM_LEN);
    return;
  }

  strcpy(p + pos + pp->value_buffer, name);
}

void set_output_dir(char *p, char *name, ParamPos *pp) {
  int pos;
  pos = find_short_matching(p, "-o", pp);

  if (strlen(name) > MAX_PARAM_LEN) {
    fprintf(stderr, "Output dir name '%s' too long (max is %d bytes)\n",
        name, MAX_PARAM_LEN);
    return;
  }

  strcpy(p + pos + pp->value_buffer, name);
}

void fill_them(char *p, int argc, char **argv) {
  ParamPos pp;
  int i;

  /* TODO */
  param_pos_init(&pp, p);
  for (i = 1; i < argc; i++)

    if (get_is_long_flag(argv[i]))
      set_long_flag(p, argv[i], &pp);

    else if (get_is_short_flag(argv[i]))
      set_short_flag(p, argc, argv, &i, &pp);

    else
      break;

  if (i < argc)
    set_input_file(p, argv[i++], &pp);

  if (i < argc)
    set_output_dir(p, argv[i++], &pp);
}

void raw_params_parse(Params *p, int argc, char **argv) {
  raw_params_callback(p, argc, argv, fill_them);
}

void raw_params_callback(Params *p, int argc, char **argv,
    void (*callback) (char *params, int argc, char **argv)) {

  /*                                                              123456789 123456789 123456789 123456789 123456789 123456789 1234*/
  char params[] =
    " f\0 F\0                   h\0                              b\0                                                             e\0"
    " h\0 help\0                print this help\0                off\0                                                            \0"
    " p\0 print\0               print defaults, plus overrides\0 off\0                                                            \0"
    " c\0 children\0            number of children\0             |8\0                                                             \0"
    " r\0 loops\0               loops per child\0                |1024\0                                                          \0"
    " i\0 input\0               input file\0                     |../data/1001-line-numbers.dat\0                                 \0"
    " o\0 output\0              output directory\0               |output\0                                                        \0"
    " w\0 shm-i-want\0          'I want' shared memory name\0    |shm_i_want\0                                                    \0"
    " t\0 shm-thank-you\0       'thank you' shared memory name\0 |shm_thank_you\0                                                 \0"
    " l\0 file-segment-length\0 lines in file segment\0          |128\0                                                           \0"
    " m\0 microsecond-delay\0   children's fake delay\0          |20000\0                                                         \0"
    "$"
    ;

  callback(params, argc, argv);
}

void raw_params_help(char *p, ParamPos *pp) {
  int row;

  printf("\n  Useage: ./rlr [options] [input [output]]\n\n");

  for (row = pp->len; p[row] != '$'; row += pp->len) {

    if (p[row + pp->takes_value] == '|')
      printf("-%s VALUE , --%s=VALUE  %10s (%s now)\n",
          p + row + pp->short_flag,
          p + row + pp->long_flag,
          p + row + pp->help,
          p + row + pp->value_buffer
          );

    else
      printf("-%s , --%s   %10s (%s now)\n",
          p + row + pp->short_flag,
          p + row + pp->long_flag,
          p + row + pp->help,
          p + row + pp->takes_value
          );
  }

  printf(
      "\n  Examples:\n"
      "./rlr   # runs with defaults\n"
      "./rlr -c 2\n"
      "./rlr --children=2\n"
      "./rlr --children=2 input-file\n"
      "./rlr --children=2 input-file output-file\n"
      "./rlr --children=2 --help  # displays parameters and exits\n"
      "./rlr -m 0    # fast run\n"
      );
}
