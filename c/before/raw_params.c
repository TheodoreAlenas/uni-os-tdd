#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "raw_params.h"
#include "raw_params_exposed.h"
#include "../both/dev_mode.h"

void param_pos_init(ParamPos *p) {
  int i = 0;

  while (p->p[i] != 'f') i++;
  p->short_flag = i;

  while (p->p[i] != 'F') i++;
  p->long_flag = i;

  while (p->p[i] != 'h') i++;
  p->help = i;

  while (p->p[i] != 'b') i++;
  p->value_type = i;

  p->takes_value = ++i;

  p->value_buffer = ++i;

  while (p->p[i] != 'e') i++;
  p->end = i;

  p->len = p->end + 2;
}

int find_long_matching(ParamPos *p, char *flag) {
  int pos, i, row;

  for (row = p->len; p->p[row] != '$'; row += p->len) {

    for (i = 0; p->p[row + p->long_flag + i] != '\0'; i++)
      if (flag[i + 2] != p->p[i + row + p->long_flag])
        break;

    if (flag[i + 2] == '=' || flag[i + 2] == '\0')
      break;
  }

  if (p->p[row] == '$') {
    fprintf(stderr, "Flag of '%s' not found\n", flag);
    return -1;
  }

  if (p->p[row + p->takes_value] == '|' && flag[i + 2] != '=') {
    fprintf(stderr, "Expected an '=' and the value, on '%s'\n", flag);
    return -1;
  }

  if (p->p[row + p->takes_value] != '|' && flag[i + 2] == '=') {
    fprintf(stderr, "The flag '%s' doesn't take an argument\n", flag);
    return -1;
  }

  if (p->p[row + p->takes_value] != '|' && flag[i + 2] != '\0') {
    fprintf(stderr, "Flag of '%s' not found\n", flag);
    return -1;
  }

  return row;
}

int find_short_matching(ParamPos *p, char *flag) {
  int i;

  for (i = p->len; p->p[i] != '$'; i += p->len)
    if (0 == strcmp(p->p + i + p->short_flag, flag + 1))
      return i;

  return -1;
}

int get_is_long_flag(char *s) { return s[0] == '-' && s[1] == '-'; }
int get_is_short_flag(char *s) { return s[0] == '-' && s[1] != '-'; }

int check_value_type(ParamPos *p, char *s, int row) {
  int i;

  if (p->p[row + p->value_type] == 's')
    return 1;

  if (p->p[row + p->value_type] != 'u')
    return 1;

  while (*s != '\0')
    if (*s <= '9' && *s >= '0')
      s++;
    else
      return 0;

  return 1;
}

void set_long_flag(ParamPos *p, char *s) {
  int pos, eq;
  pos = find_long_matching(p, s);

  if (p->p[pos + p->takes_value] != '|') {
    strcpy(p->p + pos + p->takes_value, "on");
    return;
  }

  for (eq = 0; s[eq] != '='; eq++) {}
  eq++;
  if (check_value_type(p, s + eq, pos))
    strcpy(p->p + pos + p->value_buffer, s + eq);
  else
    fprintf(stderr, "'%s' is an invalid value for %s\n", s + eq, s);
}

void set_short_flag(ParamPos *p, int argc, char **argv, int *i) {
  int pos;
  pos = find_short_matching(p, argv[*i]);

  if (pos < 0) {
    fprintf(stderr, "Invalid short flag '%s', try --help\n", argv[*i]);
    return;
  }

  if (p->p[pos + p->takes_value] != '|') {
    strcpy(p->p + pos + p->takes_value, "on");
    return;
  }

  (*i)++;
  if (*i >= argc) {
    fprintf(stderr, "Missing argument for flag '%s' (%s)\n",
        argv[*i - 1], p->p + pos + p->help);
    return;
  }

  if (strlen(argv[*i]) > MAX_PARAM_LEN) {
    fprintf(stderr, "Argument '%s' too long (max is %d bytes)\n",
        argv[*i], MAX_PARAM_LEN);
    return;
  }

  strcpy(p->p + pos + p->value_buffer, argv[*i]);
}

void set_input_file(ParamPos *p, char *name) {
  int pos;
  pos = find_short_matching(p, "-i");

  if (strlen(name) > MAX_PARAM_LEN) {
    fprintf(stderr, "Input file name '%s' too long (max is %d bytes)\n",
        name, MAX_PARAM_LEN);
    return;
  }

  strcpy(p->p + pos + p->value_buffer, name);
}

void set_output_dir(ParamPos *p, char *name) {
  int pos;
  pos = find_short_matching(p, "-o");

  if (strlen(name) > MAX_PARAM_LEN) {
    fprintf(stderr, "Output dir name '%s' too long (max is %d bytes)\n",
        name, MAX_PARAM_LEN);
    return;
  }

  strcpy(p->p + pos + p->value_buffer, name);
}

void fill_them(ParamPos *p, int argc, char **argv) {
  int i;

  for (i = 1; i < argc; i++)

    if (get_is_long_flag(argv[i]))
      set_long_flag(p, argv[i]);

    else if (get_is_short_flag(argv[i]))
      set_short_flag(p, argc, argv, &i);

    else
      break;

  if (i < argc)
    set_input_file(p, argv[i++]);

  if (i < argc)
    set_output_dir(p, argv[i++]);
}

void translate(ParamPos *p, Params *params) {

  params->show_help = 'n' == *(p->p + p->takes_value + 1 + find_short_matching(p, "-h"));
  params->show_params = 'n' == *(p->p + p->takes_value + 1 + find_short_matching(p, "-p"));

  params->num_of_children = atoi(p->p + p->value_buffer + find_short_matching(p, "-c"));;
  params->loops_per_child = atoi(p->p + p->value_buffer + find_short_matching(p, "-r"));;
  params->microsecond_delay = atoi(p->p + p->value_buffer + find_short_matching(p, "-m"));
  params->file_segment_length = atoi(p->p + p->value_buffer + find_short_matching(p, "-l"));

  strcpy(params->output_dir, p->p + p->value_buffer + find_short_matching(p, "-o"));
  strcpy(params->input_file, p->p + p->value_buffer + find_short_matching(p, "-i"));

  strcpy(params->sem_name_i_want, p->p + p->value_buffer + find_short_matching(p, "-W"));
  strcpy(params->sem_name_thank_you, p->p + p->value_buffer + find_short_matching(p, "-T"));
  strcpy(params->shmem_name_i_want, p->p + p->value_buffer + find_short_matching(p, "-w"));
  strcpy(params->shmem_name_thank_you, p->p + p->value_buffer + find_short_matching(p, "-t"));
}

void fill_and_translate(ParamPos *p, Params *params, int argc, char **argv) {
  fill_them(p, argc, argv);
  translate(p, params);
  if (params->show_params)
    raw_params_print(p);
  else if (params->show_help)
    raw_params_help(p);
}

void raw_params_parse(Params *p, int argc, char **argv) {
  raw_params_callback(p, argc, argv, fill_and_translate);
}

void append_to_value(ParamPos *p, char *flag, char *tail) {
  int point, end;
  point = p->value_buffer + find_short_matching(p, flag);
  end = point + strlen(p->p + point);
  strcpy(p->p + end, tail);
}

void raw_params_callback(Params *p, int argc, char **argv,
    void (*callback) (ParamPos *s, Params *params, int argc, char **argv)) {

  /*                                                               123456789 123456789 123456789 123456789 123456789 123456789 1234*/
  char str[] =
    " f\0 F\0                   h\0                              b\0                                                             e\0"
    " h\0 help\0                print this help\0                |off\0                                                           \0"
    " p\0 print\0               print defaults, plus overrides\0 |off\0                                                           \0"
    " c\0 children\0            number of children\0             u|8\0                                                            \0"
    " r\0 loops\0               loops per child\0                u|1024\0                                                         \0"
    " i\0 input\0               input file\0                     s|../data/1001-line-numbers.dat\0                                \0"
    " o\0 output\0              output directory\0               s|output\0                                                       \0"
    " W\0 sem-i-want\0          'I want' semaphore\0             s|sem_i_want-\0                                                  \0"
    " T\0 sem-thank-you\0       'thank you' semaphore\0          s|sem_thank_you-\0                                               \0"
    " w\0 shm-i-want\0          'I want' shared memory name\0    s|shm_i_want-\0                                                  \0"
    " t\0 shm-thank-you\0       'thank you' shared memory name\0 s|shm_thank_you-\0                                               \0"
    " l\0 file-segment-length\0 lines in file segment\0          u|128\0                                                          \0"
    " m\0 microsecond-delay\0   children's fake delay\0          u|20000\0                                                        \0"
    "$"
    ;

  ParamPos param_pos;
  char pid[32];

  param_pos.p = str;
  param_pos_init(&param_pos);

  sprintf(pid, "%d", getpid());
  append_to_value(&param_pos, "-W", pid);
  append_to_value(&param_pos, "-T", pid);
  append_to_value(&param_pos, "-w", pid);
  append_to_value(&param_pos, "-t", pid);

  callback(&param_pos, p, argc, argv);
}

void raw_params_print(ParamPos *p) {
  int row;
  char buf[1024];

  for (row = p->len; p->p[row] != '$'; row += p->len) {

    if (p->p[row + p->takes_value] == '|') {
      sprintf(buf, "-%s VALUE , --%s=VALUE",
          p->p + row + p->short_flag,
          p->p + row + p->long_flag
          );
      printf("%-40s   %s (%s now)\n",
          buf,
          p->p + row + p->help,
          p->p + row + p->value_buffer
          );
    }
    else {
      sprintf(buf, "-%s , --%s",
          p->p + row + p->short_flag,
          p->p + row + p->long_flag
          );
      printf("%-40s   %s (%s now)\n",
          buf,
          p->p + row + p->help,
          p->p + row + p->takes_value
          );
    }

  }
}

void raw_params_help(ParamPos *p) {

  printf("\n  Useage: ./rlr [options] [input [output]]\n\n");

  raw_params_print(p);

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
